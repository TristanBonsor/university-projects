/*
 * \file Controller.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include <chrono>

#include <GL/freeglut.h>
#include <GL/glut.h>

#include "Model.hpp"
#include "View.hpp"

#define MILLI_TO_SECOND 0.001f

class Controller
{
public:

    inline static void init(int argc, char** argv);

    inline static void run();

protected:

    inline static void onKeyDown(unsigned char key, int x, int y);

    inline static void onKeyUp(unsigned char key, int x, int y);

    inline static void onDisplay();

    inline static void onIdle();

    static const std::chrono::milliseconds M_DT;
    static const std::chrono::milliseconds M_MAX_FRAME_TIME;

    static std::chrono::milliseconds m_t;
    static std::chrono::milliseconds m_accumulator;
    static std::chrono::milliseconds m_physics_interpolation_alpha;

    static std::shared_ptr<Model> m_model;
    static std::shared_ptr<View> m_view;
};

inline void Controller::init(int argc, char** argv)
{
    glutInit(&argc, argv);

    m_model = std::shared_ptr<Model>(new Model());
    m_view = std::shared_ptr<View>(new View());

    m_model->init();
    m_view->init();

    glutDisplayFunc(onDisplay);
    //glutIdleFunc(onIdle);

    glutKeyboardFunc(onKeyDown);
    glutKeyboardUpFunc(onKeyUp);
}

inline void Controller::run()
{
    auto init_duration = std::chrono::high_resolution_clock::now().time_since_epoch();

    auto current_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(init_duration);

    while(1)
    {
        glutMainLoopEvent();

        auto new_duration = std::chrono::high_resolution_clock::now().time_since_epoch();

        auto new_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(new_duration);

        auto frame_time = new_time - current_time;

        if(frame_time > M_MAX_FRAME_TIME)
        {
            frame_time = M_MAX_FRAME_TIME;
        }

        current_time = new_time;

        m_accumulator += frame_time;

        while(m_accumulator >= M_DT)
        {
            m_model->update(m_t.count() * MILLI_TO_SECOND,
                            M_DT.count() * MILLI_TO_SECOND);
            m_t += M_DT;
            m_accumulator -= M_DT;
        }

        m_physics_interpolation_alpha = std::chrono::milliseconds
        { m_accumulator / M_DT };

        glutPostRedisplay();
    }
}

inline void Controller::onKeyDown(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'a':
            // Left tread forward.
            m_model->getPlayerControl()->leftTreadForward();
            break;
        case 'z':
            // Left tread backward.
            m_model->getPlayerControl()->leftTreadBackward();
            break;
        case '\'':
            // Right tread forward.
            m_model->getPlayerControl()->rightTreadForward();
            break;
        case '/':
            // Right tread backward.
            m_model->getPlayerControl()->rightTreadBackward();
            break;
        case ' ':
            // Shoot.
            m_model->getPlayerControl()->isShotDesired(true);
            break;
    }
}

inline void Controller::onKeyUp(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'a':
        case 'z':
            // Left tread.
            m_model->getPlayerControl()->leftTreadStop();
            break;
        case '\'':
        case '/':
            // Right tread.
            m_model->getPlayerControl()->rightTreadStop();
            break;
        case ' ':
            // Shoot.
            m_model->getPlayerControl()->isShotDesired(false);
            break;
    }
}

inline void Controller::onDisplay()
{
    m_view->render(m_model,
                   m_physics_interpolation_alpha.count() * MILLI_TO_SECOND);
}

/*inline void Controller::onIdle()
 {

 }*/

#endif /* CONTROLLER_HPP_ */
