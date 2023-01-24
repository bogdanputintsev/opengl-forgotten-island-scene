//----------------------------------------------------------------------------------------
/**
 * \file       Scene.cpp
 * \author     Bogdan Putintsev
 * \date       2021/05/13
 * \brief      Scene object that keeps all the object
 *
*/
//----------------------------------------------------------------------------------------

#include "Scene.h"

Scene::Scene()
  : light(glm::vec3(1.0f, 0.65f, 0.8f), glm::vec3(3.0f, 1.0f, 1.0f))
{
  objects = std::vector<Object>();
}


void Scene::init(GLuint shaderProgram)
{
  light.init(shaderProgram);

  for (auto& object : objects)
    object.init(shaderProgram);

  CHECK_GL_ERROR();
}

void Scene::draw()
{
  light.draw();
  CHECK_GL_ERROR();

  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  int objectId = 0;

  for (auto& object : objects)
  {
    glStencilFunc(GL_ALWAYS, objectId++, 255);
    object.draw();
  }

  glDisable(GL_STENCIL_TEST);

  CHECK_GL_ERROR();

}

void Scene::loadObjects()
{
  Object skybox = Object("data/skybox/skybox.obj", "data/skybox/skybox.png", Object::SKYBOX);
  Object floor = Object("data/floor/floor.obj", "data/floor/grass.jpg", Object::MESH);
  Object door = Object("data/door/door.obj", "data/door/door.jpg", Object::DOOR);
  Object threshold = Object("data/threshold/threshold.obj", "data/door/door.jpg", Object::MESH);
  Object indoor = Object("data/indoor/indoor.obj", "data/indoor/brick.jpg", Object::MESH);
  Object indoorFloor = Object("data/indoor/indoor_floor.obj", "data/indoor/floor.jpg", Object::MESH);
  Object mouse = Object("data/mouse/mouse.obj", "data/mouse/mouse.png", Object::ANIMATED);
  Object water = Object("data/water/water.obj", "data/water/water.jpg", Object::WATER);
  Object torch = Object("data/torch/torch.obj", "data/torch/textures/torch.jpg", Object::MESH);
  Object chest = Object("data/chest/chest.obj", "data/chest/textures/chest.jpg", Object::MESH);

  objects.push_back(skybox);
  objects.push_back(floor);
  objects.push_back(door);
  objects.push_back(threshold);
  objects.push_back(indoor);
  objects.push_back(indoorFloor);
  objects.push_back(mouse);
  objects.push_back(water);
  objects.push_back(torch);
  objects.push_back(chest);
}

void Scene::switchFlashLight()
{
  light.switchFlashLight();
}

void Scene::switchFog()
{
  light.switchFog();
}

void Scene::pushDoor()
{
  objects.at(2).pushDoor();
}

void Scene::touchMouse()
{
  objects.at(6).mouseClick();
}
