/*
 * Scene.cc
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#include "Defines.hpp"
#include "Scene.hpp"

Sphere Scene::m_sphere1 = Sphere(CRGB(SPHERE1_SURFACE_COLOR),
                                 CRGB(SPHERE1_SPECULAR_COLOR),
                                 CRGB(SPHERE1_AMBIENT_COLOR),
                                 Vector3d(SPHERE1_CENTER),
                                 SPHERE1_RADIUS);

Sphere Scene::m_sphere2 = Sphere(CRGB(SPHERE2_SURFACE_COLOR),
                                 CRGB(SPHERE2_SPECULAR_COLOR),
                                 CRGB(SPHERE2_AMBIENT_COLOR),
                                 Vector3d(SPHERE2_CENTER),
                                 SPHERE2_RADIUS);

Sphere Scene::m_sphere3 = Sphere(CRGB(SPHERE3_SURFACE_COLOR),
                                 CRGB(SPHERE3_SPECULAR_COLOR),
                                 CRGB(SPHERE3_AMBIENT_COLOR),
                                 Vector3d(SPHERE3_CENTER),
                                 SPHERE3_RADIUS);

Sphere Scene::m_sphere4 = Sphere(CRGB(SPHERE4_SURFACE_COLOR),
                                 CRGB(SPHERE4_SPECULAR_COLOR),
                                 CRGB(SPHERE4_AMBIENT_COLOR),
                                 Vector3d(SPHERE4_CENTER),
                                 SPHERE4_RADIUS);

std::vector<Surface*> Scene::m_surfaces = std::vector<Surface*>();

std::vector<LightSource> Scene::m_light_sources =
{ LightSource(Vector3d(LIGHT1_POSITION),
              LIGHT1_RED_INTENSITY,
              LIGHT1_GREEN_INTENSITY,
              LIGHT1_BLUE_INTENSITY),
  LightSource(Vector3d(LIGHT2_POSITION),
              LIGHT2_RED_INTENSITY,
              LIGHT2_GREEN_INTENSITY,
              LIGHT2_BLUE_INTENSITY) };

float Scene::m_ambient_light_intensity = SCENE_AMBIENT_LIGHT_INTENSITY;

void Scene::init()
{
    m_surfaces.push_back(&m_sphere1);
    m_surfaces.push_back(&m_sphere2);
    m_surfaces.push_back(&m_sphere3);
    m_surfaces.push_back(&m_sphere4);
}

const std::vector<Surface*>& Scene::getSurfaces()
{
    return m_surfaces;
}

const std::vector<LightSource>& Scene::getLightSources()
{
    return m_light_sources;
}

float Scene::getAmbientLightIntensity()
{
    return m_ambient_light_intensity;
}
