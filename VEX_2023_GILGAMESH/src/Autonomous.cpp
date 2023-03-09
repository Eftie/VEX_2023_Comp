#include "vex.h"
#include "robot-config.h"

using namespace vex;

/* DIAGRAM SOURCE:
https://en.wikipedia.org/wiki/Mecanum_wheel#/media/File:Mecanum_wheel_control_principle.svg
ALL METHODS ARE DERIVED OFF OF THIS IMAGE */

class Autonomous {
  public:

    // how many degrees per inch in a 4" wheel
    float x;
    float inchPerDegree = (3.1415926535 * x) / 360;

    void Move_Y(short inchAmt) {

      Drivetrain.driveFor(inchAmt, inches);

      Drivetrain.stop();

      leftMotorA.resetPosition();
      rightMotorA.resetPosition();
      leftMotorB.resetPosition();
      rightMotorB.resetPosition();
    }

    /**
    @brief Move left to right
    @param inchAmt The amount of inches you want to drive in that direction
    @param left If true, you will drive left. False is right
    */
    void Move_X(short inchAmt, bool left) {

      this->x = 4.0;
      int runAmt;
      if (fabs(this->inchPerDegree) > 360) {
        runAmt = Move_X(this->inchPerDegree, 1);
      }

      if (left) {
        LeftSide.spinTo(inchAmt / this->inchPerDegree, degrees);
        RightSide.spinTo(inchAmt / -this->inchPerDegree, degrees);

      } else {

        LeftSide.spinTo(inchAmt / -this->inchPerDegree, degrees);
        RightSide.spinTo(inchAmt / this->inchPerDegree, degrees);
      }

      leftMotorA.stop();
      rightMotorA.stop();
      leftMotorB.stop();
      rightMotorB.stop();

      leftMotorA.resetPosition();
      rightMotorA.resetPosition();
      leftMotorB.resetPosition();
      rightMotorB.resetPosition();
    }

    /** 
    @brief Whenever the degreeAmt is over 360, this is called to tell how many times the motor needs to complete a full cycle
    @return Returns a short, runAmt, which is the amount of times the motor needs to do full cycles
    @param degreeAmt Whenever the amount of degrees the motor needs to rotate - gained by translating inches to degrees
    @param runAmt Must start at 1, will then increase the more AutoMove_X is called
    */
    short Move_X(float degreeAmt, short runAmt) {
      if (degreeAmt > 360) {
        degreeAmt -= 360;
        runAmt += 1;

        return Move_X(degreeAmt, runAmt);
        
      } else if (degreeAmt < -360) {
        degreeAmt += 360;
        runAmt += 1;

        return Move_X(degreeAmt, runAmt);
      }

      return runAmt;
    }

    /**
    @brief Allows you to move diagonally
    @param inchAmt The amount of inches you want to drive in that direction 
    @param reverse If you want to move backwards
    @param left If true, you will drive left. False is right
    */
    void Move_XY(short inchAmt, bool reverse, bool left) {

      this->x = 4.0;

      if (!reverse && left) {

        leftMotorA.spinTo(inchAmt / this->inchPerDegree, degrees);
        rightMotorB.spinTo(inchAmt / this->inchPerDegree, degrees);

        leftMotorA.stop();
        rightMotorB.stop();

        leftMotorA.resetPosition();
        rightMotorB.resetPosition();
      } if (!reverse && !left) {

        rightMotorA.spinTo(inchAmt / this->inchPerDegree, degrees);
        leftMotorB.spinTo(inchAmt / this->inchPerDegree, degrees);

        rightMotorA.stop();
        leftMotorB.stop();

        rightMotorA.resetPosition();
        leftMotorB.resetPosition();
      }
      
      if (reverse && left) {

        rightMotorA.spinTo(inchAmt / -this->inchPerDegree, degrees);
        leftMotorB.spinTo(inchAmt / -this->inchPerDegree, degrees);

        rightMotorA.stop();
        leftMotorB.stop();

        rightMotorA.resetPosition();
        leftMotorB.resetPosition();
      }

      if (reverse && !left) {
        leftMotorA.spinTo(inchAmt / -this->inchPerDegree, degrees);
        rightMotorB.spinTo(inchAmt / -this->inchPerDegree, degrees);

        leftMotorA.stop();
        rightMotorB.stop();

        rightMotorA.resetPosition();
        leftMotorB.resetPosition();
      }
    }

    /**
    @brief Runs the intake for the given period of time (seconds)
    @param time However many seconds you want the intake to spin, will automatically default to 1.5s
    */
    void RunIntake(float time = 1.5) {
      Intake.spinFor(fwd, time, seconds);
    }

    /**
    @brief Runs the intake through the given amount of volts
    @param voltAmt The amount of volts given to run the intake
    */
    void SetIntake(float voltAmt) {
      Intake.spin(fwd, voltAmt, volt);
    }

    /**
    @brief Stops the intake
    */
    void StopIntake() {
      Intake.stop();
    }

    /**
    @brief Runs the shoot for the given period of time (seconds)
    @param time However many seconds you want the intake to spin, will automatically default to 1.5s
    */
    void RunShooter(float time = 1.5) {
      Shooter.spinFor(fwd, time, seconds);
    }

    /**
    @brief Runs the shooter through the given amount of volts
    @param voltAmt The amount of volts given to run the shooter
    */
    void SetShooter(float voltAmt) {
      Intake.spin(fwd, voltAmt, volt);
    }

    /**
    @brief Stops the shooter
    */
    void StopShooter() {
      Intake.stop();
    }

    /**
    @brief Allows you to rotate your robot (different from pivot/spin)
    @param degreeAmt However many degrees you want to rotate
    @param left If true, you will rotate left. False is right 
    */
    void Rotate(float degreeAmt, bool left) {

      this->x = 4.0;

      if (!left) {

        LeftSide.spinTo(this->inchPerDegree * degreeAmt, degrees);
        LeftSide.stop();
        LeftSide.resetPosition();
      } else {

        RightSide.spinTo(this->inchPerDegree * degreeAmt, degrees);
        RightSide.stop();
        RightSide.resetPosition();
      }
      // the math might be wrong because that shit came to me in a dream
      // im being dead serious
    }

    /**
    @brief Allows you to pivot  your robot (different from rotate/spin)
    @param degreeAmt However many degrees you want to rotate
    @param left If true, you will rotate left. False is right 
    */
    void Pivot(float degreeAmt, bool left) {

      this->x = 4.0;

      if (!left) {
          leftMotorA.spinTo(this->inchPerDegree * degreeAmt, degrees);
          rightMotorA.spinTo(-this->inchPerDegree * degreeAmt, degrees);
      } else {
          leftMotorA.spinTo(-this->inchPerDegree * degreeAmt, degrees);
          rightMotorA.spinTo(this->inchPerDegree * degreeAmt, degrees);
      }

      leftMotorA.stop();
      rightMotorA.stop();
      leftMotorA.resetPosition();
      rightMotorA.resetPosition();
    }

    /**
    @brief Allows you to spin your robot (different from rotate/pivot)
    @param degreeAmt However many degrees you want to rotate
    @param counter If true, you will spin left. False is right 
    */
    void RotateClockwise(float degreeAmt, bool counter) {

      this->x = 4.0;

      if (!counter) {
        leftMotorA.spinTo(this->inchPerDegree * degreeAmt, degrees);
        rightMotorA.spinTo(this->inchPerDegree * degreeAmt, degrees);
        leftMotorB.spinTo(-this->inchPerDegree * degreeAmt, degrees);
        rightMotorB.spinTo(-this->inchPerDegree * degreeAmt, degrees);
      } else {

        leftMotorA.spinTo(-this->inchPerDegree * degreeAmt, degrees);
        rightMotorA.spinTo(-this->inchPerDegree * degreeAmt, degrees);
        leftMotorB.spinTo(this->inchPerDegree * degreeAmt, degrees);
        rightMotorB.spinTo(this->inchPerDegree * degreeAmt, degrees);
      }

      //(!counter) ? FrontLeftWheel.spinTo(inchPerDegree * degreeAmt, degrees), BackLeftWheel.spinTo(inchPerDegree * degreeAmt, degrees), FrontRightWheel.spinTo(-inchPerDegree * degreeAmt, degrees), BackRightWheel.spinTo(-inchPerDegree * degreeAmt, degrees) : FrontLeftWheel.spinTo(-inchPerDegree * degreeAmt, degrees), BackLeftWheel.spinTo(-inchPerDegree * degreeAmt, degrees), FrontRightWheel.spinTo(inchPerDegree * degreeAmt, degrees), BackRightWheel.spinTo(inchPerDegree * degreeAmt, degrees);

      leftMotorA.stop();
      rightMotorA.stop();
      leftMotorB.stop();
      rightMotorB.stop();

      leftMotorA.resetPosition();
      rightMotorA.resetPosition();
      leftMotorB.resetPosition();
      rightMotorB.resetPosition();
    }
};