#pragma once

#include "api.h"
// #include "pros/optical.hpp"

// Your motors, sensors, etc. should go here.  Below are examples

// inline pros::Motor intake(1);
// inline pros::adi::DigitalIn limit_switch('A');

inline pros::Motor conveyor(19);
inline pros::Motor intake(6);

// inline pros::MotorGroup redirect({20, -7}, pros::v5::MotorGears::red, pros::v5::MotorUnits::degrees);
inline pros::Motor redirect1(-7);
inline pros::Motor redirect2(20);


// inline pros::Rotation encoder(16);
inline pros::Optical color(18);

