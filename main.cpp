



#include "DogeEngine/Rendering/OpenGL/OpenGLApplication.hpp"
#include "DogeEngine/Rendering/Shaders/Materials/SimpleShadedMaterial.hpp"

#include "DogeEngine/IO/OBJMesh.hpp"
#include "DogeEngine/ECS/ECSManager.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"
#include "DogeEngine/ECS/Components/TransformComponent.hpp"
#include "DogeEngine/ECS/Components/HierarchyComponent.hpp"
#include "DogeEngine/Utils/GLMPrints.hpp"
#include "DogeEngine/ECS/Systems/PrintTransform.hpp"

using namespace Doge;

int main(int argc, char *argv[]){


    ECSManager manager;

    for (int i = 0; i < 1000000; ++i) {
        Entity dog = manager.createEntity();
        HierarchyComponent component_1;
        manager.addComponent(dog,component_1);
        Entity planet = manager.createEntity();
        TransformComponent component_2;
        component_2.pos = {-1.0,-1.0,-1.3};
        manager.addComponent(planet,component_2);
        TransformComponent component;
        component.pos = {21,21,21};
        manager.addComponent(dog,component);
    }
    std::cout << "Creation \n";

    PrintTransform printer;


    Entity dog = manager.createEntity();

    HierarchyComponent component_1;
    manager.addComponent(dog,component_1);
    Entity planet = manager.createEntity();
    TransformComponent component_2;
    component_2.pos = {700.0,5000.0,3000.3};
    manager.addComponent(planet,component_2);
    TransformComponent component;
    component.pos = {1.0,2.0,3.0};
    manager.addComponent(dog,component);


    printer.run(&manager);

    std::cout << manager.getComponent<TransformComponent>(planet)->pos << "\n";
    std::cout << manager.getComponent<HierarchyComponent>(dog)->child.getID() << "\n";
    std::cout << manager.getComponent<TransformComponent>(dog)->pos << "\n";

    OpenGLApplication application("pain",200,200);
    OBJMesh loader(InFile("Models/cow.obj"));

    OpenGLMesh mesh (loader.getAttributes(), loader.getIndices());
    SimpleShadedMaterial material = SimpleShadedMaterial(application.getShaderManager(), Image(InFile("Images/grid_blue.png")));

    OBJMesh loader_2(InFile("Models/cube.obj"), false);
    OpenGLMesh mesh_2 (loader_2.getAttributes(), loader_2.getIndices());
    SimpleShadedMaterial material_2 = SimpleShadedMaterial(application.getShaderManager(), Image(InFile("Images/grid_orange.png")));

    application.setActiveCamera(Camera({50.0f,50.0f,50.0f}, {0.0f,0.0f,0.0f}, {0.0f,0.0f,1.0f}, 4.0/3.0));


    int time = 0;
    while (application.run()){
        material.light_pos = {sin(0.01*time)*50.0,cos(0.01*time)*50.0,sin(0.01*time)*50.0 };
        material_2.light_pos = {sin(0.01*time)*50.0,cos(0.01*time)*50.0,sin(0.01*time)*50.0 };
        srand(1);
        for (int i = 0; i < 100; ++i) {

            glm::mat4  mat = glm::identity<glm::mat4>();
            mat = glm::translate(mat, {((float)rand() / RAND_MAX) * 50.0,((float)rand() / RAND_MAX) * 50.0,((float)rand() / RAND_MAX) * 50.0});
            mat = glm::rotate(mat, (float)time / 100.0f, {((float)rand() / RAND_MAX),((float)rand() / RAND_MAX),((float)rand() / RAND_MAX)});
            mat = glm::scale(mat,{0.4,0.4,0.4});

            application.draw(&mesh, &material,mat );
        }
        srand(2);
        for (int i = 0; i < 100; ++i) {

            glm::mat4  mat = glm::identity<glm::mat4>();
            mat = glm::translate(mat, {((float)rand() / RAND_MAX) * 50.0,((float)rand() / RAND_MAX) * 50.0,((float)rand() / RAND_MAX) * 50.0});
            mat = glm::rotate(mat, (float)time / 100.0f, {((float)rand() / RAND_MAX),((float)rand() / RAND_MAX),((float)rand() / RAND_MAX)});
            mat = glm::scale(mat,{2,2,2});

            application.draw(&mesh_2, &material_2,mat );
        }
time++;
        //todo read: https://learnopengl.com/Guest-Articles/2020/Skeletal-Animation


    }
    //todo event system
    //todo c# style task system
    //https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling

    //todo resource dependency management. Image->Texture->Material->Node in scene graph. Re-use as much as possible.
    //todo https://learnopengl.com/Guest-Articles/2021/Tessellation/Tessellation
}
