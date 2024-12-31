#pragma once

#include "Properties/Property.h"
#include "../GameBehaviour/GameBehaviour.h"

#include "set"
#include <iostream>
#include <typeindex>

class BaseEntity
{
private:
    std::set<std::unique_ptr<IProperty>> properties;
    std::set<std::unique_ptr<IGameBehaviour>> scripts;
    int ID = 0;
    static inline int nextID = 0;

    bool isSelected = false;

public:
    BaseEntity() : ID(nextID++) {}

    bool IsSelected() const { return isSelected; }
    void SetSelected(bool selected) { isSelected = selected; }

    const int GetEntityID() const { return ID; }

    void AddScript(std::unique_ptr<IGameBehaviour> script)
    {
        scripts.insert(std::move(script));
    }

    void RemoveScript(IGameBehaviour* script)
    {
        scripts.erase(
            std::find_if(scripts.begin(), scripts.end(),
                [script](const auto& s) { return s.get() == script; })
        );
    }

    IGameBehaviour* GetScript(const type_info& scriptType)
    {
        auto it = std::find_if(scripts.begin(), scripts.end(),
            [&scriptType](const auto& s) { return typeid(*s) == scriptType; });
        return it != scripts.end() ? it->get() : nullptr;
    }

    template<typename T>
    void AddProperty(std::unique_ptr<T> prop) {
        IProperty* rawPtr = prop.get();
        properties.insert(std::move(prop));
        rawPtr->OnSerialize();
    }

    template<typename T>
    T* GetCoreProperty() {
        const type_info& requestedType = typeid(T);
        auto it = std::find_if(properties.begin(), properties.end(),
            [&requestedType](const std::unique_ptr<IProperty>& prop) {
                return typeid(*prop) == requestedType;
            });

        return it != properties.end() ? static_cast<T*>(it->get()) : nullptr;
    }

    IProperty* GetProperty(const type_info& expectedType) {
        auto it = std::find_if(properties.begin(), properties.end(),
            [&expectedType](const std::unique_ptr<IProperty>& prop) {
                return typeid(*prop) == expectedType;
            });

        return it != properties.end() ? it->get() : nullptr;
    }

    const std::set<std::unique_ptr<IProperty>>& GetProperties() const {
        return properties;
    }

    void SerializeProperties() {
        for (auto& prop : properties) {
            prop->OnSerialize();
        }
    }

    void DeserializeProperties() {
        for (auto& prop : properties) {
            prop->OnDeserialize();
        }
    }
};