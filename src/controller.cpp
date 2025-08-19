#include "controller.hpp"
#include <iostream>
#include <chrono>
#include <cmath>

CoolingController::CoolingController(CANBusSim& bus)
: bus_(bus),
  pid_(2.0, 0.5, 0.05, 0.0, 100.0),
  ignition_(false),
  level_low_(false),
  temp_setpoint_(40.0),
  running_(false),
  measured_temp_(25.0)
{
    // subscribe to CAN messages (pump status) as example
    bus_.register_listener([this](const CANMessage &m){
        // Example: we parse pump rpm message id = 0x100 with 2 bytes rpm
        if(m.id == 0x100 && m.dlc >= 2) {
            int rpm = (m.data[0] << 8) | m.data[1];
            // use rpm to potentially detect fault (simple)
            if(rpm == 0) {
                sm_.on_fault("pump_stopped");
            }
        }
    });
}

void CoolingController::start() {
    running_ = true;
    thread_ = std::make_unique<std::thread>(&CoolingController::control_loop, this);
}

void CoolingController::stop(){
    running_ = false;
    if(thread_ && thread_->joinable()) thread_->join();
}

void CoolingController::set_ignition(bool on) { ignition_ = on; if(on) sm_.on_ignition_on(); else sm_.on_ignition_off(); }
void CoolingController::set_level_low(bool low) { level_low_ = low; }
void CoolingController::set_temperature_setpoint(double celsius) { temp_setpoint_ = celsius; }

void CoolingController::control_loop() {
    using namespace std::chrono;
    auto last = steady_clock::now();
    double pump_command = 0.0;
    while(running_) {
        auto now = steady_clock::now();
        double dt = duration<double>(now - last).count();
        if(dt <= 0) dt = 0.01;
        last = now;

        // Simulate temperature dynamics (very simple thermal model)
        double ambient = 25.0;
        double cooling_effect = (pump_command/100.0) * 5.0 + 0.1; // arbitrary
        // the measured_temp goes toward ambient minus cooling + some heating from system
        measured_temp_ += ( (ambient + 0.5) - measured_temp_ ) * 0.01; // natural drift
        measured_temp_ -= cooling_effect * 0.01; // cooling

        double measured = measured_temp_.load();

        // Safety: low coolant
        if(level_low_) {
            sm_.on_fault("low_level");
        }

        // state-machine based actions
        if(ignition_) {
            // if in PREPARE and temperature near setpoint we can go to RUNNING
            if(sm_.state() == CoolingState::PREPARE && std::abs(measured - temp_setpoint_) < 10.0) {
                sm_.on_temp_ok();
            }
        }

        // PID to compute pump command only when RUNNING
        if(sm_.state() == CoolingState::RUNNING && !level_low_) {
            pump_command = pid_.update(temp_setpoint_, measured, dt);
        } else {
            pump_command = 0.0;
            pid_.reset();
        }

        // Cap pump_command and pack into CAN message: id 0x200 is pump command (2 bytes)
        int cmd_rpm = static_cast<int>(2000 + pump_command*10);
        CANMessage cmd;
        cmd.id = 0x200;
        cmd.dlc = 3;
        cmd.data = { static_cast<uint8_t>((cmd_rpm >> 8) & 0xFF),
                     static_cast<uint8_t>(cmd_rpm & 0xFF),
                     static_cast<uint8_t>(static_cast<int>(pump_command)) };

        bus_.send(cmd);

        // Also send display update id 0x300 with temperature and pump %
        CANMessage disp;
        disp.id = 0x300;
        disp.dlc = 3;
        int temp_i = static_cast<int>(measured);
        int pump_i = static_cast<int>(pump_command);
        disp.data = { static_cast<uint8_t>(temp_i & 0xFF),
                      static_cast<uint8_t>(pump_i & 0xFF),
                      static_cast<uint8_t>(static_cast<int>(sm_.state())) };
        bus_.send(disp);

        // Basic console trace
        std::cout << "State: " << static_cast<int>(sm_.state()) << " Temp: " << measured
                  << " Set: " << temp_setpoint_ << " PumpCmd%: " << pump_command << std::endl;

        // sleep until next loop
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}
