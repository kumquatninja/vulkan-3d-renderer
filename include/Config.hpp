#pragma once

namespace KQ {
    struct EngineConfig {
        float lookSensitivity = 0.2f;
        int windowWidth = 800;
        int windowHeight = 600;
    };

    EngineConfig LoadEngineConfig(const char* filePath);
}