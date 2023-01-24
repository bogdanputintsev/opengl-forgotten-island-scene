//----------------------------------------------------------------------------------------
/**
 * \file       light.h
 * \author     Bogdan Putintsev
 * \date       2021/05/13
 * \brief      Describes direction, point and source light
 *
*/
//----------------------------------------------------------------------------------------

#pragma once
#include "pgr.h"

class Light
{
private:
  glm::vec3 color;
  glm::vec3 direction;

  bool flashLightEnabled;
  bool fogEnabled;
  float sunAlpha = 0.0f;

  unsigned int colorPosition;
  unsigned int directionPosition;
  unsigned int flashLightEnabledPosition;
  unsigned int fogEnabledPosition;
  unsigned int sunAlphaPosition;

  /// A structure for a point light with its color and position
  struct PointLight
  {
    glm::vec3 transform = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    float intensity = 0.0f;

    unsigned int transformPos;
    unsigned int intensityPos;
    unsigned int colorPos;
  };

  PointLight pointLight;

public:
  Light(glm::vec3 lightColor, glm::vec3 lightDirection);

  void init(GLuint program);
  void draw();
  void drawPointLight();

  void switchFlashLight();
  void switchFog();
};