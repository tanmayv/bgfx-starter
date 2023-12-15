#include "bgfx-application.hpp"

#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#endif //  defined(_WIN32)
#if defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include <GLFW/glfw3.h>
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3native.h>


void BgfxApplication::Bootstrap() {
    if (!glfwInit()) {
        // TODO: Error handling
        return;
    }

    // Configure GLFW to use the core profile and enable forward compatibility
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(opts_.window_dimensions.x, opts_.window_dimensions.y, opts_.window_name.c_str(), NULL, NULL);
    if (!window)
    {
        // TODO: fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return ;
    }
    glfwMakeContextCurrent(window);
    bgfx::renderFrame();
    // Initialize bgfx using the native window handle and window resolution.
    bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    init.platformData.ndt = glfwGetX11Display();
    init.platformData.nwh = (void *)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
    init.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
    init.platformData.nwh = glfwGetWin32Window(window);
#endif
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    init.resolution.width = (uint32_t)width;
    init.resolution.height = (uint32_t)height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    if (!bgfx::init(init))
        // TODO: Error handling
        return;
    // Set view 0 to the same dimensions as the window and to clear the color buffer.
    const bgfx::ViewId kClearView = 0;
    bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
    bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

    OnInit();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        OnUpdate();
        bgfx::frame();
    }
    OnExit();

    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

}