#pragma once
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

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"

class BgfxApplication {
    public:
    struct Options {
        std::string window_name;        
        glm::vec2 window_dimensions;
    };

    BgfxApplication(Options opts): opts_(opts) {
    }

    void Bootstrap();
    virtual void OnInit() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnExit() = 0;
    float GetTime();
    void ProcessInput(GLFWwindow* window);


    private:
    Camera camera = Camera{glm::vec3{0.0f, 0.0f, 3.0f}};
    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;
    double lastX = 0.0f;
    double lastY = 0.0f;
    bgfx::UniformHandle viewUniform;
    Options opts_;
};