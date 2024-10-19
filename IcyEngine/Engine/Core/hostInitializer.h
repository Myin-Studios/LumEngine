#pragma once

#include <iostream>
#include <string>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN byte
#include "Windows.h"
#undef WIN32_LEAN_AND_MEAN
#else
#include <dlfcn.h>
#include <unistd.h>
#endif

#include "nethost.h"
#include "coreclr_delegates.h"
#include "hostfxr.h"

#include <QThread>

class LoadingThread : public QThread {
Q_OBJECT
private:
    // Puntatori a funzione
    hostfxr_initialize_for_runtime_config_fn init_fptr = nullptr;
    hostfxr_get_runtime_delegate_fn get_delegate_fptr = nullptr;
    hostfxr_close_fn close_fptr = nullptr;

    component_entry_point_fn StartScript = nullptr;
    component_entry_point_fn UpdateScript = nullptr;
    component_entry_point_fn LoadAssemblyMethod = nullptr;
    hostfxr_handle cxt = nullptr;

// Funzione per caricare la libreria dinamica
    void* load_library(const char_t* path) {
#if defined(_WIN32)
        return (void*)LoadLibraryW(path);
#else
        return dlopen(path, RTLD_LAZY);
#endif
    }

// Funzione per ottenere il simbolo esportato
    void* get_export(void* h, const char* name) {
#if defined(_WIN32)
        return (void*)GetProcAddress((HMODULE)h, name);
#else
        return dlsym(h, name);
#endif
    }

// Carica la libreria hostfxr e inizializza i puntatori a funzione
    bool load_hostfxr() {
        char_t buffer[MAX_PATH];
        size_t buffer_size = sizeof(buffer) / sizeof(char_t);

        // Trova il percorso di hostfxr
        int rc = get_hostfxr_path(buffer, &buffer_size, nullptr);
        if (rc != 0) {
            std::cerr << "Impossibile trovare hostfxr." << std::endl;
            return false;
        }

        // Carica hostfxr
        void* lib = load_library(buffer);
        if (lib == nullptr) {
            std::cerr << "Impossibile caricare hostfxr." << std::endl;
            return false;
        }

        // Carica i puntatori a funzione da hostfxr
        init_fptr = (hostfxr_initialize_for_runtime_config_fn)get_export(lib, "hostfxr_initialize_for_runtime_config");
        get_delegate_fptr = (hostfxr_get_runtime_delegate_fn)get_export(lib, "hostfxr_get_runtime_delegate");
        close_fptr = (hostfxr_close_fn)get_export(lib, "hostfxr_close");

        if (init_fptr == nullptr || get_delegate_fptr == nullptr || close_fptr == nullptr) {
            std::cerr << "Impossibile ottenere le funzioni da hostfxr." << std::endl;
            return false;
        }

        return true;
    }

// Funzione che carica l'assembly e ottiene il puntatore alla funzione
    load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const char_t *config_path)
    {
        if (!load_hostfxr()) {
            std::cerr << "Errore durante il caricamento di hostfxr." << std::endl;
            return nullptr;
        }

        void *load_assembly_and_get_function_pointer = nullptr;

        // Inizializza CoreCLR con il file di configurazione .NET
        int rc = init_fptr(config_path, nullptr, &cxt);
        if (rc != 0 || cxt == nullptr)
        {
            std::cerr << "Init failed: " << std::hex << std::showbase << rc << std::endl;
            close_fptr(cxt);
            return nullptr;
        }

        // Ottieni il puntatore a load_assembly_and_get_function_pointer
        rc = get_delegate_fptr(
                cxt,
                hdt_load_assembly_and_get_function_pointer,
                &load_assembly_and_get_function_pointer);
        if (rc != 0 || load_assembly_and_get_function_pointer == nullptr)
        {
            std::cerr << "Get delegate failed: " << std::hex << std::showbase << rc << std::endl;
        }

        close_fptr(cxt);
        return (load_assembly_and_get_function_pointer_fn)load_assembly_and_get_function_pointer;
    }

// Funzione per caricare l'assembly e ottenere il puntatore alla funzione C#
    load_assembly_and_get_function_pointer_fn assembly_loader = nullptr;

// Implementazione della funzione che ottiene il puntatore alla funzione C#
    int load_managed_function(const char_t *assembly_path, const char_t *type_name, const char_t *method_name, component_entry_point_fn func_ptr)
    {
        if (assembly_loader == nullptr)
        {
            std::cerr << "Assembly loader is null!" << std::endl;
            return -1;
        }

        int rc = assembly_loader(assembly_path, type_name, method_name, nullptr, nullptr, (void**)&func_ptr);
        if (rc != 0) {
            std::cerr << "Failed to load managed function. Error code: " << std::hex << rc << std::endl;
            std::cerr << "rc: " << rc << std::endl << "func_ptr: " << func_ptr << std::endl;
            return -1;
        }

        return rc;
    }

    load_assembly_and_get_function_pointer_fn create_loader(const char_t *config_path) {
        // Carica l'hostfxr e ottieni il delegato
        load_assembly_and_get_function_pointer_fn dotnet_delegate = get_dotnet_load_assembly(config_path);
        if (dotnet_delegate == nullptr) {
            std::cerr << "Impossibile caricare il delegato .NET." << std::endl;
            return nullptr;
        }

        // Assegna il puntatore alla funzione
        assembly_loader = dotnet_delegate;
        return assembly_loader;
    }

public:
    LoadingThread(QObject *parent = nullptr) : QThread(parent) {}
    component_entry_point_fn getStart() { return StartScript; }
    component_entry_point_fn getUpdate() { return UpdateScript; }
    component_entry_point_fn getLoader() { return LoadAssemblyMethod; }

public slots:
    void LoadAssembly(QString path)
    {
        if (path.isEmpty()) return;

        std::cout << "Loading project..." << std::endl;

        const wchar_t* assembly_path = L"../IcyScripting/bin/Debug/net8.0-windows10.0.26100.0/win-x64/IcyScripting.dll";
        const wchar_t* dotnet_type = L"IcyScripting.Script.Main.Program, IcyScripting";
        const wchar_t* dotnet_type_method_LoadAssembly = L"LoadScriptPath";

        if (assembly_loader == nullptr) {
            emit progressUpdated("Failed to get assembly loader.", 100 / 5);
            return;
        }

        int rc = assembly_loader(
            assembly_path,
            dotnet_type,
            dotnet_type_method_LoadAssembly,
            nullptr,
            nullptr,
            (void**)&LoadAssemblyMethod
        );

        // Convert QString to std::wstring and then get a wchar_t* pointer
        std::wstring wpath = path.toStdWString();
        const wchar_t* wpath_cstr = wpath.c_str();

        // Calculate the size of the string in bytes (including the null terminator)
        int32_t path_size_in_bytes = static_cast<int32_t>((wpath.size() + 1) * sizeof(wchar_t));

        // Prepare the argument array (only one argument, the string pointer)
        void* args[] = { (void*)wpath_cstr };

        // Call the function, passing the correct size of the argument in bytes
        if (rc == 0) {
            LoadAssemblyMethod((void*)wpath_cstr, path_size_in_bytes);  // Pass only one argument
        }
        else {
            std::cerr << "Can't find LoadAssembly method! " << std::hex << rc << std::endl;
        }
    }

signals:
    void progressUpdated(const QString &message, int value);  // Segnale per aggiornare il progresso
    void loadingCompleted();

protected:
    void run() override {
        emit progressUpdated("HostFXR loading...", 100 / 5);

        if (!load_hostfxr()) {
            emit progressUpdated("Failed to load hostfxr.", 0);
            return;
        }

        emit progressUpdated("Init Assembly loader...", 100 / 5 * 2);

        const char_t* config_path = L"../IcyScripting/bin/Debug/net8.0-windows10.0.26100.0/win-x64/IcyScripting.runtimeconfig.json";
        assembly_loader = get_dotnet_load_assembly(config_path);
        if (assembly_loader == nullptr) {
            emit progressUpdated("Failed to get assembly loader.", 100 / 5);
            return;
        }

        const wchar_t *delegate_type = L"System.Action";
        const wchar_t *assembly_path = L"../IcyScripting/bin/Debug/net8.0-windows10.0.26100.0/win-x64/IcyScripting.dll";
        const wchar_t *dotnet_type = L"IcyScripting.Script.Main.Program, IcyScripting";
        const wchar_t *dotnet_type_method_Start = L"Start";
        const wchar_t *dotnet_type_method_Run = L"Run";

        emit progressUpdated("Init Script Runner: 1...", 100 / 5 * 3);

        int rc = assembly_loader(
                assembly_path,
                dotnet_type,
                dotnet_type_method_Start,
                delegate_type,
                nullptr,
                (void**)&StartScript
        );

        emit progressUpdated("Init Script Runner: 2...", 100 / 5 * 4);

        if (rc == 0) {
            rc = assembly_loader(
                    assembly_path,
                    dotnet_type,
                    dotnet_type_method_Run,
                    delegate_type,
                    nullptr,
                    (void**)&UpdateScript
            );
        }

        if (rc != 0 || StartScript == nullptr || UpdateScript == nullptr) {
            std::cerr << "Failed to load managed function. Error code: " << std::hex << rc << std::endl;
            return;
        }


        // Al termine invia l'aggiornamento finale
        emit progressUpdated("Loading completed", 100);
        emit loadingCompleted();

        quit();
    }
};
