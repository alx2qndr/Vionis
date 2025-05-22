#include "vionis/window.hpp"

#include <stdexcept>

namespace vionis
{

Window::Window(int width, int height, std::string_view title)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_VIDEO))
    {
        throw std::runtime_error("Failed to initialize SDL. SDL error: " + std::string(SDL_GetError()));
    }

    if (!SDL_Vulkan_LoadLibrary(nullptr))
    {
        throw std::runtime_error("Failed to load Vulkan library. SDL error: " + std::string(SDL_GetError()));
    }

    m_window.reset(SDL_CreateWindow(title.data(), width, height,
                                    SDL_WINDOW_VULKAN | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE));
    if (!m_window)
    {
        throw std::runtime_error("Failed to create SDL window. SDL error: " + std::string(SDL_GetError()));
    }
}

Window::~Window()
{
    SDL_Vulkan_UnloadLibrary();
    SDL_Quit();
}

std::unique_ptr<vulkan::WindowSurface> Window::createVulkanSurface(VkInstance instance,
                                                                   const VkAllocationCallbacks *allocationCallbacks)
{
    return std::make_unique<vulkan::WindowSurface>(m_window.get(), instance, allocationCallbacks);
}

void Window::processEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            m_closeRequested = true;
            break;

        case SDL_EVENT_WINDOW_RESIZED:
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            m_resized = true;
            break;

        case SDL_EVENT_WINDOW_MINIMIZED:
            m_minimized = true;
            break;

        case SDL_EVENT_WINDOW_RESTORED:
            m_minimized = false;
            break;

        default:
            break;
        }
    }
}

} // namespace vionis
