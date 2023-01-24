//----------------------------------------------------------------------------------------
/**
 * \file       Object.cpp
 * \author     Bogdan Putintsev
 * \date       2021/05/13
 * \brief      Class for all objects on a scene.
 *
 *  Contains vertices, texture infromation, animation implementation and curves.
 *
*/
//----------------------------------------------------------------------------------------


#include "Object.h"
#include "OBJParser.h"

#include <ctime> 

Object::Object(std::string meshPath, std::string firstTextureName, ObjectType type)
{
  objectType = type;
  position = glm::vec3(0.0f, 0.0f, 0.0f);
  translate = glm::mat4(1.0f);
  localRotation = glm::mat4(1.0f);
  globalRotation = glm::mat4(1.0f);
  waterFrame = 0;

  if (!readOBJ(meshPath.c_str(), vertices))
    std::cout << "Failed to read file: " << meshPath << "." << std::endl;

  textureName = firstTextureName;

}

void Object::init(GLuint shaderProgram)
{
  glGenBuffers(1, &arrayBuffer);
  CHECK_GL_ERROR();

  glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
  CHECK_GL_ERROR();

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
  CHECK_GL_ERROR();

  glGenVertexArrays(1, &vao);
  CHECK_GL_ERROR();

  glBindVertexArray(vao);
  CHECK_GL_ERROR();

  vertexAtribPointerPos = 0;
  glEnableVertexAttribArray(vertexAtribPointerPos);
  CHECK_GL_ERROR();

  glVertexAttribPointer(vertexAtribPointerPos, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
  CHECK_GL_ERROR();

  normalPosition = 1;
  glEnableVertexAttribArray(normalPosition);
  CHECK_GL_ERROR();

  glVertexAttribPointer(normalPosition, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
  CHECK_GL_ERROR();

  objectTypePosition = glGetUniformLocation(shaderProgram, "objectType");
  CHECK_GL_ERROR();

  transformMatrixPosition = glGetUniformLocation(shaderProgram, "transform");
  CHECK_GL_ERROR();

  if (textureName != "")
  {
    texturePosition = pgr::createTexture(textureName, true);
    glActiveTexture(GL_TEXTURE0);
    CHECK_GL_ERROR();

    glBindTexture(GL_TEXTURE_2D, texturePosition);
    CHECK_GL_ERROR();

    if (texturePosition == 0) {
      pgr::dieWithError("Failed to load texture.");
    }

    textureSamplerPosition = glGetUniformLocation(shaderProgram, "MTexture");
    CHECK_GL_ERROR();

    glUseProgram(shaderProgram);
    CHECK_GL_ERROR();

    glUniform1i(textureSamplerPosition, 0);
    CHECK_GL_ERROR();

    textureCoordsPosition = 2;
    glVertexAttribPointer(textureCoordsPosition, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    CHECK_GL_ERROR();

    glEnableVertexAttribArray(textureCoordsPosition);
    CHECK_GL_ERROR();
  }

   waterFramePos = glGetUniformLocation(shaderProgram, "waterFrame");
}

void Object::draw()
{
  if (objectType == SKYBOX)
    glUniform1i(objectTypePosition, 1);
  else if (objectType == WATER)
    glUniform1i(objectTypePosition, 5);
  else
    glUniform1i(objectTypePosition, 2);

  if (objectType == Object::DOOR)
    drawDoor();
  else if (objectType == Object::ANIMATED)
    drawMouse();
  else if (objectType == Object::WATER)
  {
    glUniform1i(waterFramePos, waterFrame);
    waterFrame = (waterFrame + 0.35);
  }
  
  glUniformMatrix4fv(transformMatrixPosition, 1, GL_FALSE, glm::value_ptr(globalRotation * translate * localRotation));

  if (textureName != "")
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturePosition);
  }


  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 8);

  CHECK_GL_ERROR();

}

void Object::drawMouse()
{
  if (!mouseEnabled)
    return;

  glm::vec3 startPosition = glm::vec3(-45.41f, 42.09f, -40.38f);
  glm::vec3 middlePosition = glm::vec3(1.781f, 42.09f, -44.4f);
  glm::vec3 finishPosition = glm::vec3(-28.07f, 42.09f, -9.235f);

  if (mouseFrame <= 0.0f)
  {
    mouseFrame = 120.0f;
    lastRotation = 0.0f;
  }

  animationAlpha = 1 - (mouseFrame / 120.0f);
  glm::vec3 actualPosition = bezierPosition(startPosition, middlePosition, finishPosition, false);
  glm::vec3 futurePosition = bezierPosition(startPosition, middlePosition, finishPosition, true);

  glm::vec3 normVec = glm::normalize(glm::vec3(futurePosition.x - actualPosition.x, 0.0f, futurePosition.z - actualPosition.z));
  float rotationDegree = atan2f(normVec.z, normVec.x) * 180 / M_PI;

  transition(0.0f, 0.0f, 0.0f);
  localRotation = glm::rotate(glm::mat4(), glm::radians(rotationDegree), glm::vec3(0.0f, -1.0f, 0.0f));
  transition(actualPosition.x, actualPosition.y, actualPosition.z);
  mouseFrame -= 0.75f;

  lastRotation = rotationDegree;
}

glm::vec3 Object::randomPointInCircle()
{
  glm::vec3 result = glm::vec3(-20.78f, 42.09f, -33.3f);

  srand((unsigned)time(0));

  result.x += (rand() % 23) - 11;
  result.z -= (rand() % 23) - 11;

  return result;
}

glm::vec3 Object::bezierPosition(const glm::vec3 startPosition, const glm::vec3 middlePosition, const glm::vec3 finishPosition, const bool future)
{
  glm::vec3 result;

  float alpha = animationAlpha;
  if (future) {
    alpha = 1 - ((mouseFrame - 1.2f) / 120.0f);
    if (alpha > 1.0f)
      alpha = 1.0f;
  }

  float alpha2 = alpha * alpha;
  float alpha3 = alpha * alpha * alpha;

  float oneMinusAlpha = (1 - alpha);
  float oneMinusAlpha2 = (1 - alpha) * (1 - alpha);
  float oneMinusAlpha3 = (1 - alpha) * (1 - alpha) * (1 - alpha);

  result.x = startPosition.x * alpha3 + 3 * finishPosition.x * alpha2 * oneMinusAlpha + 3 * middlePosition.x * alpha * oneMinusAlpha2 + startPosition.x * oneMinusAlpha3;
  result.y = startPosition.y * alpha3 + 3 * finishPosition.y * alpha2 * oneMinusAlpha + 3 * middlePosition.y * alpha * oneMinusAlpha2 + startPosition.y * oneMinusAlpha3;
  result.z = startPosition.z * alpha3 + 3 * finishPosition.z * alpha2 * oneMinusAlpha + 3 * middlePosition.z * alpha * oneMinusAlpha2 + startPosition.z * oneMinusAlpha3;

  return result;
}


void Object::drawDoor()
{
  transition(11.313f, 46.65f, -46.494f);

  if (doorFrame > 0.0f)
  {
    float keyOffset = 1.5f;

    doorAnimation(keyOffset);
    doorFrame -= keyOffset;

    if (doorFrame <= 0)
      doorOpen = !doorOpen;
  }
}

void Object::pushDoor()
{
  if (objectType != DOOR || doorFrame != 0.0f)
    return;

  doorFrame = 120.0f;
}

void Object::doorAnimation(const float keyOffset)
{
  transition(0.0f, 0.0f, 0.0f);

  if (doorOpen)
    localRotation = glm::rotate(glm::mat4(), glm::radians(keyOffset), glm::vec3(0.0f, 1.0f, 0.0f)) * localRotation;
  else
    localRotation = glm::rotate(glm::mat4(), glm::radians(-keyOffset), glm::vec3(0.0f, 1.0f, 0.0f)) * localRotation;

  transition(11.313f, 46.65f, -46.494f);
}

void Object::transition(const float x, const float y, const float z)
{
  translate[3].x = x;
  translate[3].y = y;
  translate[3].z = z;
}
