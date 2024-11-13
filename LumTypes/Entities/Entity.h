#pragma once

#include "Properties/Property.h"

#include "unordered_map"
#include <iostream>
#include <typeindex>

class BaseEntity
{
private:
    std::unordered_map<std::type_index, std::unique_ptr<IProperty>> properties;

public:
    template<typename T>
    void AddProperty(std::unique_ptr<T> prop) {
        properties[std::type_index(typeid(T))] = std::move(prop);
    }

    template<typename T>
    T* GetProperty() {
        auto it = properties.find(std::type_index(typeid(T)));
        return it != properties.end() ? dynamic_cast<T*>(it->second.get()) : nullptr;
    }
};