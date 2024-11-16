#pragma once

#include "Properties/Property.h"

#include "set"
#include <iostream>
#include <typeindex>

class BaseEntity
{
private:
    std::set<std::unique_ptr<IProperty>> properties;

public:
    template<typename T>
    void AddProperty(std::unique_ptr<T> prop) {
		properties.insert(std::move(prop));
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
        std::cout << "Property requested: " << expectedType.name() << std::endl;
        std::cout << "Number of properties in set: " << properties.size() << std::endl;

        auto it = std::find_if(properties.begin(), properties.end(),
            [&expectedType](const std::unique_ptr<IProperty>& prop) {
                std::cout << "Checking property type: " << typeid(*prop).name() << std::endl;
                return typeid(*prop) == expectedType;
            });

        return it != properties.end() ? it->get() : nullptr;
    }
};