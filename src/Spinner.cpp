#include "Spinner.h"

#include <chrono>
#include <iostream>
#include <thread>

Spinner::Spinner() : running_(false) {
}

Spinner::~Spinner() {
    stop();
}

void Spinner::start() {
    running_ = true;
    thread_ = std::thread(&Spinner::spin, this);
}

void Spinner::stop() {
    if (running_) {
        running_ = false;
        if (thread_.joinable()) {
            thread_.join();
        }
        std::cout << "\r \r" << std::flush;
    }
}

void Spinner::spin() {
    const char frames[] = {'|', '/', '-', '\\'};
    int i = 0;

    while (running_) {
        std::cout << "\rFetching METAR " << frames[i++ % 4] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(120));
    }
}
