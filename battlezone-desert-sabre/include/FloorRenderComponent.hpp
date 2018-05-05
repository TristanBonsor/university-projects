/**
 * \file FloorRenderComponent.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef FLOORRENDERCOMPONENT_HPP_
#define FLOORRENDERCOMPONENT_HPP_

#include <GL/gl.h>

#include "RenderComponent.hpp"

class FloorRenderComponent: public RenderComponent
{
public:

    virtual ~FloorRenderComponent()
    {
    }

    virtual inline void render();

protected:

};

inline void FloorRenderComponent::render()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(m_position.x, m_position.y, m_position.z);

    glColor3f(0.9294f, 0.7882f, 0.6863f); //Desert sand.

    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-1024, -0.001, -1024);
    glVertex3f(-1024, -0.001, 1024);
    glVertex3f(1024, -0.001, 1024);
    glVertex3f(1024, -0.001, -1024);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.8294f, 0.6882f, 0.5863f);
    for(int i = -1024; i <= 1024; ++i)
    {
        glVertex3f(i, 0, -1024);
        glVertex3f(i, 0, 1024);
        glVertex3f(-1024, 0, i);
        glVertex3f(1024, 0, i);
    }
    glEnd();

    glPopMatrix();
}

#endif /* FLOORRENDERCOMPONENT_HPP_ */
