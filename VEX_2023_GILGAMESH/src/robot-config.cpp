#include "vex.h"
#include "robot-config.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);

motor leftMotorA = motor(PORT11, ratio18_1, false);
motor leftMotorB = motor(PORT12, ratio18_1, false);
motor rightMotorA = motor(PORT17, ratio18_1, true);
motor rightMotorB = motor(PORT19, ratio18_1, true);

motor Intake = motor(PORT1, ratio18_1, false);
motor Roller = motor(PORT2, ratio18_1, false);
motor Shooter = motor(PORT15, ratio18_1, false);

motor_group LeftSide = motor_group(leftMotorA, leftMotorB);
motor_group RightSide = motor_group(rightMotorA, rightMotorB);
drivetrain Drivetrain = drivetrain(LeftSide, RightSide, 319.19, 295, 40, mm, 1);

// VEXcode generated funawdaaawns
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool Controller1LeftShoulderControlMotorsStopped = true;
bool Controller1RightShoulderControlMotorsStopped = true;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {

      // potential issue is they are not linked to the correct motors, so i will just
      // disconnect three motors and see which one spins
      int frontLeftWheel = (Controller1.Axis3.position() + Controller1.Axis4.position() + Controller1.Axis1.position());
      int frontRightWheel = (Controller1.Axis3.position() - Controller1.Axis4.position() - Controller1.Axis1.position());
      int backLeftWheel = (Controller1.Axis3.position() - Controller1.Axis4.position() + Controller1.Axis1.position());
      int backRightWheel = (Controller1.Axis3.position() + Controller1.Axis4.position() - Controller1.Axis1.position());
      
      leftMotorA.setVelocity(frontLeftWheel, percent);
      leftMotorB.setVelocity(backLeftWheel, percent);
      rightMotorA.setVelocity(frontRightWheel, percent);
      rightMotorB.setVelocity(backRightWheel, percent);

      leftMotorA.spin(forward);
      leftMotorB.spin(forward);
      rightMotorA.spin(forward);
      rightMotorB.spin(forward);

      // check the ButtonL1/ButtonL2 status to control Motor13
      if (Controller1.ButtonL2.pressing()) {
        Intake.setVelocity(70, percent);
        Intake.spin(forward);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonL1.pressing()) {
        Intake.setVelocity(70, percent);
        Intake.spin(reverse);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (!Controller1LeftShoulderControlMotorsStopped) {
        Intake.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1LeftShoulderControlMotorsStopped = true;
      }
      // check the ButtonR1/ButtonR2 status to control Motor14
      if (Controller1.ButtonR2.pressing()) {
        Roller.setVelocity(70, percent);
        Roller.spin(forward);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonR1.pressing()) {
        Roller.spin(reverse);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (!Controller1RightShoulderControlMotorsStopped) {
        Roller.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1RightShoulderControlMotorsStopped = true;
      }
      //Brake
      if (Controller1.ButtonA.pressing()) {
        leftMotorA.stop();
        leftMotorB.stop();
        rightMotorA.stop();
        rightMotorB.stop();
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);
}