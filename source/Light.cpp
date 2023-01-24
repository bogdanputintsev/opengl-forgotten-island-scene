//----------------------------------------------------------------------------------------
/**
 * \file       light.cpp
 * \author     Bogdan Putintsev
 * \date       2021/05/13
 * \brief      Describes direction, point and source light
 *
*/
//----------------------------------------------------------------------------------------


#include "Light.h"
#include <iostream>

Light::Light(glm::vec3 lightColor, glm::vec3 lightDirection)
{
  flashLightEnabled = false;
  fogEnabled = false;
  color = lightColor;
  direction = lightDirection;
  sunAlpha = 0.0f;

  pointLight.transform = glm::vec3(-12.26f, 52.24f, -12.73f);
  pointLight.color = glm::vec3(1.0f, 0.6f, 0.0f);
  pointLight.intensity = 5.0f;
}

void Light::init(GLuint shaderProgram)
{
  directionPosition = glGetUniformLocation(shaderProgram, "sunDirection");
  colorPosition = glGetUniformLocation(shaderProgram, "lightColor");
  flashLightEnabledPosition = glGetUniformLocation(shaderProgram, "flashLightEnabled");
  fogEnabledPosition = glGetUniformLocation(shaderProgram, "fogEnabled");
  sunAlphaPosition = glGetUniformLocation(shaderProgram, "sunAlpha");

  pointLight.transformPos = glGetUniformLocation(shaderProgram, "pointPosition");
  pointLight.colorPos = glGetUniformLocation(shaderProgram, "pointColor");
  pointLight.intensityPos = glGetUniformLocation(shaderProgram, "pointIntensity");

  CHECK_GL_ERROR();
}

void Light::draw()
{
  direction.x = cos(sunAlpha * 2 * M_PI);
  direction.y = sin(sunAlpha * 2 * M_PI);
  direction.z = 0.0f;

  sunAlpha += 0.0005f;
  if (sunAlpha > 1.0f)
    sunAlpha = 0.0f;

  float sunFunction = (4 * sunAlpha * sunAlpha) - (4 * sunAlpha) + 1;
  glUniform1f(sunAlphaPosition, sunFunction);

  glUniform3f(directionPosition, direction.x, direction.y, direction.z);
  glUniform3f(colorPosition, color.x, color.y, color.z);

  glUniform1i(flashLightEnabledPosition, flashLightEnabled ? 1 : 0);
  glUniform1i(fogEnabledPosition, fogEnabled ? 1 : 0);

  drawPointLight();

  CHECK_GL_ERROR();
}

void Light::drawPointLight()
{
  pointLight.intensity = std::max(2.0f, pointLight.intensity + (rand() % 4) - 2);
  pointLight.color.y = std::max(std::min(0.0f, pointLight.color.y + (rand() / 10) - 0.05f), 0.5f);

  glUniform3f(pointLight.transformPos, pointLight.transform.x, pointLight.transform.y, pointLight.transform.z);
  glUniform3f(pointLight.colorPos, pointLight.color.x, pointLight.color.y, pointLight.color.z);
  glUniform1f(pointLight.intensityPos, pointLight.intensity);
}

void Light::switchFlashLight()
{
  flashLightEnabled = !flashLightEnabled;
}

void Light::switchFog()
{
  fogEnabled = !fogEnabled;
  glUniform1i(fogEnabledPosition, fogEnabled);
}