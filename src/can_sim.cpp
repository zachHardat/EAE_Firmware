#include "can_sim.hpp"
#include <iostream>

CANBusSim::CANBusSim() : running_(true) {
    thread_ = std::make_unique<std::thread>(&CANBusSim::thread_loop, this);
}

CANBusSim::~CANBusSim() {
    running_ = false;
    cv_.notify_all();
    if(thread_ && thread_->joinable()) thread_->join();
}

void CANBusSim::send(const CANMessage& msg) {
    {
        std::lock_guard<std::mutex> lk(mtx_);
        queue_.push(msg);
    }
    cv_.notify_all();
}

void CANBusSim::register_listener(std::function<void(const CANMessage&)> cb) {
    std::lock_guard<std::mutex> lk(mtx_);
    listeners_.push_back(cb);
}

void CANBusSim::thread_loop() {
    while (running_) {
        std::unique_lock<std::mutex> lk(mtx_);
        cv_.wait_for(lk, std::chrono::milliseconds(10), [this] {
            return !queue_.empty() || !running_;
        });

        while (!queue_.empty()) {
            CANMessage m = queue_.front();
            queue_.pop();
            // call listeners without holding the queue mutex
            lk.unlock();
            for (auto &cb : listeners_) {
                try {
                    cb(m);
                } catch(...) {
                    // swallow exceptions from listeners
                }
            }
            lk.lock();
        }
    }
}
