#pragma once

//#define EIGEN_HAS_CXX11_MATH 0
#include <Eigen/Dense>
#include <array>
#include "Geometry2d/Util.hpp"
//#include "const-math.hpp"

/// Model parameters for a robot.  Used by the controls system.
class RobotModel {
public:

    double V_Max = 18.5;

    /// Radius of omni-wheel (in meters)
    double WheelRadius;

    /// Distance from center of robot to center of wheel
    double WheelDist;

    Eigen::Matrix<double, 4, 4> A;
    Eigen::Matrix<double, 4, 4> B;
    Eigen::Matrix<double, 4, 4> PinvB;
    Eigen::Matrix<double, 4, 8> K;

    /// Wheel angles (in radians) measured between +x axis and wheel axle
    std::array<double, 4> WheelAngles;

    /// wheelSpeeds = BotToWheel * V_bot
    Eigen::Matrix<double, 4, 3> BotToWheel;

    /// botVelocity = WheelToBot * wheel_speeds
    Eigen::Matrix<double, 3, 4> WheelToBot;

    /// This should be called when any of the other parameters are changed
    void recalculateBotToWheel() {
        // See this paper for more info on how this matrix is derived:
        // http://people.idsia.ch/~foerster/2006/1/omnidrive_kiart_preprint.pdf

        // Factor WheelDist (R) into this matrix
        // clang-format off
        BotToWheel <<
            -sinf(WheelAngles[0]), cosf(WheelAngles[0]), WheelDist,
            -sinf(WheelAngles[1]), cosf(WheelAngles[1]), WheelDist,
            -sinf(WheelAngles[2]), cosf(WheelAngles[2]), WheelDist,
            -sinf(WheelAngles[3]), cosf(WheelAngles[3]), WheelDist;
        // Invert because our wheels spin opposite to paper
        BotToWheel *= -1;
        BotToWheel /= WheelRadius;
        // clang-format on

        WheelToBot = (BotToWheel.transpose() * BotToWheel).inverse() * BotToWheel.transpose();
    }

    float DutyCycleMultiplier = 2.0f;
};

/// Model parameters for robot.  See RobotModel.cpp for values.
extern const RobotModel RobotModelControl;
