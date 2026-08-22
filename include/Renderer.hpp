#pragma once

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <optional>
#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <fstream>
#include <array>

#include "Common.h"
#include "Vertex.h"
#include "Camera.hpp"
#include "Scene.hpp"

namespace KQ {
    const std::string TEXTURE_PATH = "assets/models/viking_room.png";
    
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    const int MAX_OBJECTS = 3;
    const int MAX_FRAMES_IN_FLIGHT = 2;

    #ifdef NDEBUG
        const bool enableValidationLayers = false;
    #else
        const bool enableValidationLayers = true;
    #endif

    struct TextureResource {
        VkImage image;
        VkDeviceMemory memory;
        VkImageView view;
        VkSampler sampler;
    };

    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };
    
    class Renderer {
    public:
        Renderer() = default;

        ~Renderer() {}

        void Init(GLFWwindow* window);
        void LoadScene(KQ::Scene& scene);
        void DrawFrame(const KQ::Camera& camera, KQ::Scene& scene);
        void Cleanup(KQ::Scene& scene);

        inline VkDevice* GetDevice() { return &device; }

    private:
        VkInstance m_Instance;
        GLFWwindow* m_Window;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR surface;
        VkQueue presentQueue;
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        VkRenderPass renderPass;
        VkDescriptorSetLayout descriptorSetLayout;
        VkPipelineLayout pipelineLayout;
        VkPipelineLayout gridPipelineLayout;
        VkPipeline graphicsPipeline;
        VkPipeline gridPipeline;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        bool framebufferResized = false;
        std::vector<KQ::Vertex> m_CombinedVertices;
        std::vector<std::uint32_t> m_CombinedIndices;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        VkDescriptorPool descriptorPool;
        // std::vector<VkDescriptorSet> descriptorSets;

        // std::vector<VkBuffer> uniformBuffers;
        // std::vector<VkDeviceMemory> uniformBuffersMemory;
        // std::vector<void*> uniformBuffersMapped;

        // VkImage textureImage;
        // VkDeviceMemory textureImageMemory;
        // VkImageView textureImageView;
        // VkSampler textureSampler;

        std::unordered_map<std::string, TextureResource> textureCache;

        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;

        std::uint32_t m_CurrentFrame = 0;

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        
        struct QueueFamilyIndices {
            std::optional<std::uint32_t> graphicsFamily;
            std::optional<std::uint32_t> presentFamily;

            bool isComplete() {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
        };

        struct SwapChainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        VkDevice device;
        VkQueue graphicsQueue;

        void CreateInstance();
        bool CheckValidationLayerSupport();
        std::vector<const char*> GetRequiredExtensions();
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        void SetupDebugMessenger();
        void CreateSurface(GLFWwindow* window);
        void PickPhysicalDevice();
        bool IsDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
        void CreateLogicalDevice();
        void CreateSwapChain();
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        void CreateImageViews();
        VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        void CreateRenderPass();
        VkFormat FindDepthFormat();
        VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        void CreateDescriptorSetLayout();
        void CreateGraphicsPipeline();
        void CreateGridPipeline();
        VkShaderModule CreateShaderModule(const std::vector<char>& code);
        void CreateCommandPool();
        void CreateDepthResources();
        void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void CreateFrameBuffers();
        TextureResource& GetOrCreateTexture(const std::string& path);
        void CreateTextureImage(const std::string& texturePath, TextureResource& outTexture);
        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        VkCommandBuffer BeginSingleTimeCommands();
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
        void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        void CreateTextureImageView(TextureResource& outTexture);
        void CreateTextureSampler(TextureResource& outTexture);
        void LoadModelsFromScene(KQ::Scene& scene);
        void CreateVertexBuffer();
        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void CreateIndexBuffer();
        void CreateUniformBuffers(KQ::Scene& scene);
        void CreateDescriptorPools();
        void CreateDescriptorSets(KQ::Scene& scene);
        void CreateCommandBuffers();
        void CreateSyncObjects();
        void RecreateSwapChain();
        void CleanupSwapChain();
        void UpdateUniformBuffers(uint32_t currentImage, const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraUp, const KQ::Scene& scene);
        void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, KQ::Scene& scene);
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
    };
}