//----------------------------------------------------------------------------------------
/**
 * \file       Camera.h
 * \author     Bogdan Putintsev
 * \date       2021/05/13
 * \brief      Camera class with movement implementation and animation
 *
*/
//----------------------------------------------------------------------------------------

#include "Camera.h"
#include "Collider.h"

#include <iostream>

Camera::Camera(float fov, float ratio, float zNear, float zFar, glm::vec3& startPosition, glm::vec3& startDirection, float startSpeed)
{
  perspectiveMatrix = glm::perspective(fov, ratio, zNear, zFar);

  positionVector = startPosition;
  directionVector = startDirection;
  upVector = glm::vec3(0.0f, 1.0f, 0.0f);
  yaw = pitch = 0;

  locked = false;
  speed = startSpeed;
}

void Camera::init(GLuint shaderProgram)
{
  loadCollisions();

  viewMatrix = glGetUniformLocation(shaderProgram, "viewMatrix");
  eyePos = glGetUniformLocation(shaderProgram, "eyePos");
  eyeDirection = glGetUniformLocation(shaderProgram, "eyeDirection");
  CHECK_GL_ERROR();
}

void Camera::draw()
{
  glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(this->getViewProjection()));

  if(cameraFrame > 0)
    animation();

  glUniform3f(eyePos, positionVector.x, positionVector.y, positionVector.z);
  glUniform3f(eyeDirection, directionVector.x, directionVector.y, directionVector.z);
  CHECK_GL_ERROR();
}

glm::mat4 Camera::getViewProjection()
{
  return perspectiveMatrix * glm::lookAt(positionVector, positionVector + directionVector, upVector);
}

void Camera::move(Camera::Direction direction)
{
  if (locked)
    return;

  glm::vec3 moveVector = positionVector + getMoveVector(direction);
  if (checkCollisions(moveVector))
    positionVector = moveVector;

  //std::cout << positionVector.x << ' ' << positionVector.y << ' ' << positionVector.z << ' ' << yaw << ' ' << pitch << std::endl;
}

glm::vec3 Camera::getMoveVector(Camera::Direction direction)
{
  switch (direction)
  {
  case FORWARD:
    return directionVector * speed;

  case BACKWARD:
    return -(directionVector * speed);

  case UP:
    return verticalVector() * speed;

  case DOWN:
    return -(verticalVector() * speed);
    break;

  case LEFT:
    return -(glm::normalize(glm::cross(directionVector, upVector) * speed));

  case RIGHT:
    return (glm::normalize(glm::cross(directionVector, upVector) * speed));
  }

  return glm::vec3(0.0f, 0.0f, 0.0f);
}

bool Camera::checkCollisions(glm::vec3 moveVector)
{
  for (auto& collider : colliderList)
    if (!collider.checkCollision(moveVector.x, moveVector.y, moveVector.z))
      return false;
  return true;
}

glm::vec3 Camera::verticalVector()
{
  return glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::rotate(const float deltaX, const float deltaY)
{
  if (locked)
    return;

  yaw += deltaX;
  pitch += deltaY;

  glm::vec3 front;
  front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));

  directionVector = front;
}

void Camera::clamp(float& value, const float minRange, const float maxRange)
{
  if (value > maxRange)
    value = maxRange;
  else if (value < minRange)
    value = minRange;
}

void Camera::loadCollisions()
{
  Collider skyboxCollider = Collider(-250.0f, 250.0f, -250.0f, 250.0f, -250.0f, 250.0f, true);
  Collider doorCollider = Collider(2.682f, 17.88f, 38.58f, 61.18f, -41.73f, -56.54f, false);
  colliderList.push_back(skyboxCollider);
  colliderList.push_back(doorCollider);
}

void Camera::switchStaticPosition(const int positionId)
{
  locked = true;

  switch (positionId)
  {
  case 1:
    positionVector = staticPosition1.position;
    yaw = staticPosition1.yaw;
    pitch = staticPosition1.pitch;
    break;

  case 2:
    positionVector = staticPosition2.position;
    yaw = staticPosition2.yaw;
    pitch = staticPosition2.pitch;
    break; 

  case 3:
    positionVector = staticPosition3.position;
    yaw = staticPosition3.yaw;
    pitch = staticPosition3.pitch;
    break;

  default:
    unlockView();
    break;
  }
  
  changeDirection(sin(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), -cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
}

void Camera::changeDirection(const float x, const float y, const float z)
{
  directionVector.x = x;
  directionVector.y = y;
  directionVector.z = z;
}

void Camera::startAnimation()
{
  cameraFrame = 120.0f;
}

void Camera::animation()
{
  locked = true;
  animationAlpha = 1 - (cameraFrame / 120.0f);

  glm::vec3 newPos = bezierPosition(false);

  changePosition(newPos.x, newPos.y, newPos.z);


  glm::vec3 futurePosition = bezierPosition(true);

  glm::vec3 normVec = glm::normalize(glm::vec3(futurePosition.x - newPos.x, futurePosition.y - newPos.y, futurePosition.z - newPos.z));
  yaw = atan2f(normVec.z, normVec.x) * 180 / M_PI + 90;

  changeDirection(sin(glm::radians(yaw)) * cos(glm::radians(pitch)),
    sin(glm::radians(pitch)),
    -cos(glm::radians(yaw)) * cos(glm::radians(pitch)));

  cameraFrame -= 0.5f;

  if (cameraFrame <= 0)
    unlockView();
}

void Camera::changePosition(const float x, const float y, const float z)
{
  positionVector.x = x;
  positionVector.y = y;
  positionVector.z = z;
}

glm::vec3 Camera::bezierPosition(bool future)
{
  glm::vec3 result;

  float alpha = animationAlpha;
  if (future)
  {
    alpha = 1 - ((cameraFrame - 0.5f) / 120.0f);
    if (alpha > 1.0f)
      alpha = 1.0f;
  }

  float alpha2 = alpha * alpha;
  float alpha3 = alpha * alpha * alpha;

  float oneMinusAlpha = (1 - alpha);
  float oneMinusAlpha2 = (1 - alpha) * (1 - alpha);
  float oneMinusAlpha3 = (1 - alpha) * (1 - alpha) * (1 - alpha);

  result.x = staticPosition4.position.x * alpha3 + 3 * staticPosition3.position.x * alpha2 * oneMinusAlpha + 3 * staticPosition2.position.x * alpha * oneMinusAlpha2 + staticPosition1.position.x * oneMinusAlpha3;
  result.y = staticPosition4.position.y * alpha3 + 3 * staticPosition3.position.y * alpha2 * oneMinusAlpha + 3 * staticPosition2.position.y * alpha * oneMinusAlpha2 + staticPosition1.position.y * oneMinusAlpha3;
  result.z = staticPosition4.position.z * alpha3 + 3 * staticPosition3.position.z * alpha2 * oneMinusAlpha + 3 * staticPosition2.position.z * alpha * oneMinusAlpha2 + staticPosition1.position.z * oneMinusAlpha3;

  return result;
}

void Camera::unlockView()
{
  locked = false;
}

void Camera::disableCollision()
{
  colliderList.at(1).disable();
}
