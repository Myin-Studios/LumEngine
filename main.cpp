#if defined(_WIN32)
#include <Windows.h>
#else
#include <dlfcn.h>
#include <unistd.h>
#endif

#include <iostream>
#include <filesystem>
#include <string>

#include <nethost.h>
#include <coreclr_delegates.h>
#include <hostfxr.h>

#include "QApplication"
#include "QCoreApplication"
#include "QStyleFactory"
#include "Engine/GUI/guibuilder.h"

// Puntatori a funzione
hostfxr_initialize_for_runtime_config_fn init_fptr = nullptr;
hostfxr_get_runtime_delegate_fn get_delegate_fptr = nullptr;
hostfxr_close_fn close_fptr = nullptr;

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
    hostfxr_handle cxt = nullptr;

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

typedef void (*script_runner)();

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
        std::cerr << "rc: " << rc << endl << "func_ptr: " << func_ptr << endl;
        return -1;
    }

    return rc;
}

int RunEngine(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/platforms");
    QCoreApplication::setAttribute(Qt::AA_NativeWindows, false);

    QSurfaceFormat format;
    format.setVersion(4, 5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);

    QSurfaceFormat::setDefaultFormat(format);

    auto builder = new GuiBuilder();

#if defined(Q_OS_WIN)
    QApplication::setStyle(QStyleFactory::create("Fusion")); // Tema Fusion per Windows
    QIcon icon(":/Icons/CMEngine_Logo.ico");
    if (!icon.isNull()) {
        builder->getMainWindow()->setWindowIcon(icon);
    }

#elif defined(Q_OS_LINUX)
    if (QSysInfo::productVersion().startsWith("5")) {
        // Imposta il tema Plasma se disponibile su Linux
        QApplication::setStyle(QStyleFactory::create("Fusion")); // Cambia con "Plasma" se supportato
    }

    if (QIcon icon(":/Icons/CMEngine_Logo.svg"); icon.isNull()) {
        qWarning() << "Icon not found!";
    } else {
        QApplication::setWindowIcon(icon);
        builder->getMainWindow()->setWindowIcon(icon);
    }
#endif

    return QApplication::exec();
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

int main(int argc, char* argv[])
{
    SetEnvironmentVariable(L"COREHOST_TRACE", L"1");

// Carica e inizializza l'assembly .NET
    if (!load_hostfxr()) {
        std::cerr << "Failed to load hostfxr." << std::endl;
        return 1;
    }

    // Percorso al file di configurazione .NET
    const char_t* config_path = L"../bin/Debug/net8.0/win-x64/publish/CMEngine.runtimeconfig.json";

    // Carica l'assembly e ottieni il puntatore alla funzione
    assembly_loader = get_dotnet_load_assembly(config_path);
    if (assembly_loader == nullptr) {
        std::cerr << "Failed to get assembly loader." << std::endl;
        return EXIT_FAILURE;
    }

    // Percorso all'assembly C#
    const wstring assembly_path = L"../bin/Debug/net8.0/win-x64/publish/CMEngine.dll";
    const wchar_t *dotnet_type = L"CMEngine.CS.Program, CMEngine.CS";
    const wchar_t *dotnet_type_method = L"Example";
    const wchar_t *delegate_type = L"System.Action";  // Delegate per un metodo void senza parametri

    component_entry_point_fn example_func = nullptr;

    int rc = assembly_loader(
            assembly_path.c_str(),
            dotnet_type,
            dotnet_type_method,
            delegate_type,
            nullptr,
            (void**)&example_func
    );

    cout << std::hex << rc << endl;

//    int rc = load_managed_function(assembly_path.c_str(), dotnet_type, dotnet_type_method, example_func);

    return RunEngine(argc, argv);
}