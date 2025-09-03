#pragma once
#include <cstdint>
#include <vector>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <atomic>
#include <chrono>

// Simple CAN message type for simulation
struct CANMessage {
    uint32_t id;
    std::vector<uint8_t> data;
    uint8_t dlc;
};

// A thread-safe in-memory CAN bus simulator.
// One instance represents the bus; threads can send/subscribe.
class CANBusSim {
public:
    CANBusSim();
    ~CANBusSim();

    // Send a message onto the simulated CAN bus
    void send(const CANMessage& msg);

    // Register a callback invoked for every received message
    // callback will be called from the bus thread - keep callbacks short
    void register_listener(std::function<void(const CANMessage&)> cb);

private:
    void thread_loop();
    std::queue<CANMessage> queue_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::vector<std::function<void(const CANMessage&)>> listeners_;
    std::unique_ptr<std::thread> thread_;
    std::atomic<bool> running_;
};
