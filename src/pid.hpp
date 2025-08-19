#pragma once
#include <chrono>

class PID {
public:
    PID(double kp, double ki, double kd, double out_min = 0.0, double out_max = 100.0);

    // Call every loop with current measurement, returns control output.
    double update(double setpoint, double measurement, double dt_seconds);

    void reset();

private:
    double kp_;
    double ki_;
    double kd_;
    double integrator_;
    double prev_error_;
    double out_min_;
    double out_max_;
};
