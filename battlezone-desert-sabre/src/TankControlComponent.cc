/*
 * TankControlComponent.cc
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#include "TankControlComponent.hpp"

const Vector3d TankControlComponent::M_DEFAULT_UP(0.0f, 1.0f, 0.0f);

const Vector3d TankControlComponent::M_DEFAULT_AT(0.0f, 0.0f, -1.0f);

const float TankControlComponent::M_TREAD_VELOCITY_MAGNITUDE = 1.0f;

const Vector3d TankControlComponent::M_TREAD_VELOCITY_STOP(0.0f, 0.0f, 0.0f);

const Vector3d TankControlComponent::M_TREAD_VELOCITY_FORWARD =
   3.0f *  M_TREAD_VELOCITY_MAGNITUDE * M_DEFAULT_AT;

const Vector3d TankControlComponent::M_TREAD_VELOCITY_BACKWARD =
    -3.0f * M_TREAD_VELOCITY_MAGNITUDE * M_DEFAULT_AT;

const float TankControlComponent::M_ANGULAR_VELOCITY_STOP = 0.0f;
