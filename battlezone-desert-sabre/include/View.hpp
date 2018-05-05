/*
 * \file View.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef VIEW_HPP_
#define VIEW_HPP_

#include <iostream>

#include <GL/glut.h>

#include "Camera.hpp"
#include "Model.hpp"

#define WINDOW_TITLE "Desert Sabre"
#define DEFAULT_WINDOW_X 100
#define DEFAULT_WINDOW_Y 0
#define DEFAULT_WINDOW_WIDTH 700 //512
#define DEFAULT_WINDOW_HEIGHT 700 //512
#define DEFAULT_NEAR_CLIP 0.1f
#define DEFAULT_FAR_CLIP 512.0f

#define DEFAULT_PLAYER_FOV_Y 53.0f

#define CONSOLE_HELP "CONTROLS\n========\nLeft Tread:  'a'-forward, 'z'-backward\nRight Tread: '''-forward, '/'-backward\nFire:        Space bar\n"

class View
{
public:

    View(const int& window_x = DEFAULT_WINDOW_X,
         const int& window_y = DEFAULT_WINDOW_Y,
         const int& window_width = DEFAULT_WINDOW_WIDTH,
         const int& window_height = DEFAULT_WINDOW_HEIGHT)
        : m_window_x(window_x),
          m_window_y(window_y),
          m_window_width(window_width),
          m_window_height(window_height)
    {
    }

    inline void init();

    inline void render(const std::shared_ptr<Model>& model, const float& alpha =
                           0.0f);

protected:

    int m_window_x;
    int m_window_y;
    int m_window_width;
    int m_window_height;
};

inline void View::init()
{
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowPosition(m_window_x, m_window_y);
    glutInitWindowSize(m_window_width, m_window_height);
    glutCreateWindow(WINDOW_TITLE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    std::cout << CONSOLE_HELP;
}

inline void View::render(const std::shared_ptr<Model>& model,
                         const float& alpha)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_window_x = glutGet(GLUT_WINDOW_X);
    m_window_y = glutGet(GLUT_WINDOW_Y);
    m_window_width = glutGet(GLUT_WINDOW_WIDTH);
    m_window_height = glutGet(GLUT_WINDOW_HEIGHT);

    glViewport(0, 0, m_window_width, m_window_height);

    float angle;
    Vector3d axis;

    // Set up camera for first-person, player perspective.

    PhysicsComponent::PhysicsState state = model->getPlayerPhysicsState(alpha);
    state.orientation().angleAxis(angle, axis);
    axis.normalize();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(DEFAULT_PLAYER_FOV_Y,
                   m_window_width / m_window_height,
                   DEFAULT_NEAR_CLIP,
                   DEFAULT_FAR_CLIP);

    Camera::translateTo(state.position());
    Camera::rotateTo(angle, axis);
    Camera::display();

    glClearColor(0.5294f, 0.8078f, 0.9216f, 1); // Sky blue.

    // Render for first-person, player perspective.
    model->render(alpha);

    // Render map.

    glViewport(m_window_width * 0.3f,
               m_window_height * 0.0f,
               m_window_width * 0.4f,
               m_window_height * 0.4f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glClear(GL_DEPTH_BUFFER_BIT);

    const float scale = 2.0f;

    gluLookAt(scale * state.position().x,
              state.position().y + 512.0f,
              scale * state.position().z,
              scale * state.position().x,
              state.position().y,
              scale * state.position().z,
              0.0f,
              0.0f,
              -1.0f);

    glScalef(scale, scale, scale);

    model->render(alpha);

    glPopMatrix();

    // Render target reticle.

    glViewport(0, 0, m_window_width, m_window_height);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT);

    const float s = 0.125f;
    const float m = 0.09375f;

    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-s, s);
    glVertex2f(-s, -s);
    glVertex2f(s, s);
    glVertex2f(s, -s);

    glVertex2f(-s, s);
    glVertex2f(-m, s);

    glVertex2f(-s, -s);
    glVertex2f(-m, -s);

    glVertex2f(s, s);
    glVertex2f(m, s);

    glVertex2f(s, -s);
    glVertex2f(m, -s);

    // Map border.
    glColor3f(0.8294f, 0.6882f, 0.5863f);
    glVertex2f(-0.4f, -1.0f);
    glVertex2f(0.4f, -1.0f);

    glVertex2f(0.4f, -1.0f);
    glVertex2f(0.4f, -0.2f);

    glVertex2f(0.4f, -0.2f);
    glVertex2f(-0.4f, -0.2f);

    glVertex2f(-0.4f, -0.2f);
    glVertex2f(-0.4f, -1.0f);
    glEnd();

    if(model->isPlayerDead())
    {
        // Death overlay.
        glBegin(GL_QUADS);
        glColor4f(0.5412f, 0.0275f, 0.0275f, 0.75f);
        glVertex2f(-1.0f, 1.0f);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(1.0f, 1.0f);
        glEnd();
    }

    glEnable(GL_CULL_FACE);

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glutSwapBuffers();
}

#endif /* VIEW_HPP_ */
