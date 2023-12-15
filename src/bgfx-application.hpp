#pragma once
#include <string>
#include <glm/glm.hpp>

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

    private:
    Options opts_;
};