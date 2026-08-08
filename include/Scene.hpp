#pragma once

#include <vector>
#include "GameObject.hpp"

namespace KQ {
    struct Scene
    {
        std::vector<KQ::GameObject> gameObjects;

        void Cleanup() {
            gameObjects.clear();
        }
    };
}