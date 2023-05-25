



#include "DogeEngine/Rendering/OpenGL/OpenGLApplication.hpp"
#include "DogeEngine/Rendering/Shaders/Materials/SimpleShadedMaterial.hpp"
#include "DogeEngine/IO/OBJMesh.hpp"
#include "DogeEngine/Utils/GLMPrints.hpp"
#include "DogeEngine/IO/GLTFScene.hpp"
#include "DogeEngine/ECS/Systems/RenderSystem.hpp"
#include "DogeEngine/ECS/Systems/UpdateTransforms.hpp"

using namespace Doge;




int main(int argc, char *argv[]){

    OpenGLApplication application("pain",200,200);
    application.setBackGroundColor({0,0,0});
    application.setActiveCamera(Camera({5.0f,-5.0f,5.0f}, {0.0f,0.0f,0.0f}, {0.0f,-1.0f,0.0f}, 4.0/3.0));
    ECSManager manager;
    ResourceManager rm(std::filesystem::path("./")); //todo defer loading of opengl stuff until opengl is loaded(Should not be a problem once properly integrated into context)
    Resource<GLTFScene> scene1 = rm.requestResource<GLTFScene>("gridworld");

    Entity grid = manager.createEntity();
    manager.addComponent(grid, TransformComponent(glm::translate(glm::mat4(1.0f), {0,0.5,0})));
    manager.addComponent(grid, HierarchyComponent());
    scene1->create(&manager,&rm, grid);

    Resource<GLTFScene> scene2 = rm.requestResource<GLTFScene>("heavy");

    Entity heavy = manager.createEntity();
    manager.addComponent(heavy, TransformComponent(glm::rotate(glm::mat4(1.0f),glm::pi<float>()/2, {0,0,1})));
    manager.addComponent(heavy, HierarchyComponent());
    scene2->create(&manager,&rm, heavy);

    Resource<GLTFScene> scene = rm.requestResource<GLTFScene>("truck");




    std::vector<Entity> cars;
    for (int i = 0; i < 100; ++i) {
        Entity top = manager.createEntity();
        manager.addComponent(top, TransformComponent(glm::mat4(1.0f)));
        manager.addComponent(top, HierarchyComponent());

        scene->create(&manager,&rm,top);
        cars.push_back(top);
    }


    RenderSystem renderer(&application,&manager);
    UpdateTransforms update_transform(&manager);


    double i = 0;
    while (application.run()){
        float idx = 14;
        srand(idx);
        for (Entity car2 : cars) {
            TransformComponent* car = manager.getComponent<TransformComponent>(car2);
            glm::mat4 car_t(1.0f);
            float r = (float)rand() / (float)RAND_MAX ;
            float speed = -(i / idx) * r;
            car_t = glm::translate(car_t, {sin(speed)*idx, 0, cos(speed)*idx});
            car_t = glm::rotate(car_t, (float)speed + glm::pi<float>()/2 + (float)sin(i/10.0f)/100.0f, {0,1,0});
            car->setLocal(car_t);
            idx+=6;
        }


        update_transform.update();
        renderer.update();
        application.setActiveCamera(Camera({sin(i/1000.0) * 15,sin(i/100.0) * -200 - 205,cos(i/1000.0) * 15}, {0.0f,0.0f,0.0f}, {0.0f,-1.0f,0.0f}, 4.0/3.0));
        i++;
    }



    return 0;

/*
    std::cout << rm.requestResource<OpenGLTexture>("grid_blue")->getTextureID() << "\n";

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
*/
  /*  OBJMesh loader(InFile("Models/cow.obj"));

    OpenGLMesh mesh (loader.getAttributes(), loader.getIndices());
    SimpleShadedMaterial material = SimpleShadedMaterial(application.getShaderManager(), rm.requestResource<OpenGLTexture>("grid_blue"));

    OBJMesh loader_2(InFile("Models/cube.obj"), false);
    Resource<OpenGLMesh> mesh_2 = rm.requestResource<OpenGLMesh>("sanford3/Mesh.001/1");
    SimpleShadedMaterial material_2 = SimpleShadedMaterial(application.getShaderManager(), rm.requestResource<OpenGLTexture>("grid_orange"));

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

            application.draw(mesh_2.operator->(), &material_2,mat );
        }
time++;
        //todo read: https://learnopengl.com/Guest-Articles/2020/Skeletal-Animation


    }*/
    //todo event system
    //todo c# style task system
    //https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling


    //todo https://learnopengl.com/Guest-Articles/2021/Tessellation/Tessellation
}
