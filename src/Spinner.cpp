#include "Spinner.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

static constexpr char FETCH_LABEL[] = "Fetching METAR ";
static constexpr char FRAMES[] = {'|', '/', '-', '\\'};

static void clear_status_line(std::ostream& os, std::size_t line_width) {
    os << '\r' << std::string(line_width, ' ') << '\r' << std::flush;
}

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
        clear_status_line(std::cout, sizeof(FETCH_LABEL));
    }
}

void Spinner::spin() {
    int i = 0;

    while (running_) {
        std::cout << '\r' << FETCH_LABEL << FRAMES[i++ % 4] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(120));
    }
}
