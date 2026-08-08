#pragma once

#include <vector>
#include "GameObject.hpp"

namespace KQ {
    struct Scene
    {
        std::vector<KQ::GameObject> gameObjects;

        GameObject& AddGameObject(const GameObject& object = GameObject()) {
            gameObjects.push_back(object);
            return gameObjects.back();
        }

        void Cleanup() {
            gameObjects.clear();
        }
    };
}