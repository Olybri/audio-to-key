// Audio-to-Key v0.1 © 2017 Loris Witschard

#include <iostream>
#include <thread>
#include <fstream>
#include <csignal>
#include <type_traits>
#include <experimental/filesystem>
namespace std { namespace filesystem = experimental::filesystem; }

#include "recorder.hpp"
#include "../lib/json.hpp"
namespace nl = nlohmann;

/////////////////////////////////////
//   HELPERS
/////////////////////////////////////

template<typename T>
T getNumber(T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max())
{
    static_assert(std::is_arithmetic<T>::value, "Arithmetic type required.");

    while(true)
    {
        std::string line;
        std::getline(std::cin, line);

        T value;
        try
        {
            if(std::is_integral<T>::value)
                if(std::is_unsigned<T>::value)
                    value = std::stoul(line);
                else
                    value = std::stol(line);

            else if(std::is_floating_point<T>::value)
                value = std::stof(line);

            if(value < min)
                throw std::runtime_error("Value too small.");
            if(value > max)
                throw std::runtime_error("Value too big.");

            return value;
        }
        catch(std::runtime_error & e)
        {
            std::cout << e.what();
        }
        catch(std::exception & e)
        {
            std::cout << "Invalid number!";
        }
        std::cout << "\nPlease enter a valid number: ";
    }
}

auto getDevice()
{
    auto availableDevices = sf::SoundRecorder::getAvailableDevices();
    std::cout << "Available devices:\n";
    for(unsigned i=0; i<availableDevices.size(); ++i)
        std::cout << i+1 << ". " << availableDevices[i] << std::endl;

    if(availableDevices.size() > 1)
    {
        std::cout << "Enter device number (1-" << availableDevices.size() << "): ";
        auto index = getNumber<unsigned>(1, availableDevices.size());

        return availableDevices[index - 1];
    }
    else
    {
        std::cout << "Selected device 1." << std::endl;
        return availableDevices.front();
    }
}

/////////////////////////////////////
//   MAIN
/////////////////////////////////////

volatile sig_atomic_t running = true;

int main()
{
    std::cout.sync_with_stdio(false);

    std::cout << "Audio-to-Key v0.1 (c) 2017 Loris Witschard" << std::endl;

    nl::json config;

    std::filesystem::path configPath = "config.json";
    if(!std::filesystem::exists(configPath))
    {
        std::cout << "Starting initial configuration." << std::endl;

        config["device"] = getDevice();

        std::cout << "Enter threshold (0-100): ";
        auto threshold = getNumber<float>(0, 100) / 100;
        config["threshold"] = static_cast<short>(threshold * std::numeric_limits<short>::max());

        std::cout << "Enter multiplier: ";
        config["multiplier"] = getNumber<float>(0);

        std::cout << "Enter release time (milliseconds): ";
        config["release"] = getNumber<int>(0);

        std::ofstream configFile(configPath.string());
        configFile << std::setw(2) << config << std::endl;

        std::cout << "Configuration saved to: " << configPath << std::endl;
    }
    else
    {
        std::ifstream configFile(configPath.string());
        configFile >> config;

        std::cout << "Configuration loaded from: " << configPath << std::endl;
    }

    std::cout << "Recording now from device: " << config["device"] << ".\nPress Ctrl+C to exit." << std::endl;

    SignalToKey rec(config["device"], config["threshold"], config["multiplier"], config["release"], 60);
    rec.start();

    #pragma GCC diagnostic ignored "-Wunused-parameter"
    signal(SIGINT, [](int signum){ running = false; });

    while(running)
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

    rec.stop();

    std::cout << "\nEnd of record. Exiting..." << std::endl;
}
