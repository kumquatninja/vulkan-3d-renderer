#pragma once

#include <vector>

namespace KQ {
    struct MeshRange {
        uint32_t firstVertex = 0;
        uint32_t vertexCount = 0;
        uint32_t firstIndex = 0;
        uint32_t indexCount = 0;
    };
}