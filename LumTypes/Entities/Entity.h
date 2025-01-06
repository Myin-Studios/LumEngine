#pragma once

#include "Properties/Property.h"
#include "../GameBehaviour/GameBehaviour.h"
#include "IEntity.h"

#include "set"
#include <iostream>
#include <typeindex>
#include "../Physics/ICollisions.h"

class BaseEntity : public IEntity
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
        for (const auto& prop : properties) {
            if (T* derived = dynamic_cast<T*>(prop.get())) {
                return derived;
            }
        }
        return nullptr;
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

    bool Intersects(const LumEngine::Physics::IRay& ray, float& distance) override
    {
        if (auto collider = GetCoreProperty<LumEngine::Physics::ICollider>()) {
            const LumEngine::Physics::IAABB& boundingBox = collider->GetBoundingBox();

            std::cout << "Testing intersection with bounds: " << std::endl;
            std::cout << "Min: " << boundingBox.GetMin().ToString() << std::endl;
            std::cout << "Max: " << boundingBox.GetMax().ToString() << std::endl;
            std::cout << "Ray origin: " << ray.GetOrigin().ToString() << std::endl;
            std::cout << "Ray direction: " << ray.GetDirection().ToString() << std::endl;

            Vec3Core rayOrigin = ray.GetOrigin();
            Vec3Core rayDirection = ray.GetDirection();
            Vec3Core min = boundingBox.GetMin();
            Vec3Core max = boundingBox.GetMax();

            // Test di intersezione usando il slab method
            float tmin = -std::numeric_limits<float>::infinity();
            float tmax = std::numeric_limits<float>::infinity();

            for (int i = 0; i < 3; ++i) {
                if (std::abs(rayDirection[i]) < std::numeric_limits<float>::epsilon()) {
                    if (rayOrigin[i] < min[i] || rayOrigin[i] > max[i])
                        return false;
                }
                else {
                    float invD = 1.0f / rayDirection[i];
                    float t1 = (min[i] - rayOrigin[i]) * invD;
                    float t2 = (max[i] - rayOrigin[i]) * invD;

                    if (t1 > t2) std::swap(t1, t2);

                    tmin = t1 > tmin ? t1 : tmin;
                    tmax = t2 < tmax ? t2 : tmax;

                    if (tmin > tmax) return false;
                }
            }

            if (tmin < 0) {
                distance = tmax;
                return tmax > 0;
            }

            distance = tmin;
            return true;
        }

		std::cout << "No collider found for entity " << ID << std::endl;
        return false;
    }

};