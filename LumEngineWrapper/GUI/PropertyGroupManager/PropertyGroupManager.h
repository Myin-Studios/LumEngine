#pragma once

#include "Rendering/Renderer.h"
#include "Entities/Properties/Properties.h"
#include "Engine/GUI/PropertyGroupManager/PropertyGroupManagerCore.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN byte
#include <msclr/marshal_cppstd.h>
#undef WIN32_LEAN_AND_MEAN
#endif
#include <msclr/gcroot.h>
#include <vcclr.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace LumScripting::Script::Properties;

public ref class UIManager {
public:
    UIManager();
    List<Property^>^ GetProperties();
    BaseEntity* GetSelectedEntity();
    std::string GetPropertyFromEntity(Property^ prop);

private:
    List<Property^>^ properties;
};

class UIManagerCore : public IUIManager {
private:
    gcroot<UIManager^> managed;
public:
    UIManagerCore(UIManager^ owner) : managed(owner) {}
    void UpdateUI() override;
};

class UIManagerFactory : public UIManagerFactoryBase {
public:
	UIManagerFactory() {
		System::Console::WriteLine("UIManagerFactory created");
	}

	~UIManagerFactory() {
		System::Console::WriteLine("UIManagerFactory destroyed");
	}

    std::unique_ptr<IUIManager> CreateUIManager() override {
        return std::unique_ptr<UIManagerCore>(new UIManagerCore(gcnew UIManager()));
    }
};

public ref class UIManagerManaged {
public:
	UIManagerManaged() {
		static UIManagerFactory* factory = new UIManagerFactory();
		UIManagerFactoryBase::SetFactoryInstance(factory);
	}
};

UIManager::UIManager() {
	properties = gcnew List<Property^>();
	for (int i = 0; i < Renderer::Instance->GetEntityCount(); i++) {
		BaseEntity* entity = Renderer::Instance->GetEntityAt(i);
		const auto& entityProperties = entity->GetProperties();
		for (const auto& prop : entityProperties) {
			try {
				Property^ managedProp;
				managedProp->CreateFromNative(prop.get());
				properties->Add(managedProp);
			}
			catch (Exception^ ex) {
				System::Console::WriteLine("Could not create instance: {0}", ex->Message);
			}
		}
	}
}

BaseEntity* UIManager::GetSelectedEntity()
{
	for (int i = 0; i < Renderer::Instance->GetEntityCount(); i++)
	{
		BaseEntity* entity = Renderer::Instance->GetEntityAt(i);
		if (entity->IsSelected())
		{
			return entity;
		}
	}
	return nullptr;
}

std::string UIManager::GetPropertyFromEntity(Property^ prop)
{
	BaseEntity* entity = GetSelectedEntity();
	if (entity != nullptr)
	{
		if (entity->GetProperty(prop->GetNativeTypeInfo()) != nullptr)
		{
			Type^ type = prop->GetType();
			System::Console::WriteLine("Property found: {0}", type->Name);
			return msclr::interop::marshal_as<std::string>(type->Name);
		}
	}
	return "";
}

List<Property^>^ UIManager::GetProperties() {
	return properties;
}

void UIManagerCore::UpdateUI() {
	for (int i = 0; i < managed->GetProperties()->Count; i++) {
		IPropertyUIBridge::Instance->AddElement("PROPERTIES",
			managed->GetPropertyFromEntity(managed->GetProperties()[i]));
	}
}

// using namespace System;
// using namespace System::Collections::Generic;
// using namespace LumScripting::Script::Properties;
// 
// ref class UIManager;
// 
// class UIManagerCore : public IUIManager
// {
// private:
// 	gcroot<UIManager^> managed;
// 
// public:
// 	UIManagerCore(UIManager^ owner) : managed(owner) {}
// 
// 	void UpdateUI() override
// 	{
// 		for (int i = 0; i < managed->GetProperties()->Count; i++)
// 		{
// 			IPropertyUIBridge::Instance->AddElement("PROPERTIES",
// 				managed->GetPropertyFromEntity(managed->GetProperties()[i]));
// 		}
// 	}
// };
// 
// public ref class UIManager
// {
// public:
// 	UIManager() {
// 		properties = gcnew List<Property^>();
// 		// Ottieni tutte le proprietà disponibili nel tuo assembly
// 		Array^ assemblies = AppDomain::CurrentDomain->GetAssemblies();
// 		for each (Assembly ^ assembly in assemblies) {
// 			array<Type^>^ types = assembly->GetTypes();
// 			for each (Type ^ type in types) {
// 				if (type->IsSubclassOf(Property::typeid) && !type->IsAbstract) {
// 					// Crea un'istanza della proprietà
// 					try {
// 						Property^ prop = static_cast<Property^>(Activator::CreateInstance(type));
// 						properties->Add(prop);
// 					}
// 					catch (Exception^ ex) {
// 						System::Console::WriteLine("Could not create instance of {0}: {1}",
// 							type->Name, ex->Message);
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	List<Property^>^ GetProperties() {
// 		return properties;
// 	}
// 
// 	BaseEntity* GetSelectedEntity()
// 	{
// 		for (int i = 0; i < Renderer::Instance->GetEntityCount(); i++)
// 		{
// 			BaseEntity* entity = Renderer::Instance->GetEntityAt(i);
// 			if (entity->IsSelected())
// 			{
// 				return entity;
// 			}
// 		}
// 		return nullptr;
// 	}
// 
// 	std::string GetPropertyFromEntity(Property^ prop)
// 	{
// 		BaseEntity* entity = GetSelectedEntity();
// 		if (entity != nullptr)
// 		{
// 			if (entity->GetProperty(prop->GetNativeTypeInfo()) != nullptr)
// 			{
// 				Type^ type = prop->GetType();
// 				System::Console::WriteLine("Property found: {0}", type->Name);
// 				return msclr::interop::marshal_as<std::string>(type->Name);
// 			}
// 		}
// 		return "";
// 	}
// 
// private:
// 	List<Property^>^ properties;
// };