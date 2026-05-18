#include "Config.hpp"

#include <ini.h>
#include <string.h>
#include <iostream>

static int InihCallback(void* user, const char* section, const char* name, const char* value) {
    auto* config = static_cast<KQ::EngineConfig*>(user);

    if (strcmp(section, "Controls") == 0) {
        if (strcmp(name, "LookSensitivity") == 0) {
            config->lookSensitivity = static_cast<float>(atof(value));
        }
    }
    else if (strcmp(section, "Window") == 0) {
        if (strcmp(name, "Width") == 0) {
            config->windowWidth = atoi(value);
        }
        if (strcmp(name, "Height") == 0) {
            config->windowHeight = atoi(value);
        }
    }

    return 1;
}

namespace KQ {
    EngineConfig LoadEngineConfig(const char* filePath) {
        EngineConfig config;

        if (ini_parse(filePath, InihCallback, &config) < 0) {
            std::cout << "[Config] Could not open " << filePath << ", using engine defaults.\n";
        }
        else {
            std::cout << "[Config] Successfully loaded configuration from " << filePath << "\n";
        }

        return config;
    }
}