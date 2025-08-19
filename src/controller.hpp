#pragma once
#include "can_sim.hpp"
#include "pid.hpp"
#include "state_machine.hpp"
#include <atomic>
#include <thread>

// A simple controller that ties everything together.
class CoolingController {
public:
    CoolingController(CANBusSim& bus);

    // Start/stop the periodic control thread
    void start();
    void stop();

    // External inputs (can be set from tests or main)
    void set_ignition(bool on);
    void set_level_low(bool low);

    // Set setpoints
    void set_temperature_setpoint(double celsius);

private:
    void control_loop();

    CANBusSim& bus_;
    PID pid_;
    CoolingStateMachine sm_;
    std::atomic<bool> ignition_;
    std::atomic<bool> level_low_;
    std::atomic<double> temp_setpoint_;
    std::atomic<bool> running_;
    std::unique_ptr<std::thread> thread_;
    std::atomic<double> measured_temp_;   // used to simulate sensor
};
