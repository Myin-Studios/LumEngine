#pragma once

#include <iostream>
#include <string>

class IProperty {
public:
    virtual ~IProperty() = default;

    virtual void OnSerialize() {}
    virtual void OnDeserialize() {}
};