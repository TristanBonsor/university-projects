/*
 * Camera.cc
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#include "Camera.hpp"

const Vector3d Camera::M_DEFAULT_UP = Vector3d(0.0f, 1.0f, 0.0f);
const Vector3d Camera::M_DEFAULT_AT = Vector3d(0.0f, 0.0f, -1.0f);
const float Camera::M_FOCUS_RADIUS = 256.0f;

Vector3d Camera::m_position = Vector3d();
Quaternion Camera::m_orientation = Quaternion();
