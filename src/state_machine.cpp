#include "state_machine.hpp"

CoolingStateMachine::CoolingStateMachine() : state_(CoolingState::IDLE), last_fault_("") {}

CoolingState CoolingStateMachine::state() const { return state_; }

void CoolingStateMachine::on_ignition_on() {
    if(state_ == CoolingState::IDLE) {
        state_ = CoolingState::PREPARE;
    }
}

void CoolingStateMachine::on_ignition_off() {
    if(state_ == CoolingState::RUNNING || state_ == CoolingState::PREPARE) {
        state_ = CoolingState::SHUTDOWN;
    } else if(state_ == CoolingState::FAULT) {
        state_ = CoolingState::SHUTDOWN;
    } else {
        state_ = CoolingState::IDLE;
    }
}

void CoolingStateMachine::on_temp_ok() {
    if(state_ == CoolingState::PREPARE) state_ = CoolingState::RUNNING;
    if(state_ == CoolingState::RUNNING) {/* remain */}
}

void CoolingStateMachine::on_temp_high() {
    if(state_ == CoolingState::RUNNING) state_ = CoolingState::FAULT;
}

void CoolingStateMachine::on_fault(const std::string& reason) {
    last_fault_ = reason;
    state_ = CoolingState::FAULT;
}

void CoolingStateMachine::clear_fault() {
    if(state_ == CoolingState::FAULT) state_ = CoolingState::SHUTDOWN;
}
