#include <stdio.h>
#include <iostream>
#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>
#include <bx/bx.h>

#include <vs.sc.glsl.bin.h>
#include <vs.sc.essl.bin.h>
#include <vs.sc.spv.bin.h>
#include <fs.sc.glsl.bin.h>
#include <fs.sc.essl.bin.h>
#include <fs.sc.spv.bin.h>
#if defined(__linux__)
#include <vs.sc.dx11.bin.h>
#include <fs.sc.dx11.bin.h>
#endif //  defined(__linux__)
#if defined(_WIN32)
#include <vs.sc.dx11.bin.h>
#include <fs.sc.dx11.bin.h>
#endif //  defined(_WIN32)
#if __APPLE__
// #include <vs.sc.mtl.bin.h>
// #include <fs.sc.mtl.bin.h>
#endif // __APPLE__

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "bgfx-application.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "mesh.hpp"
#include "camera.h"


static const bgfx::EmbeddedShader kEmbeddedShaders[] =
    {
        BGFX_EMBEDDED_SHADER(vs),
        BGFX_EMBEDDED_SHADER(fs),
        BGFX_EMBEDDED_SHADER_END()};

// Callback function to handle GLFW errors
void glfwErrorCallback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    std::cout << "GLFW Error: " << description;
}


class Application : public BgfxApplication
{
public:
    Application() : BgfxApplication({"Tanmay", {500, 500}}) {}
    void OnInit() override
    {
        int texWidth, texHeight, texNChannels;
        auto *textureData = stbi_load("assets/textures/test.png", &texWidth, &texHeight, &texNChannels, STBI_rgb_alpha);
        if (!textureData)
        {
            std::cout << "Texture data is null" << texWidth << texHeight << std::endl;
            return;
        }
        else
        {
            std::cout << "Texture data is not null" << texWidth << texHeight << std::endl;
        }
        camera = Camera{glm::vec3(0.0f, 0.0f, 3.0f)};
        std::cout << "textureColorUniform Start" << std::endl;
        textureColorUniform = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
        transformUniform = bgfx::createUniform("u_transform", bgfx::UniformType::Mat4);
        modelUniform = bgfx::createUniform("u_model1", bgfx::UniformType::Mat4);
        projectionUniform = bgfx::createUniform("u_projection1", bgfx::UniformType::Mat4);
        std::cout << "textureColorUniform created" << std::endl;
        textureHandle = bgfx::createTexture2D(texWidth, texHeight, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_NONE, bgfx::copy(textureData, texWidth * texHeight * 4));
        std::cout << "textureColorHandle created" << std::endl;
        // Create vertex buffer for a triangle
        bgfx::VertexLayout vertexLayout;
        vertexLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();

        vertexBuffer = bgfx::createVertexBuffer(
            bgfx::makeRef(triangleVertices, sizeof(triangleVertices)),
            vertexLayout);

        // Create index buffer for a triangle
        indexBuffer = bgfx::createIndexBuffer(
            bgfx::makeRef(triangleIndices, sizeof(triangleIndices)));

        // Create a simple program

        std::cout << bgfx::getRendererType() << std::endl;
        program = bgfx::createProgram(
            bgfx::createEmbeddedShader(kEmbeddedShaders, bgfx::getRendererType(), "vs"),
            bgfx::createEmbeddedShader(kEmbeddedShaders, bgfx::getRendererType(), "fs"),
            true);

        stbi_image_free(textureData);

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
    }

    void OnUpdate() override
    {
        // Set vertex and index buffers
        bgfx::setState(
            BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |  BGFX_STATE_MSAA | BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G | BGFX_STATE_WRITE_B | BGFX_STATE_WRITE_A);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, GetTime() * glm::radians(50.0f),
                            glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 view = glm::mat4(1.0f);
        // note that we’re translating the scene in the reverse direction
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f,
                                      100.0f);

        bgfx::setVertexBuffer(0, vertexBuffer);
        bgfx::setIndexBuffer(indexBuffer);
        bgfx::setTexture(0, textureColorUniform, textureHandle);
        trans = glm::mat4(1.0f);
        // trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
        // trans = glm::rotate(trans, 0, glm::vec3(0.0, 0.0, 1.0));
        bgfx::setUniform(transformUniform, glm::value_ptr(trans));
        bgfx::setUniform(modelUniform, glm::value_ptr(model));
        bgfx::setUniform(projectionUniform, glm::value_ptr(projection));

        // Set transform state (not shown in this example)
        // bgfx::setDebug(BGFX_DEBUG_TEXT);
        // bgfx::dbgTextPrintf(0, 1, 0x0f, "Hello World");
        // Enable debug text.

        // Submit the draw call
        bgfx::submit(0, program);
    }

    void OnExit() override
    {
        bgfx::destroy(program);
    }

private:
    bgfx::VertexBufferHandle vertexBuffer;
    bgfx::IndexBufferHandle indexBuffer;
    bgfx::TextureHandle textureHandle;
    bgfx::UniformHandle textureColorUniform;
    bgfx::UniformHandle transformUniform;
    bgfx::UniformHandle modelUniform;
    bgfx::UniformHandle projectionUniform;
    bgfx::ProgramHandle program;
    glm::mat4 trans;
    Camera camera;
};

int main()
{
    Application app;
    app.Bootstrap();
    return 0;
}
