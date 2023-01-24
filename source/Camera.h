//----------------------------------------------------------------------------------------
/**
 * \file       Camera.h
 * \author     Bogdan Putintsev
 * \date       2021/05/13
 * \brief      Camera class with movement implementation and animation
 *
*/
//----------------------------------------------------------------------------------------

#pragma once
#include "pgr.h"
#include "Collider.h"

class Camera
{
public:
  Camera(float fov, float ratio, float zNear, float zFar, glm::vec3& startPosition, glm::vec3& startDirection, float startSpeed);

  /// Enum for direction of the next move
  enum Direction
  {
    FORWARD,      ///< Up arrow key
    BACKWARD,     ///< Down arrow key
    UP,           ///< Q key
    DOWN,         ///< E key
    LEFT,         ///< Left arrow key
    RIGHT         ///< Right arrow key
  };

  void init(GLuint shaderProgram);
  void draw();
  void move(Direction direction);
  void rotate(const float mouseX, const float mouseY);
  
  void startAnimation();
  void changePosition(const float x, const float y, const float z);
  void changeDirection(const float x, const float y, const float z);
  void loadCollisions();
  void switchStaticPosition(const int positionId);
  void unlockView();
  void disableCollision();

private:
  glm::mat4 perspectiveMatrix;
  uint32_t viewMatrix;

  GLuint eyePos;
  GLuint eyeDirection;

  GLuint texturePosition;

  glm::vec3 positionVector;
  glm::vec3 directionVector;
  glm::vec3 upVector;

  bool locked;
  float yaw;
  float pitch;
  float speed;

  std::vector<Collider> colliderList;

  /// Structure containing information about position. Used in static positions.
  struct StaticPosition
  {
    StaticPosition()
    {
      position = (glm::vec3(0.0f, 0.0f, 0.0f));
      yaw = pitch = 0.0f;
    }

    StaticPosition(const glm::vec3 staticPos, const float newYaw, const float newPitch)
    {
      position = staticPos;
      yaw = newYaw;
      pitch = newPitch;
    }

    glm::vec3 position;
    float yaw = 0.0f;
    float pitch = 0.0f;

  };

  StaticPosition staticPosition1 = StaticPosition(glm::vec3(105.7f, 85.5f, 197.0f), -198, 181);
  StaticPosition staticPosition2 = StaticPosition(glm::vec3(173.9f, 77.8f, -60.4f), -274, 189);
  StaticPosition staticPosition3 = StaticPosition(glm::vec3(32.1f, 51.8f, -109.2f), -339, 181);
  StaticPosition staticPosition4 = StaticPosition(glm::vec3(17.5f, 55.5f, -71.7f), 158, 2);

  float cameraFrame = 0.0f;
  float animationAlpha = 0.0f;

  glm::vec3 getMoveVector(Direction direction);
  bool checkCollisions(glm::vec3 moveVector);
  glm::mat4 getViewProjection();
  glm::vec3 verticalVector();

  void animation();
  glm::vec3 bezierPosition(bool future);
  static void clamp(float& value, const float minRange, const float maxRange);
};