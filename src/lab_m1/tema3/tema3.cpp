#include "lab_m1/tema3/tema3.h"
#include "lab_m1/tema3/transform3D.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    GetSceneCamera()->SetPosition(glm::vec3(init_X, init_Y, init_Z));

    // Load textures
    {
        const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "textures");

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "grass_bilboard.png").c_str(), GL_REPEAT);
            mapTextures["grass"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "crate.jpg").c_str(), GL_REPEAT);
            mapTextures["crate"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "earth.png").c_str(), GL_REPEAT);
            mapTextures["earth"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "snow.jpg").c_str(), GL_REPEAT);
            mapTextures["snow"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "gift_box.jpg").c_str(), GL_REPEAT);
            mapTextures["gift_box"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "tree.jpg").c_str(), GL_REPEAT);
            mapTextures["tree"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "stem.jfif").c_str(), GL_REPEAT);
            mapTextures["stem"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "christmas_lights.jpg").c_str(), GL_REPEAT);
            mapTextures["christmas_lights"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "star.jfif").c_str(), GL_REPEAT);
            mapTextures["star"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "rock.jfif").c_str(), GL_REPEAT);
            mapTextures["rock"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "moss_tree.jfif").c_str(), GL_REPEAT);
            mapTextures["moss_tree"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "rock_moss.jpg").c_str(), GL_REPEAT);
            mapTextures["rock_moss"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "pole.jpg").c_str(), GL_REPEAT);
            mapTextures["pole"] = texture;
        }
    }

    // Load meshes
    {
        {
            Mesh* mesh = new Mesh("box");
            mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
            meshes[mesh->GetMeshID()] = mesh;
        }

        {
            Mesh* mesh = new Mesh("sphere");
            mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
            meshes[mesh->GetMeshID()] = mesh;
        }

        // Quad
        {
            vector<glm::vec3> vertices
            {
                glm::vec3(50.f,  0.f, 50.f),
                glm::vec3(50.f,  0.f, -50.f),
                glm::vec3(-50.f, 0.f, -50.f),
                glm::vec3(-50.f, 0.f, 50.f),
            };

            vector<glm::vec3> normals
            {
                glm::vec3(0, 1, 1),
                glm::vec3(1, 0, 1),
                glm::vec3(1, 0, 0),
                glm::vec3(0, 1, 0)
            };

            vector<glm::vec2> textureCoords
            {
                glm::vec2(0.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(1.0f, 0.0f)

            };

            vector<unsigned int> indices =
            {
                0, 1, 3,
                1, 2, 3
            };

            Mesh* mesh = new Mesh("square");
            mesh->InitFromData(vertices, normals, textureCoords, indices);
            meshes[mesh->GetMeshID()] = mesh;
        }

        // Cone
        {
            Mesh* mesh = new Mesh("cone");
            mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");
            meshes[mesh->GetMeshID()] = mesh;
        }
    }

    // Light shader
    {
        Shader* shader = new Shader("LightShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Texture shader
    {
        Shader* shader = new Shader("ShaderTexture");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShaderTexture.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShaderTexture.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    ObstaclesGenerator();
}


void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::Update(float deltaTimeSeconds)
{
    if (!end_game) {
        diff = move * deltaTimeSeconds;
        tY_player -= diff;
        tZ_player += diff;

        camera_Y -= diff;
        camera_Z += diff;

        if (move_left && !move_right) {
            tX_player -= diff;
            camera_X -= diff;
        }
        
        if (!move_left && move_right) {
            tX_player += diff;
            camera_X += diff;
        }

        GetSceneCamera()->SetPosition(glm::vec3(camera_X, camera_Y, camera_Z));
    }

    ObstaclesGenerator();
    ObstaclesUpdate();
    CheckCollision();

    // Field
    RenderField(tX_player, tY_player, tZ_player);

    // Player
    RenderPlayer(tX_player, tY_player, tZ_player);

    // Objects
    RenderObjects();
}


void Tema3::FrameEnd()
{
    //DrawCoordinateSystem();
}


void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    time = Engine::GetElapsedTime();
    // Set any other shader uniforms that you need
    glUniform1i(glGetUniformLocation(shader->program, "mixBool"), mixTexture);
    glUniform1f(glGetUniformLocation(shader->program, "time"), time / 200);
    glUniform1i(glGetUniformLocation(shader->program, "is_field"), is_field);

    if (texture1)
    {
        // - activate texture location 0
        glActiveTexture(GL_TEXTURE0);

        // - bind the texture1 ID
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());

        // - send theuniform value
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

    }

    if (texture2)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);

    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema3::RenderSimpleMeshLight(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light properties
    int light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema3::RenderField(float x, float y, float z) {
    
    if (!end_game)
        is_field = 1;
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y - 0.15, z));
        modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
        RenderSimpleMesh(meshes["square"], shaders["ShaderTexture"], modelMatrix, mapTextures["snow"]);
    }
    is_field = 0;
}


void Tema3::RenderTree(float x, float y, float z) {
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y + tree3_offset, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.2, 0.2));
        RenderSimpleMesh(meshes["cone"], shaders["ShaderTexture"], modelMatrix, mapTextures["star"]);
    }

    mixTexture = 1;
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y + tree2_offset, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.4, 0.3));
        RenderSimpleMesh(meshes["cone"], shaders["ShaderTexture"], modelMatrix, mapTextures["tree"], mapTextures["christmas_lights"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y + tree1_offset, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4, 0.4, 0.4));
        RenderSimpleMesh(meshes["cone"], shaders["ShaderTexture"], modelMatrix, mapTextures["tree"], mapTextures["christmas_lights"]);
    }
    mixTexture = 0;

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.6, 0.2));
        RenderSimpleMesh(meshes["box"], shaders["ShaderTexture"], modelMatrix, mapTextures["stem"]);
    }
}


void Tema3::RenderRock(float x, float y, float z) {
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x + rock2_offset, y + rock2_offset_y, z));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(30.f), glm::vec3(0, 0, 1));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.1, 0.2));
        RenderSimpleMesh(meshes["sphere"], shaders["ShaderTexture"], modelMatrix, mapTextures["rock"]);
    }

    mixTexture = 1;
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x + rock1_offset, y, z));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.f), glm::vec3(0, 0, 1));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.1, 0.2));
        RenderSimpleMesh(meshes["sphere"], shaders["ShaderTexture"], modelMatrix, mapTextures["rock"], mapTextures["moss_tree"]);
    }
    mixTexture = 0;

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.1, 0.3));
        RenderSimpleMesh(meshes["sphere"], shaders["ShaderTexture"], modelMatrix, mapTextures["rock_moss"]);
    }
}


void Tema3::RenderGift(float x, float y, float z) {
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.2, 0.2));
        RenderSimpleMesh(meshes["box"], shaders["ShaderTexture"], modelMatrix, mapTextures["gift_box"]);
    }
}


void Tema3::RenderPole(float x, float y, float z) {
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 1.5, 0.1));
        RenderSimpleMesh(meshes["box"], shaders["ShaderTexture"], modelMatrix, mapTextures["pole"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y + pole_offset, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.1, 0.1));
        RenderSimpleMesh(meshes["box"], shaders["ShaderTexture"], modelMatrix, mapTextures["pole"]);
    }
}


void Tema3::RenderPlayer(float x, float y, float z) {
    if (!move_left && !move_right) {
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y + 0.18, z));
            modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.4, 0.3));
            RenderSimpleMeshLight(meshes["box"], shaders["LightShader"], modelMatrix, glm::vec3(0, 0, 1));
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(x - 0.1, y - 0.04, z - 0.1));
            modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.1, 0.5));
            RenderSimpleMeshLight(meshes["box"], shaders["LightShader"], modelMatrix, glm::vec3(0, 1, 0));
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(x + 0.1, y - 0.04, z - 0.1));
            modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.1, 0.5));
            RenderSimpleMeshLight(meshes["box"], shaders["LightShader"], modelMatrix, glm::vec3(0, 1, 0));
        }
    }
    else if (!move_left && move_right) {
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y + 0.18, z));
            modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.4, 0.3));
            RenderSimpleMeshLight(meshes["box"], shaders["LightShader"], modelMatrix, glm::vec3(0, 0, 1));
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(x - 0.1, y - 0.04, z - 0.1));
            modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.1, 0.5));
            RenderSimpleMeshLight(meshes["box"], shaders["LightShader"], modelMatrix, glm::vec3(0, 1, 0));
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(x + 0.1, y - 0.04, z - 0.1));
            modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.1, 0.5));
            RenderSimpleMeshLight(meshes["box"], shaders["LightShader"], modelMatrix, glm::vec3(0, 1, 0));
        }
    }
    else if (move_left && !move_right) {
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y + 0.18, z));
            modelMatrix = glm::rotate(modelMatrix, -field_angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.4, 0.3));
            RenderSimpleMeshLight(meshes["box"], shaders["LightShader"], modelMatrix, glm::vec3(0, 0, 1));
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(x - 0.1, y - 0.04, z - 0.1));
            modelMatrix = glm::rotate(modelMatrix, -field_angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.1, 0.5));
            RenderSimpleMeshLight(meshes["box"], shaders["LightShader"], modelMatrix, glm::vec3(0, 1, 0));
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(x + 0.1, y - 0.04, z - 0.1));
            modelMatrix = glm::rotate(modelMatrix, -field_angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.1, 0.5));
            RenderSimpleMeshLight(meshes["box"], shaders["LightShader"], modelMatrix, glm::vec3(0, 1, 0));
        }
    }
}


void Tema3::RenderObjects() {
    for (int i = 0; i < obstacles_model.size(); i++) {
        
        int model = obstacles_model[i];
        glm::vec3 position = obstacles_positions[i];

        // 1 - tree, 2 - pole, 3 - rock, 4 - gift
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, field_angle, glm::vec3(1, 0, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(position[0], position[1], position[2]));

        obstacles_col[i][0] = modelMatrix[3][0];
        obstacles_col[i][1] = modelMatrix[3][1];
        obstacles_col[i][2] = modelMatrix[3][2];

        switch (model)
        {
        case 1:
            RenderTree(position[0], position[1], position[2]);
            break;
        case 2:
            RenderPole(position[0], position[1], position[2]);
            break;
        case 3:
            RenderRock(position[0], position[1], position[2]);
            break;
        case 4:
            RenderGift(position[0], position[1], position[2]);
            break;
        default:
            break;
        }

    }
}


bool Tema3::ExistingCoordinates(float x, float z) {
    for (int i = 0; i < obstacles_positions.size(); i++) {
        if (obstacles_positions[i][0] == x && obstacles_positions[i][2] == z)
            return true;
    }
    return false;
}


void Tema3::ObstaclesUpdate() {
    for (int i = 0; i < obstacles_col.size(); i++) {
        if (tZ_player - 3 > obstacles_col[i][2]) {
            obstacles_positions.erase(obstacles_positions.begin() + i);
            obstacles_model.erase(obstacles_model.begin() + i);
            obstacles_col.erase(obstacles_col.begin() + i);
        }
    }
}


void Tema3::ObstaclesGenerator() {
    if (obstacles_model.size() < max_obstacles) {
        int free_space = max_obstacles - obstacles_model.size();
        for (int i = 1; i <= free_space; i++) {
            // 1 - tree, 2 - pole, 3 - rock, 4 - gift
            int model = rand() % 4 + 1;

            int Min_X = tX_player - 20;
            int Max_X = tX_player + 20;

            int Min_Z = tZ_player + 20;
            int Max_Z = tZ_player + 50;

            float randX;
            float randY = 0;
            float randZ;
            do {
                randX = rand() % (Max_X + 1 - Min_X) + Min_X;
                randZ = rand() % (Max_Z + 1 - Min_Z) + Min_Z;
            } while (ExistingCoordinates(randX, randZ));

            obstacles_model.push_back(model);
            obstacles_positions.push_back(glm::vec3(randX, randY, randZ));
            obstacles_col.push_back(glm::vec3(randX, randY, randZ));
        }
    }
}


bool Tema3::SphereCollision(glm::vec4 player_sphere, glm::vec4 obstacle_sphere) {
    float Ax = player_sphere[0];
    float Az = player_sphere[2];
    float Aradius = player_sphere[3];

    float Bx = obstacle_sphere[0];
    float Bz = obstacle_sphere[2];
    float Bradius = obstacle_sphere[3];

    float diff1 = (Ax - Bx) * (Ax - Bx);
    float diff3 = (Az - Bz) * (Az - Bz);
    float diff = sqrt(diff1 + diff3);

    if (diff <= (Aradius + Bradius))
        return true;
    return false;
}


void Tema3::CheckCollision() {
    glm::vec4 player_sphere(tX_player, tY_player, tZ_player, 0.2);

    for (int i = 0; i < obstacles_positions.size(); i++) {
        glm::vec4 obstacle_sphere(obstacles_col[i][0],
                                  obstacles_col[i][1],
                                  obstacles_col[i][2], 0.1);

        if (SphereCollision(player_sphere, obstacle_sphere) == true)
            if (obstacles_model[i] == 1) {
                if (!end_game) {
                    cout << "A atins un brad!" << endl;
                    cout << "Ai adunat " << gifts_count << " cadouri!" << endl;
                    end_game = true;
                }
            }
            else if (obstacles_model[i] == 2) {
                if (!end_game) {
                    cout << "A atins un stalp!" << endl;
                    cout << "Ai adunat " << gifts_count << " cadouri!" << endl;
                    end_game = true;
                }
            }
            else if (obstacles_model[i] == 3) {
                if (!end_game) {
                    cout << "A atins 0 piatra!" << endl;
                    cout << "Ai adunat " << gifts_count << " cadouri!" << endl;
                    end_game = true;
                }
            }
            else {
                obstacles_model.erase(obstacles_model.begin() + i);
                obstacles_positions.erase(obstacles_positions.begin() + i);
                obstacles_col.erase(obstacles_col.begin() + i);
                gifts_count++;
            }
    }
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{

}


void Tema3::OnKeyPress(int key, int mods)
{
    if (end_game && key == GLFW_KEY_R) {
        end_game = false;
        tX_player = 0;
        tY_player = 0;
        tZ_player = 0;

        obstacles_positions.clear();
        obstacles_model.clear();
        obstacles_col.clear();

        diff = 0;
        gifts_count = 0;

        camera_X = init_X;
        camera_Y = init_Y;
        camera_Z = init_Z;

        cout << endl;
    }
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

    if (!end_game) {
        glm::vec2 resolution = window->GetResolution();

        rotate_angle = atan2(mouseX - resolution[0], resolution[1] - mouseY);

        if (mouseX < resolution[0] / 2) {
            move_right = false;
            move_left = true;
        }
        else if (mouseX > resolution[0] / 2) {
            move_right = true;
            move_left = false;
        }
        else {
            move_right = false;
            move_left = false;
        }
    }
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
