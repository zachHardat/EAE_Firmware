#pragma once
#include <string>

enum class CoolingState {
    IDLE,
    PREPARE,
    RUNNING,
    FAULT,
    SHUTDOWN
};

class CoolingStateMachine {
public:
    CoolingStateMachine();
    CoolingState state() const;

    // feed events
    void on_ignition_on();
    void on_ignition_off();
    void on_temp_ok();
    void on_temp_high();
    void on_fault(const std::string& reason);
    void clear_fault();

private:
    CoolingState state_;
    std::string last_fault_;
};
