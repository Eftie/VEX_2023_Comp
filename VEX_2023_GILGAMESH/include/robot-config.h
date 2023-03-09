#ifndef ROBOT_CONFIG_H_
#define ROBOT_CONFIG_H_

using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;

extern motor leftMotorA;
extern motor leftMotorB;
extern motor rightMotorA;
extern motor rightMotorB;

extern motor Intake;
extern motor Roller;
extern motor Shooter;

extern motor_group LeftSide;
extern motor_group RightSide;
extern drivetrain Drivetrain;

#endif

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );