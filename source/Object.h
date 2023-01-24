//----------------------------------------------------------------------------------------
/**
 * \file       Object.h
 * \author     Bogdan Putintsev
 * \date       2021/05/13
 * \brief      Class for all objects on a scene.
 *
 *  Contains vertices, texture infromation, animation implementation and curves.
 *
*/
//----------------------------------------------------------------------------------------


#pragma once

#include <pgr.h>
#include <iostream>

class Object
{
public:
  /// Enum for the type of the object. This type defines the behaviour of the object and its lighting
  enum ObjectType
  {
    EMPTY = 0,        ///< Default value
    SKYBOX = 1,       ///< Skybox has different intensity of color depending on a time
    MESH = 2,         ///< Static mesh
    DOOR = 3,         ///< Door with animation of opening
    ANIMATED = 4,     ///< Mouse object that moves throught the curve
    WATER = 5         ///< Water with dynamic texture
  };

  Object(std::string meshPath, std::string firstTextureName, ObjectType type);

  void init(GLuint shaderProgram);
  void draw();

  void drawDoor();
  void pushDoor();
  void doorAnimation(const float keyOffset);

  void mouseClick()
  {
    mouseEnabled = !mouseEnabled;
  }

  void drawMouse();
private:
  int objectId;
  ObjectType objectType;
  unsigned int objectTypePosition;

  glm::vec3 position;
  glm::mat4 translate;
  glm::mat4 localRotation;
  glm::mat4 globalRotation;

  std::vector<float> vertices;

  GLuint arrayBuffer;
  GLuint vao;
  GLuint vertexAtribPointerPos;
  GLuint normalPosition;
  unsigned int transformMatrixPosition;

  GLuint texturePosition;
  GLuint textureSamplerPosition;
  GLuint textureCoordsPosition;
  std::string textureName; 
  unsigned int skyboxTexture;
  unsigned int skyboxTextureSamplerPos;

  bool doorOpen = false;
  float doorFrame = 0.0f;

  bool mouseEnabled = true;
  float mouseFrame = 0.0f;
  float animationAlpha = 0.0f;
  glm::vec3 lastMousePos = glm::vec3(0.0f, 0.0f, 0.0f);
  float lastRotation = 0.0f;

  float waterFrame = 0.0f;
  unsigned int waterFramePos;

  void transition(const float x, const float y, const float z);
  glm::vec3 bezierPosition(const glm::vec3 startPosition, const glm::vec3 middlePosition, const glm::vec3 finishPosition, const bool future);
  glm::vec3 randomPointInCircle();
};