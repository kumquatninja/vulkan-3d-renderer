#pragma once

#include <vector>
#include <string>
#include "Vertex.h"
#include <filesystem>
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "tiny_gltf_v3.h"

const std::string MODEL_PATH = "assets/models/viking_room.obj";

namespace KQ {
namespace ModelLoader {
    inline std::uint32_t FindAttributeIndex(const tg3_primitive& primitive, const char* attributeName) {
        uint32_t attributeIndex = UINT32_MAX;
        for (uint32_t i = 0; i < primitive.attributes_count; i++)
        {
            auto pair = primitive.attributes[i];
            if (strcmp(pair.key.data, attributeName) == 0) {
                attributeIndex = i;
                break;
            }
        }

        return attributeIndex;
    }

    static void LoadModelGLTF(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, const std::string& modelPath = MODEL_PATH) {
        tg3_parse_options   opts;
        tg3_error_stack     errors;
        tg3_model           model;

        tg3_parse_options_init(&opts);
        tg3_error_stack_init(&errors);

        tg3_error_code err = tg3_parse_file(&model, &errors, modelPath.c_str(), static_cast<uint32_t>(modelPath.length()), &opts);

        if (err != TG3_OK) {
            for (uint32_t i = 0; i < errors.count; i++) {
                fprintf(stderr, "[%d] %s\n", (int)errors.entries[i].severity,
                    errors.entries[i].message ? errors.entries[i].message : "(null)");
            }
        }

        vertices.clear();
        indices.clear();

        for (uint32_t i = 0; i < model.meshes_count; i++)
        {
            const auto& mesh = model.meshes[i];

            for (uint32_t j = 0; j < mesh.primitives_count; j++)
            {
                const auto& primitive = mesh.primitives[j];

                const tg3_accessor      &indexAccessor      = model.accessors[primitive.indices];
                const tg3_buffer_view   &indexBufferView    = model.buffer_views[indexAccessor.buffer_view];
                const tg3_buffer        &indexBuffer        = model.buffers[indexBufferView.buffer];

                auto posAttributeIndex = FindAttributeIndex(primitive, "POSITION");
                assert(posAttributeIndex != UINT32_MAX);
                
                uint32_t posAccessorIndex = primitive.attributes[posAttributeIndex].value;

                const tg3_accessor&         posAccessor            = model.accessors[posAccessorIndex];
                const tg3_buffer_view&      posBufferView          = model.buffer_views[posAccessor.buffer_view];
                const tg3_buffer&           posBuffer              = model.buffers[posBufferView.buffer];

                uint32_t texCoordAttributeIndex = FindAttributeIndex(primitive, "TEXCOORD_0");
                
                bool hasTexCoords = (texCoordAttributeIndex != -1);
                const tg3_accessor*         texCoordAccessor       = nullptr;
                const tg3_buffer_view*      texCoordBufferView     = nullptr;
                const tg3_buffer*           texCoordBuffer         = nullptr;

                if (hasTexCoords) {
                    texCoordAccessor    = &model.accessors[texCoordAttributeIndex];
                    texCoordBufferView  = &model.buffer_views[texCoordAccessor->buffer_view];
                    texCoordBuffer      = &model.buffers[texCoordBufferView->buffer];
                }

                uint32_t baseVertex = static_cast<uint32_t>(vertices.size());
                const uint8_t* baseBytes = posBuffer.data.data;

                for (size_t i = 0; i < posAccessor.count; i++) {
                    Vertex vertex{};
                    
                    
                    size_t absoluteOffset = posBufferView.byte_offset + posAccessor.byte_offset + (i * 12); // stride is 3 components * 4 bytes
                    const float *pos = reinterpret_cast<const float *>(baseBytes + absoluteOffset);
                    // glTF uses a right-handed, Y-up coordinate system, while this scene is set up
                    // Z-up. Negating the Y coordinate here would mirror the model instead of
                    // reorienting it - Vulkan's clip-space Y convention is already handled
                    // separately via proj[1][1] *= -1 in updateUniformBuffer, so we keep the
                    // position as-is and let a rotation there do the Y-up to Z-up conversion.
                    vertex.pos = {pos[0], pos[1], pos[2]};

                    if (hasTexCoords) {
                        const float *texCoord = reinterpret_cast<const float *>(&texCoordBuffer->data.data[texCoordBufferView->byte_offset + texCoordAccessor->byte_offset + i * 8]);
                        vertex.texCoord       = {texCoord[0], texCoord[1]};
                    }
                    else {
                        vertex.texCoord = {0.0f, 0.0f};
                    }

                    vertex.color = {1.0f, 1.0f, 1.0f};
                    vertices.push_back(vertex);
                }

                const unsigned char* indexData = &indexBuffer.data.data[indexBufferView.byte_offset + indexAccessor.byte_offset];
                size_t               indexCount = indexAccessor.count;
                size_t               indexStride = 0;

                if (indexAccessor.component_type == TG3_COMPONENT_TYPE_UNSIGNED_SHORT) {
                    indexStride = sizeof(uint16_t);
                }
                else if (indexAccessor.component_type == TG3_COMPONENT_TYPE_UNSIGNED_INT) {
                    indexStride = sizeof(uint32_t);
                }
                else if (indexAccessor.component_type == TG3_COMPONENT_TYPE_UNSIGNED_BYTE) {
                    indexStride = sizeof(uint8_t);
                }
                else {
                    throw std::runtime_error("Unsupported index component type");
                }
                
                indices.reserve(indices.size() + indexCount);

                for (size_t i = 0; i < indexCount; i++) {
                    uint32_t index = 0;

                    if (indexAccessor.component_type == TG3_COMPONENT_TYPE_UNSIGNED_SHORT)
                    {
                        index = *reinterpret_cast<const uint16_t *>(indexData + i * indexStride);
                    }
                    else if (indexAccessor.component_type == TG3_COMPONENT_TYPE_UNSIGNED_INT)
                    {
                        index = *reinterpret_cast<const uint32_t *>(indexData + i * indexStride);
                    }
                    else if (indexAccessor.component_type == TG3_COMPONENT_TYPE_UNSIGNED_BYTE)
                    {
                        index = *reinterpret_cast<const uint8_t *>(indexData + i * indexStride);
                    }

                    indices.push_back(baseVertex + index);
                }
            }

        }

        tg3_model_free(&model);
        tg3_error_stack_free(&errors);
    }

    static void LoadModelOBJ(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, const std::string& modelPath = MODEL_PATH) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str())) {
            throw std::runtime_error(warn + err);
        }

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex{};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = {1.0f, 1.0f, 1.0f};

                vertices.push_back(vertex);
                indices.push_back(indices.size());
            }
        }
    }

    static void LoadModel(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, const std::string& modelPath = MODEL_PATH) {
        std::filesystem::path filePath = static_cast<std::filesystem::path>(modelPath);
        auto extension = filePath.extension();
        if (extension == ".obj") {
            LoadModelOBJ(vertices, indices, modelPath);
        }
        else if (extension == ".gltf") {
            LoadModelGLTF(vertices, indices, modelPath);
        }
    }

    inline uint32_t FindModelAccessorIndex(const tg3_model& model, const tg3_primitive& primitive, const char* attributeName) {
        uint32_t accessorIndex = -1;
        for (uint32_t k = 0; k < model.accessors_count; k++)
        {
            const auto accessor = model.accessors[k];
            if (tg3_str_equals(accessor.name, primitive.attributes[FindAttributeIndex(primitive, attributeName)].key) == 0) {
                accessorIndex = k;
                break;
            }
        }

        return accessorIndex;
    }

}
}