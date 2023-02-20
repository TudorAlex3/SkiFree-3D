#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/tema3/camera.h"

#include <string>
#include <unordered_map>
#include <list>
#include <utility>

namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL);
        void Tema3::RenderSimpleMeshLight(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);
        void Tema3::RenderField(float x, float y, float z);
        void Tema3::RenderTree(float x, float y, float z);
        void Tema3::RenderRock(float x, float y, float z);
        void Tema3::RenderGift(float x, float y, float z);
        void Tema3::RenderPole(float x, float y, float z);
        void Tema3::RenderPlayer(float x, float y, float z);
        void Tema3::RenderObjects();
        bool Tema3::ExistingCoordinates(float x, float z);
        void Tema3::ObstaclesGenerator();
        void Tema3::ObstaclesUpdate();
        bool Tema3::SphereCollision(glm::vec4 player_sphere, glm::vec4 obstacle_sphere);
        void Tema3::CheckCollision();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        GLuint randomID;
        int mix = 0;

        std::unordered_map<std::string, Texture2D*> mapTextures;

        float field_angle = RADIANS(45);
        int mixTexture = 0;

        // obstacles
        float tree1_offset = 0.6;
        float tree2_offset = 1;
        float tree3_offset = 1.2;
        float rock1_offset = -0.2;
        float rock2_offset = -0.05;
        float rock2_offset_y = 0.05;
        float pole_offset = 0.8;

        // player
        float tX_player = 0;
        float tY_player = 0;
        float tZ_player = 0;
        float rotate_angle = 0;
        bool move_left = false;
        bool move_right = false;

        // light
        glm::vec3 lightPosition = glm::vec3(0, 1, 1);
        glm::vec3 lightDirection = glm::vec3(0, -1, 0);
        unsigned int materialShininess = 30;
        float materialKd = 2;
        float materialKs = 0.5;
        int is_spot_light = 0;

        // field
        float field_y = 0;
        float field_z = 0;
        float move = 0.5;
        float diff;
        int is_field = 0;
        float time;

        // obstacle lists
        std::vector<glm::vec3> obstacles_col;
        std::vector<glm::vec3> obstacles_positions;
        std::vector<int> obstacles_model;
        int max_obstacles = 200;

        // gameplay
        bool end_game = false;
        int gifts_count = 0;

        // camera
        float init_X = 0;
        float init_Y = 1;
        float init_Z = 5;
        float camera_X = init_X;
        float camera_Y = init_Y;
        float camera_Z = init_Z;
    };
}   // namespace m1
