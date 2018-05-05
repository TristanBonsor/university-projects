/*
 * LightSource.cc
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#include "LightSource.hpp"

Vector3d LightSource::getPosition() const
{
    return m_position;
}

float LightSource::getRedIntensity() const
{
    return m_red_intensity;
}

float LightSource::getGreenIntensity() const
{
    return m_green_intensity;
}

float LightSource::getBlueIntensity() const
{
    return m_blue_intensity;
}
