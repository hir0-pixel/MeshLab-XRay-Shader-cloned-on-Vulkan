#pragma once

#include <vector>
#include <optional>

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VulkanVertex.h"

struct GLFWwindow;

class VulkanApp {
public:
    VulkanApp(const std::vector<VulkanVertex>& vertices,
              const std::vector<uint32_t>& indices);

    void run();
    void onScroll(double xoffset, double yoffset);

    struct PushConsts {
        glm::mat4 mvp;
        glm::mat4 mv;
    };

private:
    // mesh data
    std::vector<VulkanVertex> m_vertices;
    std::vector<uint32_t>     m_indices;
    uint32_t                  m_indexCount = 0;

    // window
    GLFWwindow*   m_window = nullptr;
    const int     WIDTH    = 1280;
    const int     HEIGHT   = 720;

    // vulkan core
    VkInstance       m_instance       = VK_NULL_HANDLE;
    VkSurfaceKHR     m_surface        = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice         m_device         = VK_NULL_HANDLE;
    VkQueue          m_graphicsQueue  = VK_NULL_HANDLE;
    VkQueue          m_presentQueue   = VK_NULL_HANDLE;

    // swapchain
    VkSwapchainKHR              m_swapchain = VK_NULL_HANDLE;
    std::vector<VkImage>        m_swapchainImages;
    VkFormat                    m_swapchainImageFormat{};
    VkExtent2D                  m_swapchainExtent{};
    std::vector<VkImageView>    m_swapchainImageViews;
    std::vector<VkFramebuffer>  m_swapchainFramebuffers;

    // pipeline / renderpass
    VkRenderPass     m_renderPass       = VK_NULL_HANDLE;
    VkPipelineLayout m_pipelineLayout   = VK_NULL_HANDLE;
    VkPipeline       m_graphicsPipeline = VK_NULL_HANDLE;

    // commands
    VkCommandPool                m_commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> m_commandBuffers;

    // sync
    static const int MAX_FRAMES_IN_FLIGHT = 2;
    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence>     m_inFlightFences;
    size_t                   m_currentFrame = 0;

    // vertex / index buffers
    VkBuffer       m_vertexBuffer       = VK_NULL_HANDLE;
    VkDeviceMemory m_vertexBufferMemory = VK_NULL_HANDLE;
    VkBuffer       m_indexBuffer        = VK_NULL_HANDLE;
    VkDeviceMemory m_indexBufferMemory  = VK_NULL_HANDLE;

    // simple orbit camera state
    float  m_yaw      = 0.0f;
    float  m_pitch    = 0.4f;
    float  m_distance = 3.0f;

    bool   m_mousePressed = false;
    double m_lastMouseX   = 0.0;
    double m_lastMouseY   = 0.0;

    // high-level flow
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    // camera
    void updateCameraFromInput();

    // vulkan setup
    void createInstance();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapchain();
    void createImageViews();
    void createRenderPass();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createVertexBuffer();
    void createIndexBuffer();
    void createCommandBuffers();
    void createSyncObjects();

    void drawFrame();

    // helpers
    VkShaderModule createShaderModule(const std::vector<char>& code);
    static std::vector<char> readFile(const std::string& filename);

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR        capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   presentModes;
    };

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR      chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
    VkPresentModeKHR        chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& modes);
    VkExtent2D              chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createBuffer(VkDeviceSize size,
                      VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties,
                      VkBuffer& buffer,
                      VkDeviceMemory& bufferMemory);
    VkCommandBuffer beginSingleTimeCommands();
    void            endSingleTimeCommands(VkCommandBuffer cmd);
};
