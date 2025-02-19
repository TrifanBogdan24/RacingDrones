#pragma once

#include "components/simple_scene.h"

#define SPEED_FACTOR (2.f)

namespace m1
{
    class Lab1 : public gfxc::SimpleScene
    {
    public:
        Lab1();
        ~Lab1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        // Variables for background color
        GLfloat red, green, blue;
        GLuint colorCase;

        // Variables for object rendering
        std::string objectName;
        GLuint objectCase;
        glm::vec3 objScale;

        // Variables for object movement
        GLfloat posX, posY, posZ;
        GLfloat degrees;

        // Helper functions
        void changeClearColor();
        void changeObject();
    };
}   // namespace m1
