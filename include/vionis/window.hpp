#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "vionis/window_surface.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <vulkan/vulkan.h>

#include <glm/vec2.hpp>

namespace vionis
{

class Window
{
public:
    Window() = default;
    Window(int width, int height, std::string_view title);

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    Window(Window &&) noexcept = default;
    Window &operator=(Window &&) noexcept = default;

    ~Window();

    std::unique_ptr<vulkan::WindowSurface> createVulkanSurface(
        VkInstance instance, const VkAllocationCallbacks *allocationCallbacks = nullptr);

    void processEvents();
    void resetResizeFlag() { m_resized = false; }

    bool closeRequested() const { return m_closeRequested; }
    bool resized() const { return m_resized; }
    bool minimized() const { return m_minimized; }
    glm::vec2 size() const
    {
        int width = 0, height = 0;
        SDL_GetWindowSize(m_window.get(), &width, &height);
        return glm::vec2(width, height);
    }

    static std::vector<const char *> getRequiredInstanceExtensions()
    {
        uint32_t extensionCount = 0;
        char const *const *extensionNames = SDL_Vulkan_GetInstanceExtensions(&extensionCount);

        std::vector<const char *> extensions(extensionNames, extensionNames + extensionCount);

        return extensions;
    }

    SDL_Window *get() const { return m_window.get(); }

private:
    struct SDLWindowDeleter
    {
        void operator()(SDL_Window *window) const { SDL_DestroyWindow(window); }
    };

    std::unique_ptr<SDL_Window, SDLWindowDeleter> m_window;

    bool m_resized = false;
    bool m_minimized = false;
    bool m_closeRequested = false;
};

} // namespace vionis
