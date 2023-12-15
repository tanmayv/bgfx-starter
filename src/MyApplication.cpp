#include <stdio.h>
#include <iostream>
#include "bgfx-application.hpp"
#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>
#include <bx/bx.h>

#include <vs.sc.glsl.bin.h>
#include <vs.sc.essl.bin.h>
#include <vs.sc.spv.bin.h>
#include <fs.sc.glsl.bin.h>
#include <fs.sc.essl.bin.h>
#include <fs.sc.spv.bin.h>
#if defined(_WIN32)
#include <vs.sc.dx11.bin.h>
#include <fs.sc.dx11.bin.h>
#endif //  defined(_WIN32)
#if __APPLE__
#include <vs.sc.mtl.bin.h>
#include <fs.sc.mtl.bin.h>
#endif // __APPLE__

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static const bgfx::EmbeddedShader kEmbeddedShaders[] =
        {
                BGFX_EMBEDDED_SHADER(vs),
                BGFX_EMBEDDED_SHADER(fs),
                BGFX_EMBEDDED_SHADER_END()
        };

// Callback function to handle GLFW errors
void glfwErrorCallback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    std::cout << "GLFW Error: " << description;
}

// Vertex data for a colored triangle
const float triangleVertices[] = {
     -1.0f, -1.0f, 0.0f,  0.0f, 1.0f, // Vertex 0: position and texture coordinate
     1.0f, -1.0f, 0.0f,  1.0f, 1.0f, // Vertex 1: position and texture coordinate
     0.0f,  1.0f, 0.0f,  0.5f, 0.0f, // Vertex 2: position and texture coordinate
};
// const float triangleVertices[] = {
//        -1.0f, -1.0f, 0.0f,  // Vertex 0: position
//      1.0f,  0.0f, 0.0f,  // Vertex 0: color (red)

//      1.0f, -1.0f, 0.0f,  // Vertex 1: position
//      0.0f,  1.0f, 0.0f,  // Vertex 1: color (green)

//      0.0f,  1.0f, 0.0f,  // Vertex 2: position
//      0.0f,  0.0f, 1.0f,  // Vertex 2: color (blue)
// };


// Index data for a single triangle
const uint16_t triangleIndices[] = {
    0, 1, 2};

class Application : public BgfxApplication
{
public:
    Application() : BgfxApplication({"Tanmay", {500, 500}}) {}
    void OnInit() override
    {
        int texWidth, texHeight, texNChannels;
        auto* textureData = stbi_load("assets/textures/test.png", &texWidth, &texHeight, &texNChannels, STBI_rgb_alpha);
        if (!textureData) {
            std::cout << "Texture data is null" << texWidth << texHeight << std::endl;
            return;
        } else {
            std::cout << "Texture data is not null" << texWidth << texHeight << std::endl;
        }
        std::cout << "textureColorUniform Start" << std::endl;
        textureColorUniform  = bgfx::createUniform("s_texColor",  bgfx::UniformType::Sampler);
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
        program = bgfx::createProgram(
            bgfx::createEmbeddedShader(kEmbeddedShaders, bgfx::getRendererType(), "vs"),
            bgfx::createEmbeddedShader(kEmbeddedShaders, bgfx::getRendererType(), "fs"),
            true);

        // stbi_image_free(textureData);
    }

    void OnUpdate() override
    {
        // Set vertex and index buffers
        bgfx::setState(
        BGFX_STATE_WRITE_R
                | BGFX_STATE_WRITE_G
                | BGFX_STATE_WRITE_B
                | BGFX_STATE_WRITE_A
        );
        bgfx::setVertexBuffer(0, vertexBuffer);
        bgfx::setIndexBuffer(indexBuffer);
        bgfx::setTexture(0, textureColorUniform, textureHandle);

        // Set transform state (not shown in this example)
        // bgfx::setDebug(BGFX_DEBUG_TEXT);
        // bgfx::dbgTextPrintf(0, 1, 0x0f, "Hello World");
        // Enable debug text.

        // Submit the draw call
        bgfx::submit(0, program);
    }

    void OnExit() override
    {
    }
    private:
    bgfx::VertexBufferHandle vertexBuffer;
    bgfx::IndexBufferHandle indexBuffer;
    bgfx::TextureHandle textureHandle;
    bgfx::UniformHandle textureColorUniform;
    bgfx::ProgramHandle program;

};

int main()
{
    Application app;
    app.Bootstrap();
    return 0;
    //     return 0;
}
