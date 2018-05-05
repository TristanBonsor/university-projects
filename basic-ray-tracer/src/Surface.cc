/*
 * Surface.cc
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#include "Surface.hpp"

CRGB Surface::getSurfaceColor() const
{
    return m_surface_color;
}

CRGB Surface::getSpecularColor() const
{
    return m_specular_color;
}

CRGB Surface::getAmbientColor() const
{
    return m_ambient_color;
}
