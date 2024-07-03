/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///                                      THIS CODE IS PART OF:                                        ///
///                                       CryoMoon Engine (C)                                         ///
///                                                                                                   ///
///                                     WHICH IS LICENSED UNDER                                       ///
///                                          MIT License                                              ///
///                                      All Right Reserved.                                          ///
///                                                                                                   ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///   Copyright (c) 2024 Myin Studios                                                                 ///
///                                                                                                   ///
///   Permission is hereby granted, free of charge,  to any person obtaining a copy of this software  ///
///   and associated documentation files (the "Software"), to deal in the Software without            ///
///   restriction, including without limitation the rights to use, copy, modify, merge, publish,      ///
///   distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the   ///
///   Software is furnished to do so, subject to the following conditions:                            ///
///                                                                                                   ///
///   The above copyright notice and this permission notice shall be included in all copies or        ///
///   substantial portions of the Software.                                                           ///
///                                                                                                   ///
///   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING   ///
///   BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND      ///
///   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,    ///
///   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  ///
///   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.         ///
///                                                                                                   ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <unordered_map>
#include <typeindex>

using namespace std;

class BaseProperty;

class PropertyManager
{
public:
    PropertyManager()
    {
        if (properties == nullptr)
        {
            properties = new unordered_map<type_index, shared_ptr<BaseProperty>>();
        }
    }
    ~PropertyManager() = default;

    template<typename T>
    void addProperty();

    template<typename T>
    shared_ptr<T> getProperty();

    void getPropertyList();

    unordered_map<type_index, shared_ptr<BaseProperty>>* properties = nullptr;
private:
};

class BaseProperty
{
public:
    BaseProperty() = default;
    virtual ~BaseProperty() = default;

    string getClassName() const { return typeid(*this).name(); }
};

//-----------------------{ Methods definitions }-----------------------

template<typename T>
void PropertyManager::addProperty()
{
    if (properties == nullptr)
        properties = new unordered_map<type_index, shared_ptr<BaseProperty>>();

    static_assert(is_base_of<BaseProperty, T>::value, "The property must derive from BaseProperty");
    auto ty = type_index(typeid(T));
    if (properties->find(ty) == properties->end())
    {
        properties->emplace(ty, make_shared<T>());
    }
}

template<typename T>
shared_ptr<T> PropertyManager::getProperty()
{
    if (properties == nullptr)
    {
        Log::Error("Property list is null! Add some property to make it non-null.");
        throw runtime_error("Property list is null! Add some property to make it non-null.");
    }

    if (properties->empty())
    {
        Log::Error("Property list is empty!");
        throw runtime_error("Property list is empty!");
    }

    auto ty = type_index(typeid(T));

    if (properties->find(ty) != properties->end())
        return dynamic_pointer_cast<T>(properties->at(ty));

    Log::Error("Property not found!");
    return nullptr;
}

void PropertyManager::getPropertyList()
{
    if (properties == nullptr)
    {
        Log::Error("Property list is null! Add some property to make it non-null.");
        return;
    }

    if (properties->empty())
    {
        Log::Error("Property list is empty!");
        return;
    }

    for (auto& it : *properties)
    {
        Log::Debug(it.second.get()->getClassName());
    }
}
