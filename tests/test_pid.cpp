#include <gtest/gtest.h>
#include "pid.hpp"

TEST(PID, BasicStability) {
    PID pid(1.0, 0.5, 0.01, 0.0, 100.0);
    double sp = 50.0;
    double meas = 20.0;
    double out=0;
    for(int i=0;i<100;i++){
        out = pid.update(sp, meas, 0.1);
        // simulate plant responds a bit
        meas += out * 0.01;
    }
    EXPECT_GT(meas, 40.0);
    EXPECT_LE(out, 100.0);
}

TEST(PID, Reset) {
    PID pid(1.0, 0.5, 0.01);
    pid.update(10,0,0.1);
    pid.reset();
    double out = pid.update(10,0,0.1);
    EXPECT_GE(out, 0.0);
}
