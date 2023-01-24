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


#include "Collider.h"

#include <algorithm>
#include <iostream>

Collider::Collider(const float x1, const float x2, const float y1, const float y2, const float z1, const float z2, const bool isInside)
{
  minX = std::min(x1, x2);
  maxX = std::max(x1, x2);
  minY = std::min(y1, y2);
  maxY = std::max(y1, y2);
  minZ = std::min(z1, z2);
  maxZ = std::max(z1, z2);

  checkInside = isInside;
}

bool Collider::checkCollision(const float x, const float y, const float z)
{
  if (!enabled)
    return true;

  bool playerIsInside = (minX <= x && x <= maxX) && (minY <= y && y <= maxY) && (minZ <= z && z <= maxZ);
  if (checkInside)
    return playerIsInside;
  return !playerIsInside;
}

void Collider::disable()
{
  enabled = false;
}