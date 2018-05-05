/*
 * Controller.cc
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#include "Controller.hpp"

const std::chrono::milliseconds Controller::M_DT{10};
const std::chrono::milliseconds Controller::M_MAX_FRAME_TIME{250};

std::chrono::milliseconds Controller::m_t{0};
std::chrono::milliseconds Controller::m_accumulator{0};
std::chrono::milliseconds Controller::m_physics_interpolation_alpha{0};

std::shared_ptr<Model> Controller::m_model = nullptr;
std::shared_ptr<View> Controller::m_view = nullptr;
