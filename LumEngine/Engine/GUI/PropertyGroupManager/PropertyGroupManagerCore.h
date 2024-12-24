#pragma once

#include "../Panels/properties.h"
#include "../Panels/panel.h"
#include "../LumTypes/Entities/Properties/Property.h"
#include "../LumTypes/Entities/Entity.h"
#include "Engine/GUI/Rendering/renderer.h"

#include <iostream>
#include <algorithm>
#include <memory>

#include "Engine/Core/LumEngineAPI.h"

#include "Engine/GUI/Rendering/renderer.h"

class IUIManager
{
public:
    virtual void UpdateUI() {};
};

class UIManagerFactoryBase {
public:
    virtual ~UIManagerFactoryBase() = default;
    virtual std::unique_ptr<IUIManager> CreateUIManager() = 0;

    static std::unique_ptr<IUIManager> Create() {
        std::cout << "UIManagerFactoryBase::Create() called, instance is: " << &Instance << ": " << Instance << std::endl;
        if (Instance) {
            return Instance->CreateUIManager();
        }
        std::cout << "UIManagerFactoryBase::Create() called with no instance set" << std::endl;
        return nullptr;
    }

    static void SetInstance(UIManagerFactoryBase* factory) {
        std::cout << "UIManagerFactoryBase::SetInstance() called" << std::endl;
        if (factory) { 
            std::cout << "UIManagerFactoryBase::SetInstance() called with valid factory at: " << &factory << ": " << factory << std::endl;
            Instance = factory;
            std::cout << "Assignment completed successfully, instance is now: " << &Instance << ": " << Instance << std::endl;
        }
    }

    __declspec(dllexport) static void SetFactoryInstance(UIManagerFactoryBase* factory)
    {
        Instance = factory;
    }
    __declspec(dllexport) static UIManagerFactoryBase* GetFactoryInstance()
    {
        return Instance;
    }

    static UIManagerFactoryBase* Instance;
};

inline UIManagerFactoryBase* UIManagerFactoryBase::Instance = nullptr;

class PropertyGroupManagerCore
{
public:
    PropertyGroup* createPropertyGroup(QWidget* parent, const std::string& title);
};