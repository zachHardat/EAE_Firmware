#include "pid.hpp"
#include <algorithm>
#include <cmath>

PID::PID(double kp, double ki, double kd, double out_min, double out_max)
: kp_(kp), ki_(ki), kd_(kd), integrator_(0.0), prev_error_(0.0),
  out_min_(out_min), out_max_(out_max) {}

double PID::update(double setpoint, double measurement, double dt_seconds) {
    if (dt_seconds <= 0.0) return 0.0;
    double error = setpoint - measurement;
    integrator_ += error * dt_seconds;
    double derivative = (error - prev_error_) / dt_seconds;
    double out = kp_ * error + ki_ * integrator_ + kd_ * derivative;

    // clamp output
    out = std::max(out_min_, std::min(out_max_, out));
    prev_error_ = error;
    return out;
}

void PID::reset() {
    integrator_ = 0.0;
    prev_error_ = 0.0;
}
