#include "main.h"
#include "autons.hpp"
#include "subsystems.hpp"

/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// Chassis constructor
ez::Drive chassis(
    // These are your drive motors, the first motor is used for sensing!
    {-1, 2, -3, 4, -5},     // Left Chassis Ports (negative port will reverse it!)
    { 11, -12, 13, -14, 15},  // Right Chassis Ports (negative port will reverse it!)
    
    8,      // IMU Port
    2.75,  // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
    600);   // Wheel RPM

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  // Print our branding over your terminal :D
  ez::ez_template_print();

  pros::delay(500);  // Stop the user from doing anything while legacy ports configure

  // Configure your chassis controls
  chassis.opcontrol_curve_buttons_toggle(true);  // Enables modifying the controller curve with buttons on the joysticks
  chassis.opcontrol_drive_activebrake_set(2);    // Sets the active brake kP. We recommend ~2.  0 will disable.
  chassis.opcontrol_curve_default_set(3.5, 3.5);     // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)

  // Set the drive to your own constants from autons.cpp!
  default_constants();

  // These are already defaulted to these buttons, but you can change the left/right curve buttons here!
  // chassis.opcontrol_curve_buttons_left_set(pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT);  // If using tank, only the left side is used.
  // chassis.opcontrol_curve_buttons_right_set(pros::E_CONTROLLER_DIGITAL_Y, pros::E_CONTROLLER_DIGITAL_A);

  // Autonomous Selector using LLEMU
  ez::as::auton_selector.autons_add({
      Auton("Example Drive\n\nDrive forward and come back.", drive_example),
    //   Auton("Example Turn\n\nTurn 3 times.", turn_example),
    //   Auton("Drive and Turn\n\nDrive forward, turn, come back. ", drive_and_turn),
    //   Auton("Drive and Turn\n\nSlow down during drive.", wait_until_change_speed),
    //   Auton("Swing Example\n\nSwing in an 'S' curve", swing_example),
    //   Auton("Motion Chaining\n\nDrive forward, turn, and come back, but blend everything together :D", motion_chaining),
    //   Auton("Combine all 3 movements", combining_movements),
    //   Auton("Interference\n\nAfter driving forward, robot performs differently if interfered or not.", interfered_example),
  });

  // Initialize chassis and auton selector
  chassis.initialize();
  ez::as::initialize();
  master.rumble(".");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  // . . .
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  // . . .
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  chassis.pid_targets_reset();                // Resets PID targets to 0
  chassis.drive_imu_reset();                  // Reset gyro position to 0
  chassis.drive_sensor_reset();               // Reset drive sensors to 0
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD);  // Set motors to hold.  This helps autonomous consistency

  ez::as::auton_selector.selected_auton_call();  // Calls selected auton from autonomous selector
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  // This is preference to what you like to drive on
  pros::motor_brake_mode_e_t driver_preference_brake = MOTOR_BRAKE_BRAKE;

    chassis.drive_brake_set(driver_preference_brake);
    redirect1.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    redirect2.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

    intake.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    conveyor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);


    //for some reason it doesn't like using the ez namespace in subsystems.hpp
    ez::Piston clamp('B', true);
    bool clampState = true;

    bool color_sensing = false;
    bool redirect_sensing = false;

    int operating_speed = 127;


  while (true) {

    // chassis.opcontrol_tank();  // Tank control
    chassis.opcontrol_arcade_standard(ez::SPLIT);   // Standard split arcade
    // chassis.opcontrol_arcade_standard(ez::SINGLE);  // Standard single arcade
    // chassis.opcontrol_arcade_flipped(ez::SPLIT);    // Flipped split arcade
    // chassis.opcontrol_arcade_flipped(ez::SINGLE);   // Flipped single arcade


    //Intake + Conveyor
    if(master.get_digital(DIGITAL_R1)){
        intake.move(127);
        conveyor.move(operating_speed);
    }
    else if(master.get_digital(DIGITAL_R2)){
        intake.move(-127);
        conveyor.move(-127);
    }
    else{
        intake.brake();
        conveyor.brake();
    }

    // if(master.get_digital(DIGITAL_L1)){
    //     conveyor.move(127);
    // }
    // else if(master.get_digital(DIGITAL_L2)){
    //     conveyor.move(-127);
    // }
    // else{
    //     conveyor.brake();
    // }

    //Redirect
    if(master.get_digital(DIGITAL_UP)){
        redirect1.move(127);
        redirect2.move(127);
    }
    else if(master.get_digital(DIGITAL_DOWN)){
        redirect1.move(-127);
        redirect2.move(-127);
    }
    else{
        redirect1.brake();
        redirect2.brake();
    }

    //clamp
  if(master.get_digital_new_press(DIGITAL_X)){
    clampState = !clampState;
    clamp.set(clampState);
    master.rumble(". .");

  }


  double hue = color.get_hue();

  if(master.get_digital_new_press(DIGITAL_A)){
    redirect_sensing = !redirect_sensing;
    if(redirect_sensing) operating_speed = 80;
    else{
      operating_speed = 127;
    }
    master.rumble(". .");

  }

  if(master.get_digital_new_press(DIGITAL_Y)){
    color_sensing = !color_sensing;
  }

  /*Color Sorting:
  RED RING HUE: 5-15
  DEFAULT HUE: 30-50
  BLUE RING HUE: 160-220
  */

  // if(color_sensing){
  //   master.rumble(".");
  //   color.set_led_pwm(100);
  //   if(red_side){
  //     if(hue > 160 && hue < 220){
  //       pros::delay(300);
  //       conveyor.brake();
  //     }
  //   }
  //   else{
  //     if(hue > 0 && hue < 20){
  //       pros::delay(300);
  //       conveyor.brake();
  //     }
  //   }
  // }
  // else{
  //   color.set_led_pwm(0);
  // }


  if(redirect_sensing){
    color.set_led_pwm(100);      //turn on sensor LED (values 0-100)
    if(red_side){                      //for red rings
      if(hue > 0 && hue < 20){          //hue values for red
        pros::delay(120); //let the conveyor reach almost to the top
        conveyor.move(-80);   //reverse the conveyor
        pros::delay(300); //reverse for 300ms
        conveyor.brake();              //stop conveyor
      }
    }
    else{
      if(hue > 160 && hue < 220){
        pros::delay(120);
        conveyor.move(-80);
        pros::delay(300);
        conveyor.brake();
    }
    }
  }
  else{
    color.set_led_pwm(0);
  }

    pros::delay(ez::util::DELAY_TIME);  // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}