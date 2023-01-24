//----------------------------------------------------------------------------------------
/**
 * \file       Scene.h
 * \author     Bogdan Putintsev
 * \date       2021/05/13
 * \brief      Scene object that keeps all the object
 *
*/
//----------------------------------------------------------------------------------------


#pragma once
#include "Camera.h"
#include "Object.h"
#include "Light.h"
#include "Constants.h"

class Scene
{
public:
  Scene();
  void init(GLuint program);
  void draw();
  void loadObjects();

  void switchFlashLight();
  void switchFog();
  void pushDoor();
  void touchMouse();
private:
  Light light;
  std::vector<Object> objects;
};
