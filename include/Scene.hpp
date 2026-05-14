#pragma once

#include <vector>

namespace KQ {
    struct GameObject;

    struct Scene
    {
        std::vector<KQ::GameObject> gameObjects;

        void Cleanup() {
            gameObjects.clear();
        }
    };
}