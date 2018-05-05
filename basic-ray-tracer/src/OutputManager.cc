/*
 * OutputManager.cc
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#include <cmath>

#include <GL/gl.h>
#include <GL/glut.h>

#include "color.h"
#include "Camera.hpp"
#include "Defines.hpp"
#include "Scene.hpp"
#include "OutputManager.hpp"

CRGB PIXEL_BUFFER[WINDOW_WIDTH * WINDOW_HEIGHT];

int OutputManager::m_recursion_level = 0;

void OutputManager::init()
{
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowPosition(WINDOW_X, WINDOW_Y);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_TITLE);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
}

void OutputManager::renderScene()
{
    for(int x = 0; x < WINDOW_WIDTH; ++x)
    {
        for(int y = 0; y < WINDOW_HEIGHT; ++y)
        {
            int ix = y * WINDOW_WIDTH + x;

            Ray ray = generateViewingRay(x, y);
            m_recursion_level = 0;

            PIXEL_BUFFER[ix] = rayColor(ray, MIN_INTERVAL, MAX_INTERVAL);
        }
    }

    glDrawPixels(WINDOW_WIDTH,
                 WINDOW_HEIGHT,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 PIXEL_BUFFER);

    glutSwapBuffers();
}

float OutputManager::getImagePlanePosition(const int& unit, const int& distance)
{
    float max = distance / 2.0f;
    float min = -max;

    return min + (max - min) * (unit + 0.5f) / (float)distance;
}

Vector3d OutputManager::getRayDirection(const float& x, const float& y)
{
    return Camera::getAt().getScaled(Camera::getImageDistance())
           + Camera::getRight().getScaled(x) + Camera::getUp().getScaled(y);
}

Ray OutputManager::generateViewingRay(const int& x, const int& y)
{
    float rel_x = getImagePlanePosition(x, WINDOW_WIDTH);
    float rel_y = getImagePlanePosition(y, WINDOW_HEIGHT);

    return Ray(Camera::getPosition(), getRayDirection(rel_x, rel_y));
}

CRGB OutputManager::rayColor(const Ray& ray, const float& t0, const float& t1)
{
    float t = t1;

    const std::vector<Surface*>& surfaces = Scene::getSurfaces();

    Surface* hit_surface = 0;

    // Determine the nearest object (to the camera) that the ray hits,
    // if at all.
    for(unsigned int i = 0; i < surfaces.size(); ++i)
    {
        Ray::Contact contact = surfaces[i]->hit(ray, t0, t);

        if(contact.isContact())
        {
            hit_surface = surfaces[i];
            t = contact.getParameter();
        }
    }

    if(hit_surface)
    {
        // Determine the color generated from the ray striking a surface.

        Vector3d p = ray.interpolate(t);
        Vector3d n = hit_surface->getNormal(p).getNormalized();

        CRGB color = hit_surface->getAmbientColor()
                     * Scene::getAmbientLightIntensity();

        const std::vector<LightSource>& light_sources =
            Scene::getLightSources();

        float red_light_total = 0;
        float green_light_total = 0;
        float blue_light_total = 0;

        // Determine RGB values from all light sources.
        for(unsigned int i = 0; i < light_sources.size(); ++i)
        {
            Vector3d v = ray.getOrigin() - p;
            Vector3d l = light_sources[i].getPosition() - p;

            Ray shadow_ray(p, l);
            bool shadow_hit = false;

            // Determine the nearest object that the shadow ray hits,
            // if at all.
            for(unsigned int i = 0; i < surfaces.size(); ++i)
            {
                Ray::Contact contact = surfaces[i]->hit(shadow_ray,
                                                        0.00001f,
                                                        1.0f);

                if(contact.isContact())
                {
                    shadow_hit = true;
                    break;
                }
            }

            if(!shadow_hit)
            {
                // No surface is in between the hit surface and the light source.

                Vector3d h = l.getNormalized() + v.getNormalized();
                h.normalize();

                // Total red channel diffuse and specular values.
                float red_intensity = light_sources[i].getRedIntensity();
                float rd = red_intensity * hit_surface->getSurfaceColor().Red()
                           * max(0, Vector3d::dotProduct(n, l.getNormalized()));
                float rs = red_intensity * hit_surface->getSpecularColor().Red()
                           * pow(max(0, Vector3d::dotProduct(n, h)),
                           PHONG_EXPONENT);

                red_light_total += rd + rs;

                // Total green channel diffuse and specular values.
                float green_intensity = light_sources[i].getGreenIntensity();
                float gd = green_intensity
                           * hit_surface->getSurfaceColor().Green()
                           * max(0, Vector3d::dotProduct(n, l.getNormalized()));
                float gs = green_intensity
                           * hit_surface->getSpecularColor().Green()
                           * pow(max(0, Vector3d::dotProduct(n, h)),
                           PHONG_EXPONENT);

                green_light_total += gd + gs;

                // Total blue channel diffuse and specular values.
                float blue_intensity = light_sources[i].getBlueIntensity();
                float bd = blue_intensity
                           * hit_surface->getSurfaceColor().Blue()
                           * max(0, Vector3d::dotProduct(n, l.getNormalized()));
                float bs = blue_intensity
                           * hit_surface->getSpecularColor().Blue()
                           * pow(max(0, Vector3d::dotProduct(n, h)),
                           PHONG_EXPONENT);

                blue_light_total += bd + bs;
            }
        }

        if(++m_recursion_level < MAX_RECURSION_LEVEL)
        {
            // Calculate reflections/refractions.

            Vector3d r =
                ray.getDirection()
                - n.getScaled(2 * Vector3d::dotProduct(ray.getDirection(), n));

            Ray reflect_ray(p, r);

            return color
                   + CRGB((int)red_light_total,
                          (int)green_light_total,
                          (int)blue_light_total)
                   + rayColor(reflect_ray, MIN_INTERVAL, MAX_INTERVAL)
                     * MIRROR_REFLECTION;
        }

        return color
               + CRGB((int)red_light_total,
                      (int)green_light_total,
                      (int)blue_light_total);
    }

    return CRGB(0, 0, 0); // No surface was hit by the ray.
}

float OutputManager::min(const float& a, const float& b)
{
    return (a < b ? a : b);
}

float OutputManager::max(const float& a, const float& b)
{
    return (a > b ? a : b);
}
