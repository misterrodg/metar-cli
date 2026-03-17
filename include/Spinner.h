#ifndef SPINNER_H
#define SPINNER_H

#include <atomic>
#include <thread>

class Spinner {
public:
    Spinner();
    ~Spinner();

    void start();
    void stop();

private:
    void spin();

    std::atomic<bool> running_;
    std::thread thread_;
};

#endif
