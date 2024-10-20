#pragma once

#include "../../LumEngine/Engine/Core/AssemblyLoader/ScriptPathLoader.h"

using namespace System;

namespace LumEngine
{
	namespace Script
	{
		namespace FileLoader
		{
			public ref class ScriptPathLoader
			{
			public:
				ScriptPathLoader()
				{
					splc = new ScriptLoaderCore::ScriptPathLoader();
					if (splc == nullptr)
					{
						System::Console::WriteLine("Failed to initialize ScriptPathLoader in C++.");
					}
				}

				String^ getPath()
				{
					std::string nativePath = splc->getPath();  // Recupera il path nativo
					std::cout << "Native path in C++/CLI (getPath): " << nativePath << std::endl; // Debug

					if (nativePath.empty()) {
						Console::WriteLine("Native path is empty!");
						return String::Empty;
					}

					return gcnew String(nativePath.c_str());  // Converte std::string in String^
				}

				void MarshalString(String^ s, string& os) {
					using namespace Runtime::InteropServices;
					const char* chars =
						(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
					os = chars;
					Marshal::FreeHGlobal(IntPtr((void*)chars));
				}

				/*
				void setPath(String^ path) {
					std::string nativePath;
					MarshalString(path, nativePath);
					splc->setPath(nativePath); // Assicurati che questo imposti correttamente il path
					Console::WriteLine("Path set in C++/CLI: {0}", path);
				}
				*/

				ScriptLoaderCore::ScriptPathLoader* splc = nullptr;
			};
		}
	}
}