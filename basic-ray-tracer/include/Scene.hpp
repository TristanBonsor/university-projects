/**
 * \file Scene.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <vector>

#include "LightSource.hpp"
#include "Sphere.hpp"

/**
 * \class Scene Scene.hpp "include/Scene.hpp"
 * \brief Encapsulates all surfaces in scene.
 *
 * Contains all surfaces in a scene; provides access to a list of these
 * surfaces.
 */
class Scene
{
public:

    /**
     * Initialize the scene.
     */
    static void init();

    /**
     * Get a list of pointers to all surfaces in the scene.
     * @return A reference to a list of pointers to all surfaces in the scene.
     */
    static const std::vector<Surface*>& getSurfaces();

    /**
     * Get a list of light sources in the scene.
     * @return A reference to a list of all light sources in the scene.
     */
    static const std::vector<LightSource>& getLightSources();

    /**
     * Get the ambient light intensity of the scene.
     * @return The ambient light intensity value.
     */
    static float getAmbientLightIntensity();

protected:

    /**
     * Sphere 1.
     */
    static Sphere m_sphere1;

    /**
     * Sphere 2.
     */
    static Sphere m_sphere2;

    /**
     * Sphere 3.
     */
    static Sphere m_sphere3;

    /**
     * Sphere 4.
     */
    static Sphere m_sphere4;

    /**
     * List of all surfaces in the scene.
     */
    static std::vector<Surface*> m_surfaces;

    /**
     * List of all light sources in the scene.
     */
    static std::vector<LightSource> m_light_sources;

    /**
     * The ambient light intensity of the scene.
     */
    static float m_ambient_light_intensity;
};

#endif /* SCENE_HPP_ */
