#include <gtest/gtest.h>
#include "state_machine.hpp"

TEST(StateMachine, Transitions) {
    CoolingStateMachine sm;
    EXPECT_EQ(sm.state(), CoolingState::IDLE);
    sm.on_ignition_on();
    EXPECT_EQ(sm.state(), CoolingState::PREPARE);
    sm.on_temp_ok();
    EXPECT_EQ(sm.state(), CoolingState::RUNNING);
    sm.on_temp_high();
    EXPECT_EQ(sm.state(), CoolingState::FAULT);
    sm.clear_fault();
    EXPECT_EQ(sm.state(), CoolingState::SHUTDOWN);
    sm.on_ignition_off();
    EXPECT_EQ(sm.state(), CoolingState::IDLE);
}
