#pragma once

#include "api.h"

// Your motors, sensors, etc. should go here.  Below are examples

// inline pros::Motor intake(1);
// inline pros::adi::DigitalIn limit_switch('A');

inline pros::Motor intake(-19);
inline pros::Motor conveyor(6);

inline pros::MotorGroup redirect({-7, 20}, pros::v5::MotorGears::red, pros::v5::MotorUnits::degrees);
// inline pros::Rotation encoder(16);

