#pragma once

#include "components/simple_scene.h"


#define MOVE_RATIO (.25f)
#define LIMIT (.5f)
#define SQUARE_SIDE (100.f)
#define SQUARES_DISTANCE (100.f)
#define SCALE_FACTOR_X (1.5f)
#define SCALE_FACTOR_Y (2.f)

namespace m1
{
    class Lab3 : public gfxc::SimpleScene
    {
    public:
        Lab3();
        ~Lab3();

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

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;

        // TODO(student): If you need any other class variables, define them here.
        GLfloat posSunX, posSunY;
        GLboolean moveRight;
    };
}   // namespace m1
