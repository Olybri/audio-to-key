
#include <csignal>
#include <iostream>
#include <thread>

#include "recorder.hpp"

volatile sig_atomic_t running = 1;

int main()
{
    std::cout.sync_with_stdio(false);

    std::cout << "Starting Audio-to-Key..." << std::endl;

    auto availableDevices = sf::SoundRecorder::getAvailableDevices();
    std::cout << "Available devices:\n";
    for(unsigned i=0; i<availableDevices.size(); ++i)
        std::cout << i+1 << ". " << availableDevices[i] << std::endl;

    std::string device;

    if(availableDevices.size() > 1)
    {
        unsigned value = 0;
        while(value < 1 || value > availableDevices.size())
        {
            std::cout << "Enter device number (1-" << availableDevices.size() << "): ";
            std::string line;
            std::getline(std::cin, line);
            try
            {
                value = std::stoul(line);
            }
            catch(std::exception)
            {
                std::cout << "Please enter a number." << std::endl;
            }
        }
        device = availableDevices[value - 1];
    }
    else
        device = availableDevices.front();

    std::cout << "Recording now from device: " << device << ".\nPress Ctrl+C to exit." << std::endl;

    // ctor: device, threshold, multiplier, release, interval
    SignalToKey rec(device, 8000, 700, 100, 60);
    rec.start();

    #pragma GCC diagnostic ignored "-Wunused-parameter"
    signal(SIGINT, [](int signum){ running = 0; });

    while(running)
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

    rec.stop();

    std::cout << "\nEnd of reccord. Exiting..." << std::endl;
}
