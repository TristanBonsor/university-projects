/**
 * \file ShellRenderComponent.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef SHELLRENDERCOMPONENT_HPP_
#define SHELLRENDERCOMPONENT_HPP_

#include <GL/gl.h>

#include "RenderComponent.hpp"

class ShellRenderComponent: public RenderComponent
{
public:

    virtual ~ShellRenderComponent()
    {
    }

    virtual inline void render();

protected:

};

inline void ShellRenderComponent::render()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(m_position.x, m_position.y, m_position.z);
    glRotatef(m_angle / 3.14159f * 180.0f, m_axis.x, m_axis.y, m_axis.z);

    glColor3f(0.3f, 0.3f, 0.3f); //Desert sand.

    const float s = 0.125f;

    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(-s, -s, +s);
    glVertex3f(+s, -s, +s);
    glVertex3f(+s, +s, +s);
    glVertex3f(-s, +s, +s);

    glNormal3f(0, 0, -1);
    glVertex3f(-s, -s, -s);
    glVertex3f(-s, +s, -s);
    glVertex3f(+s, +s, -s);
    glVertex3f(+s, -s, -s);

    glNormal3f(0, +1, 0);
    glVertex3f(-s, +s, -s);
    glVertex3f(-s, +s, +s);
    glVertex3f(+s, +s, +s);
    glVertex3f(+s, +s, -s);

    glNormal3f(0, -1, 0);
    glVertex3f(-s, -s, -s);
    glVertex3f(+s, -s, -s);
    glVertex3f(+s, -s, +s);
    glVertex3f(-s, -s, +s);

    glNormal3f(+1, 0, 0);
    glVertex3f(+s, -s, -s);
    glVertex3f(+s, +s, -s);
    glVertex3f(+s, +s, +s);
    glVertex3f(+s, -s, +s);

    glNormal3f(-1, 0, 0);
    glVertex3f(-s, -s, -s);
    glVertex3f(-s, -s, +s);
    glVertex3f(-s, +s, +s);
    glVertex3f(-s, +s, -s);

    glEnd();

    glPopMatrix();
}

#endif /* SHELLRENDERCOMPONENT_HPP_ */
