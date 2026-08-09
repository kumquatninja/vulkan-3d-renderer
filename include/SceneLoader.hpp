#pragma once

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace KQ {
    struct SceneDefinition {
        struct CameraData {
            glm::vec3 position{0.0f};
            glm::vec3 lookAt{0.0f, 0.0f, 1.0f};
        };

        struct ObjectData {
            std::string name;
            std::string modelPath;
            std::string texturePath;
            glm::vec3 position{0.0f};
            glm::vec3 rotation{0.0f};
            glm::vec3 scale{1.0f};
        };

        CameraData camera;
        std::vector<ObjectData> objects;
    };

    namespace SceneLoader {
        inline std::string ReadFile(const std::string& filePath) {
            std::ifstream input(filePath);
            if (!input.is_open()) {
                return {};
            }

            std::string content((std::istreambuf_iterator<char>(input)),
                                std::istreambuf_iterator<char>());
            return content;
        }

        inline void SkipWhitespace(const std::string& text, size_t& index) {
            while (index < text.size() && std::isspace(static_cast<unsigned char>(text[index]))) {
                ++index;
            }
        }

        inline bool ConsumeChar(const std::string& text, size_t& index, char expected) {
            SkipWhitespace(text, index);
            if (index >= text.size() || text[index] != expected) {
                return false;
            }

            ++index;
            return true;
        }

        inline bool ParseString(const std::string& text, size_t& index, std::string& outValue) {
            SkipWhitespace(text, index);

            if (index >= text.size() || text[index] != '"') {
                return false;
            }

            ++index;
            std::string value;

            while (index < text.size()) {
                const char c = text[index++];

                if (c == '"') {
                    outValue = value;
                    return true;
                }

                if (c == '\\') {
                    if (index >= text.size()) {
                        return false;
                    }

                    const char escaped = text[index++];
                    switch (escaped) {
                        case '"': value.push_back('"'); break;
                        case '\\': value.push_back('\\'); break;
                        case 'n': value.push_back('\n'); break;
                        case 't': value.push_back('\t'); break;
                        default: value.push_back(escaped); break;
                    }
                } else {
                    value.push_back(c);
                }
            }

            return false;
        }

        inline bool ParseNumber(const std::string& text, size_t& index, float& outValue) {
            SkipWhitespace(text, index);

            const size_t start = index;
            if (index < text.size() && (text[index] == '-' || text[index] == '+')) {
                ++index;
            }

            bool hasDigits = false;
            while (index < text.size() && std::isdigit(static_cast<unsigned char>(text[index]))) {
                ++index;
                hasDigits = true;
            }

            if (index < text.size() && text[index] == '.') {
                ++index;
                while (index < text.size() && std::isdigit(static_cast<unsigned char>(text[index]))) {
                    ++index;
                    hasDigits = true;
                }
            }

            if (index < text.size() && (text[index] == 'e' || text[index] == 'E')) {
                ++index;
                if (index < text.size() && (text[index] == '-' || text[index] == '+')) {
                    ++index;
                }

                bool exponentDigits = false;
                while (index < text.size() && std::isdigit(static_cast<unsigned char>(text[index]))) {
                    ++index;
                    exponentDigits = true;
                }

                if (!exponentDigits) {
                    return false;
                }
            }

            if (!hasDigits) {
                return false;
            }

            const std::string token = text.substr(start, index - start);
            try {
                outValue = std::stof(token);
                return true;
            } catch (...) {
                return false;
            }
        }

        inline bool ParseVec3(const std::string& text, size_t& index, glm::vec3& outValue) {
            if (!ConsumeChar(text, index, '[')) {
                return false;
            }

            float x = 0.0f;
            float y = 0.0f;
            float z = 0.0f;

            if (!ParseNumber(text, index, x)) {
                return false;
            }

            if (!ConsumeChar(text, index, ',')) {
                return false;
            }

            if (!ParseNumber(text, index, y)) {
                return false;
            }

            if (!ConsumeChar(text, index, ',')) {
                return false;
            }

            if (!ParseNumber(text, index, z)) {
                return false;
            }

            if (!ConsumeChar(text, index, ']')) {
                return false;
            }

            outValue = glm::vec3(x, y, z);
            return true;
        }

        inline bool ParseCamera(const std::string& text, size_t& index, SceneDefinition::CameraData& camera) {
            if (!ConsumeChar(text, index, '{')) {
                return false;
            }

            std::string key;
            if (!ParseString(text, index, key) || key != "position") {
                return false;
            }

            if (!ConsumeChar(text, index, ':')) {
                return false;
            }

            if (!ParseVec3(text, index, camera.position)) {
                return false;
            }

            if (!ConsumeChar(text, index, ',')) {
                return false;
            }

            if (!ParseString(text, index, key) || key != "lookAt") {
                return false;
            }

            if (!ConsumeChar(text, index, ':')) {
                return false;
            }

            if (!ParseVec3(text, index, camera.lookAt)) {
                return false;
            }

            if (!ConsumeChar(text, index, '}')) {
                return false;
            }

            return true;
        }

        inline bool ParseObject(const std::string& text, size_t& index, SceneDefinition::ObjectData& object) {
            if (!ConsumeChar(text, index, '{')) {
                return false;
            }

            std::string key;

            // name
            if (!ParseString(text, index, key) || key != "name") {
                return false;
            }
            if (!ConsumeChar(text, index, ':')) {
                return false;
            }
            if (!ParseString(text, index, object.name)) {
                return false;
            }

            if (!ConsumeChar(text, index, ',')) {
                return false;
            }

            // model
            if (!ParseString(text, index, key) || key != "model") {
                return false;
            }
            if (!ConsumeChar(text, index, ':')) {
                return false;
            }
            if (!ParseString(text, index, object.modelPath)) {
                return false;
            }

            if (!ConsumeChar(text, index, ',')) {
                return false;
            }

            // texture
            if (!ParseString(text, index, key) || key != "texture") {
                return false;
            }
            if (!ConsumeChar(text, index, ':')) {
                return false;
            }
            if (!ParseString(text, index, object.texturePath)) {
                return false;
            }

            if (!ConsumeChar(text, index, ',')) {
                return false;
            }

            // position
            if (!ParseString(text, index, key) || key != "position") {
                return false;
            }
            if (!ConsumeChar(text, index, ':')) {
                return false;
            }
            if (!ParseVec3(text, index, object.position)) {
                return false;
            }

            if (!ConsumeChar(text, index, ',')) {
                return false;
            }

            // rotation
            if (!ParseString(text, index, key) || key != "rotation") {
                return false;
            }
            if (!ConsumeChar(text, index, ':')) {
                return false;
            }
            if (!ParseVec3(text, index, object.rotation)) {
                return false;
            }

            if (!ConsumeChar(text, index, ',')) {
                return false;
            }

            // scale
            if (!ParseString(text, index, key) || key != "scale") {
                return false;
            }
            if (!ConsumeChar(text, index, ':')) {
                return false;
            }
            if (!ParseVec3(text, index, object.scale)) {
                return false;
            }

            if (!ConsumeChar(text, index, '}')) {
                return false;
            }

            return true;
        }

        inline bool ParseObjectArray(const std::string& text, size_t& index, std::vector<SceneDefinition::ObjectData>& outObjects) {
            if (!ConsumeChar(text, index, '[')) {
                return false;
            }

            SkipWhitespace(text, index);

            if (index < text.size() && text[index] == ']') {
                ++index;
                return true;
            }

            while (true) {
                SceneDefinition::ObjectData object;
                if (!ParseObject(text, index, object)) {
                    std::cerr << "Failed to parse object entry" << std::endl;
                    return false;
                }

                outObjects.push_back(object);

                SkipWhitespace(text, index);

                if (index < text.size() && text[index] == ',') {
                    ++index;
                    SkipWhitespace(text, index);
                    continue;
                }

                break;
            }

            if (!ConsumeChar(text, index, ']')) {
                std::cerr << "Expected ']' at the end of objects array" << std::endl;
                return false;
            }

            return true;
        }

        inline bool LoadScene(const std::string& filePath, SceneDefinition& outScene) {
            const std::string text = ReadFile(filePath);
            if (text.empty()) {
                std::cerr << "Scene file is empty or could not be read: " << filePath << std::endl;
                return false;
            }

            size_t index = 0;

            if (!ConsumeChar(text, index, '{')) {
                std::cerr << "Expected '{' at start of scene JSON" << std::endl;
                return false;
            }

            bool foundCamera = false;
            bool foundObjects = false;

            while (index < text.size()) {
                SkipWhitespace(text, index);

                if (text[index] == '}') {
                    ++index;
                    break;
                }

                std::string key;
                if (!ParseString(text, index, key)) {
                    std::cerr << "Expected object key in scene JSON" << std::endl;
                    return false;
                }

                if (!ConsumeChar(text, index, ':')) {
                    std::cerr << "Expected ':' after scene key '" << key << "'" << std::endl;
                    return false;
                }

                if (key == "camera") {
                    if (!ParseCamera(text, index, outScene.camera)) {
                        std::cerr << "Failed to parse camera block" << std::endl;
                        return false;
                    }
                    foundCamera = true;
                } else if (key == "objects") {
                    if (!ParseObjectArray(text, index, outScene.objects)) {
                        std::cerr << "Failed to parse objects array" << std::endl;
                        return false;
                    }
                    foundObjects = true;
                } else {
                    std::cerr << "Unsupported scene key: " << key << std::endl;
                    return false;
                }

                SkipWhitespace(text, index);

                if (index < text.size() && text[index] == ',') {
                    ++index;
                    continue;
                }

                SkipWhitespace(text, index);

                if (index < text.size() && text[index] == '}') {
                    ++index;
                    break;
                }
            }

            if (!foundCamera || !foundObjects) {
                std::cerr << "Scene JSON is missing camera or objects data" << std::endl;
                return false;
            }

            return true;
        }
    }
}