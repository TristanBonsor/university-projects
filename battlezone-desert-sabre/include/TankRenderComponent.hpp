/*
 * \file TankRenderComponent.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef TANKRENDERCOMPONENT_HPP_
#define TANKRENDERCOMPONENT_HPP_

#include <GL/gl.h>

#include "RenderComponent.hpp"

class TankRenderComponent: public RenderComponent
{
public:

    virtual ~TankRenderComponent()
    {
    }

    virtual inline void render();

protected:
};

inline void TankRenderComponent::render()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(m_position.x, m_position.y, m_position.z);
    glRotatef(m_angle / 3.14159f * 180.0f, m_axis.x, m_axis.y, m_axis.z);

    //const float s = 1.0f;

    glBegin(GL_QUADS);

    // Chasis bottom.
    glColor3f(0.0f, 0.25f, 0.0f);

    glVertex3f(-1.0f, -0.5f, -2.0f);
    glVertex3f(1.0f, -0.5f, -2.0f);
    glVertex3f(1.0f, -1.0f, -1.5f);
    glVertex3f(-1.0f, -1.0f, -1.5f);

    glVertex3f(-1.0f, -1.0f, 1.5f);
    glVertex3f(1.0f, -1.0f, 1.5f);
    glVertex3f(1.0f, -0.5f, 2.0f);
    glVertex3f(-1.0f, -0.5f, 2.0f);

    glColor3f(0.0f, 0.375f, 0.0f);

    glVertex3f(-1.0f, -1.0f, -1.5f);
    glVertex3f(-1.0f, -1.0f, 1.5f);
    glVertex3f(-1.0f, -0.5f, 2.0f);
    glVertex3f(-1.0f, -0.5f, -2.0f);

    glVertex3f(1.0f, -0.5f, -2.0f);
    glVertex3f(1.0f, -0.5f, 2.0f);
    glVertex3f(1.0f, -1.0f, 1.5f);
    glVertex3f(1.0f, -1.0f, -1.5f);

    glVertex3f(-1.0f, -1.0f, -1.5f);
    glVertex3f(-1.0f, -1.0f, 1.5f);
    glVertex3f(1.0f, -1.0f, 1.5f);
    glVertex3f(1.0f, -1.0f, -1.5f);

    // Chasis top.
    glColor3f(0.0f, 0.4375f, 0.0f);

    glVertex3f(-1.0f, 0.0f, -1.5f);
    glVertex3f(1.0f, 0.0f, -1.5f);
    glVertex3f(1.0f, -0.5f, -2.0f);
    glVertex3f(-1.0f, -0.5f, -2.0f);

    glVertex3f(-1.0f, -0.5f, 2.0f);
    glVertex3f(1.0f, -0.5f, 2.0f);
    glVertex3f(1.0f, 0.0f, 1.5f);
    glVertex3f(-1.0f, 0.0f, 1.5f);

    glVertex3f(-1.0f, -0.5f, -2.0f);
    glVertex3f(-1.0f, -0.5f, 2.0f);
    glVertex3f(-1.0f, 0.0f, 1.5f);
    glVertex3f(-1.0f, 0.0f, -1.5f);

    glVertex3f(1.0f, 0.0f, -1.5f);
    glVertex3f(1.0f, 0.0f, 1.5f);
    glVertex3f(1.0f, -0.5f, 2.0f);
    glVertex3f(1.0f, -0.5f, -2.0f);

    glVertex3f(-1.0f, 0.0f, -1.5f);
    glVertex3f(-1.0f, 0.0f, 1.5f);
    glVertex3f(1.0f, 0.0f, 1.5f);
    glVertex3f(1.0f, 0.0f, -1.5f);

    // Turret.
    glColor3f(0.0f, 0.5f, 0.0f);

    glVertex3d(-0.5f, 0.5f, -0.5f);
    glVertex3d(0.5f, 0.5f, -0.5f);
    glVertex3d(0.5f, 0.0f, -1.0f);
    glVertex3d(-0.5f, 0.0f, -1.0f);

    glVertex3d(-0.5f, 0.0f, 1.0f);
    glVertex3d(0.5f, 0.0f, 1.0f);
    glVertex3d(0.5f, 0.5f, 0.5f);
    glVertex3d(-0.5f, 0.5f, 0.5f);

    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.0f, -1.0f);
    glVertex3f(-0.5f, 0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.0f, 1.0f);
    glVertex3f(0.5f, 0.0f, -1.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    glColor3f(0.0f, 0.515625f, 0.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    // Barrel.
    glColor3f(0.1f, 0.1f, 0.1f);

    glVertex3f(-0.125f, 0.3f, -2.0f);
    glVertex3f(0.125f, 0.3f, -2.0f);
    glVertex3f(0.125f, 0.05f, -2.0f);
    glVertex3f(-0.125f, 0.05f, -2.0f);

    glColor3f(0.0f, 0.4375f, 0.0f);

    glVertex3f(-0.125f, 0.3f, -2.0f);
    glVertex3f(-0.125f, 0.05f, -2.0f);
    glVertex3f(-0.125f, 0.05f, 0.0f);
    glVertex3f(-0.125f, 0.3f, 0.0f);

    glVertex3f(0.125f, 0.3f, 0.0f);
    glVertex3f(0.125f, 0.05f, 0.0f);
    glVertex3f(0.125f, 0.05f, -2.0f);
    glVertex3f(0.125f, 0.3f, -2.0f);

    glVertex3f(-0.125f, 0.05f, -2.0f);
    glVertex3f(-0.125f, 0.05f, 0.0f);
    glVertex3f(0.125f, 0.05f, 0.0f);
    glVertex3f(0.125f, 0.05f, -2.0f);

    glVertex3f(-0.125f, 0.3f, -2.0f);
    glVertex3f(-0.125f, 0.3f, 0.0f);
    glVertex3f(0.125f, 0.3f, 0.0f);
    glVertex3f(0.125f, 0.3f, -2.0f);

    glEnd();

    glPopMatrix();
}

#endif /* TANKRENDERCOMPONENT_HPP_ */
