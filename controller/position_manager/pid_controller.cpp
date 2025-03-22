#include "pid_controller.hpp"

PIDController::PIDController(double kp, double ki, double kd)
    : kp_(kp), ki_(ki), kd_(kd), integral_(0.0), prev_error_(0.0) {}

double PIDController::calculate(double setpoint, double current, double dt) {
    double error = setpoint - current;
    integral_ += dt * error;

    double derivative = (dt > 0) ? (error - prev_error_) / dt : 0.0;

    prev_error_ = error;

    return (kp_ * error) + (ki_ * integral_)  + (kd_ * derivative);
}

void PIDController::resetPID() {
    integral_ = 0.0;
    prev_error_ = 0.0;
}