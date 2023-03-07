#include "vex.h"
#include "robot-config.cpp"

using namespace vex;

/* DIAGRAM SOURCE:
https://en.wikipedia.org/wiki/Mecanum_wheel#/media/File:Mecanum_wheel_control_principle.svg
ALL METHODS ARE DERIVED OFF OF THIS IMAGE */

class Autonomous {
public:
    // calculates how to get the robot to move forward
    // inchAmt is the set amount of inches for robot to move
    void AutoMove_Y(short inchAmt) {

        float inchPerDegree = (3.1415926535 * 4.0) / 360;

        Drivetrain.driveFor(inchAmt * inchPerDegree, inches);

        Drivetrain.stop();

        leftMotorA.resetPosition();
        rightMotorA.resetPosition();
        leftMotorB.resetPosition();
        rightMotorB.resetPosition();
    }

    // moves left and right
    // if reverse is true, motors spin backwards
    void AutoMove_X(short inchAmt, bool left) {

        float inchPerDegree = (3.1415926535 * 4.0) / 360;

        if (left)
        {
            leftMotorA.spinTo(inchAmt / inchPerDegree, degrees);
            rightMotorA.spinTo(inchAmt / -inchPerDegree, degrees);
            leftMotorB.spinTo(inchAmt / -inchPerDegree, degrees);
            rightMotorB.spinTo(inchAmt / inchPerDegree, degrees);
        } else {

            leftMotorA.spinTo(inchAmt / -inchPerDegree, degrees);
            rightMotorA.spinTo(inchAmt / inchPerDegree, degrees);
            leftMotorB.spinTo(inchAmt / inchPerDegree, degrees);
            rightMotorB.spinTo(inchAmt / -inchPerDegree, degrees);
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

    // moves diagonally
    // if reverse is true, motors spin backwards, if left is true, you drift left
    void AutoMove_XY(short inchAmt, bool reverse, bool left) {

        float inchPerDegree = (3.1415926535 * 4.0) / 360;

        if (!reverse && left) {

            leftMotorA.spinTo(inchAmt / inchPerDegree, degrees);
            rightMotorB.spinTo(inchAmt / inchPerDegree, degrees);

            leftMotorA.stop();
            rightMotorB.stop();

            leftMotorA.resetPosition();
            rightMotorB.resetPosition();
        } if (!reverse && !left) {

            rightMotorA.spinTo(inchAmt / inchPerDegree, degrees);
            leftMotorB.spinTo(inchAmt / inchPerDegree, degrees);

            rightMotorA.stop();
            leftMotorB.stop();

            rightMotorA.resetPosition();
            leftMotorB.resetPosition();
        }
        
        if (reverse && left) {

            rightMotorA.spinTo(inchAmt / -inchPerDegree, degrees);
            leftMotorB.spinTo(inchAmt / -inchPerDegree, degrees);

            rightMotorA.stop();
            leftMotorB.stop();

            rightMotorA.resetPosition();
            leftMotorB.resetPosition();
        }

        if (reverse && !left) {
            leftMotorA.spinTo(inchAmt / -inchPerDegree, degrees);
            rightMotorB.spinTo(inchAmt / -inchPerDegree, degrees);

            leftMotorA.stop();
            rightMotorB.stop();

            rightMotorA.resetPosition();
            leftMotorB.resetPosition();
        }
    }

    // param is how long you want it to run, default is 1.5 seconds
    void AutoRunIntake(float time = 1.5) {
        Intake.spinFor(fwd, time, seconds);
    }

    // param is how long you want it to run, default is 1.5 seconds
    void AutoRunShooter(float time = 1.5) {
        Shooter.spinFor(fwd, time, seconds);
    }

    /* allows you to rotate the robot, pivot position is on the sides
    degreeAmt is how many degrees you're trying to rotate to, if left is false then you go right
    rotate right: front left forward, back left forward
    rotate left: front right forward, back right forward */
    void AutoRotate(float degreeAmt, bool left) {

        float inchPerDegree = (3.1415926535 * 4.0) / 360;

        if (!left) {

            LeftSide.spinTo(inchPerDegree * degreeAmt, degrees);
            LeftSide.stop();
            LeftSide.resetPosition();
        } else {

            RightSide.spinTo(inchPerDegree * degreeAmt, degrees);
            RightSide.stop();
            RightSide.resetPosition();
        }
        // the math might be wrong because that shit came to me in a dream
        // im being dead serious
    }

    /* allows you to pivot the robot, pivot position is front and back
    degreeAmt is how many degrees you're trying to rotate to, if left is false then you go right
    pivot right: front left forward, front right reverse
    pivot left: front left reverse, front right forward */
    void AutoPivot(float degreeAmt, bool left) {

        float inchPerDegree = (3.1415926535 * 4.0) / 360;

        if (!left) {
            leftMotorA.spinTo(inchPerDegree * degreeAmt, degrees);
            rightMotorA.spinTo(-inchPerDegree * degreeAmt, degrees);
        } else {
            leftMotorA.spinTo(-inchPerDegree * degreeAmt, degrees);
            rightMotorA.spinTo(inchPerDegree * degreeAmt, degrees);
        }

        leftMotorA.stop();
        rightMotorA.stop();
        leftMotorA.resetPosition();
        rightMotorA.resetPosition();
    }

    /* allows you to spin the robot, pivot position is is centered
    degreeAmt is how many degrees you're trying to rotate to, if left is false then you go right
    clockwise:
    left side forward, right side reverse
    counter-clockwise: left side reverse, right side forward */
    void AutoRotateClockwise(float degreeAmt, bool counter) {

        float inchPerDegree = (3.1415926535 * 4.0) / 360;

        if (!counter) {
            leftMotorA.spinTo(inchPerDegree * degreeAmt, degrees);
            rightMotorA.spinTo(inchPerDegree * degreeAmt, degrees);
            leftMotorB.spinTo(-inchPerDegree * degreeAmt, degrees);
            rightMotorB.spinTo(-inchPerDegree * degreeAmt, degrees);
        } else {

            leftMotorA.spinTo(-inchPerDegree * degreeAmt, degrees);
            rightMotorA.spinTo(-inchPerDegree * degreeAmt, degrees);
            leftMotorB.spinTo(inchPerDegree * degreeAmt, degrees);
            rightMotorB.spinTo(inchPerDegree * degreeAmt, degrees);
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