#pragma once

#define WIN32_LEAN_AND_MEAN byte
#include "Windows.h"
#undef WIN32_LEAN_AND_MEAN

#include <cstdlib>
#include <QString>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace ScriptLoaderCore
{
	class ScriptPathLoader
	{
	public:
		ScriptPathLoader() = default;

		std::string getPath()
		{
			std::cout << "Path in native C++: " << assemblyPath << std::endl;
			return assemblyPath;
		}

		void setPath(QString path)
		{
			assemblyPath = path.toStdString();
			std::cout << "Setting path in native C++: " << assemblyPath << std::endl;
		}
		
        void createProject(const std::string& projPath, const std::string& projName)
        {
            std::string csprojPath = projPath + "/" + projName + ".csproj";
            std::string slnPath = projPath + "/" + projName + ".sln";

            std::string createSolutionCommand = "dotnet new sln -n " + projName + " -o " + projPath + " --force";
            if (std::system(createSolutionCommand.c_str()) != 0)
            {
                std::cerr << "Error during solution creation with dotnet." << std::endl;
                return;
            }

            // Usa --force per sovrascrivere eventuali file esistenti
            std::string createProjCommand = "dotnet new classlib -n " + projName + " -o " + projPath + " --force";
            if (std::system(createProjCommand.c_str()) != 0) {
                std::cerr << "Error during project creation with dotnet." << std::endl;
                return;
            }

            std::string addCSProjToSolution = "dotnet sln " + slnPath + " add " + csprojPath;
            if (std::system(addCSProjToSolution.c_str()) != 0) {
                std::cerr << "Error during project binding to the solution with dotnet." << std::endl;
                return;
            }

            std::filesystem::path absEngineDllPath(std::filesystem::absolute("LumScripting/"));
            std::string engineDllPath(absEngineDllPath.string());

            // Leggi il contenuto del file .csproj
            std::ifstream csprojFileIn(csprojPath);
            if (!csprojFileIn.is_open()) {
                std::cerr << "Unable to open .csproj file for reading!" << std::endl;
                return;
            }

            std::stringstream buffer;
            buffer << csprojFileIn.rdbuf();
            std::string csprojContent = buffer.str();
            csprojFileIn.close();

            // Cerca la chiusura di <Project> e inserisci prima di essa il nuovo ItemGroup
            std::size_t projectEndPos = csprojContent.find("</Project>");
            if (projectEndPos == std::string::npos) {
                std::cerr << "Invalid .csproj file structure!" << std::endl;
                return;
            }

            std::string referenceBlock =
                "  <ItemGroup>\n"
                "    <Reference Include=\"LumScripting\">\n"
                "      <HintPath>" + absEngineDllPath.string() + "LumScripting.dll" "</HintPath>\n"
                "    </Reference>\n"
                "    <Reference Include=\"LumEngineWrapper\">\n"
                "      <HintPath>" + absEngineDllPath.string() + "LumEngineWrapper.dll" "</HintPath>\n"
                "    </Reference>\n"
                "  </ItemGroup>\n";

            // Inserisci il nuovo blocco <ItemGroup> prima della chiusura di <Project>
            csprojContent.insert(projectEndPos, referenceBlock);

            csprojContent.replace(csprojContent.find("net8.0"), 6, "net8.0-windows10.0.26100.0");

            // Scrivi il contenuto modificato nel file .csproj
            std::ofstream csprojFileOut(csprojPath);
            if (!csprojFileOut.is_open()) {
                std::cerr << "Unable to open .csproj file for writing!" << std::endl;
                return;
            }

            csprojFileOut << csprojContent;
            csprojFileOut.close();

            std::cout << ".csproj file created and modified successfully!" << std::endl;

            // Restore e build
            restore(projPath);
            build(projPath);
        }

        void restore(std::string projPath)
        {
            std::string restoreCommand = "dotnet restore " + projPath;
            if (std::system(restoreCommand.c_str()) != 0) {
                std::cerr << "Error during NuGet package restore." << std::endl;
                return;
            }
        }

        void build(std::string projPath)
        {
            std::string buildCommand = "dotnet build " + projPath + " -c Debug";
            if (std::system(buildCommand.c_str()) != 0) {
                std::cerr << "Error during project compilation." << std::endl;
            }
            else {
                std::cout << "Project compiled successfully." << std::endl;
            }
        }

	private:
		std::string assemblyPath;

		std::string getEnvVar(const std::string& var) {
			// Ottieni la dimensione necessaria per la variabile d'ambiente
			size_t requiredSize = 0;
			getenv_s(&requiredSize, NULL, 0, var.c_str());

			// Se la variabile non esiste, restituisci una stringa vuota
			if (requiredSize == 0) {
				return "";
			}

			// Alloca un buffer per la variabile d'ambiente
			std::vector<char> buffer(requiredSize);
			getenv_s(&requiredSize, buffer.data(), requiredSize, var.c_str());

			// Converti il buffer in std::string
			return std::string(buffer.data());
		}
	};
}