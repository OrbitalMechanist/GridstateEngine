#pragma once

#include <utility>
#include <memory>
#include <string>
#include "ecs/CommonECS.h"

class Component;

class GameObj {
public:
    std::string gameName;

    GameObj(std::string inputName, std::vector<Component*> inputComponents);

	GameObj();

	~GameObj();

    void setSerializedName(std::string input);

    Component* addComponent(std::shared_ptr<Component> c);

	//std::vector<Component*> addComponents(std::vector<Component*> input); previous implementation

	void update(float deltaTime);

	//This section consists of code from Max's previous implementation of an ECS

    template<typename T = Component>
    std::vector<Component*> getComponentsOfType() {
        static_assert(std::is_base_of<Component, T>::value, "type must be a child of Component");
        std::vector<Component*> result = std::vector<Component*>();
        for (auto& i : components) {
            if (dynamic_cast<T*>(i.get())) {
                result.push_back(i.get());
            }
        }
        return result;
    }

    template<typename T = Component>
    Component* getComponent() {
        static_assert(std::is_base_of<Component, T>::value, "type must be a child of Component");
        for (auto& i : components) {
            if (dynamic_cast<T*>(i.get())) {
                return i.get();
            }
        }
        return nullptr;
    }

	void removeComponent(Component* c);

private:
    std::string serializedName;
	//std::vector<Component*> components;
    std::vector<std::shared_ptr<Component>> components;
};