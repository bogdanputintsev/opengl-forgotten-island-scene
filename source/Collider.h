//----------------------------------------------------------------------------------------
/**
 * \file       Collider.h
 * \author     Bogdan Putintsev
 * \date       2021/05/13
 * \brief      Class describing the collider behaviour and its position
 *
 *
*/
//----------------------------------------------------------------------------------------

#pragma once

class Collider
{
public:
  Collider(const float x1, const float x2, const float y1, const float y2, const float z1, const float z2, const bool isInside);
  bool checkCollision(const float x, const float y, const float z);
  void disable();
private:
  float minX, maxX;
  float minY, maxY;
  float minZ, maxZ;
  bool checkInside;         ///< Decide whether the player has to be inside or outside the collider
  bool enabled = true;
};