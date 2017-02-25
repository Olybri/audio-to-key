#include <SFML/Audio/SoundBufferRecorder.hpp>
#include <SFML/System/Clock.hpp>
#include <windows.h>
#include <csignal>
#include <iostream>
#include <limits>
#include <thread>
#include <algorithm>

class MyRecorder : public sf::SoundRecorder
{
public:
    MyRecorder(std::string device, short threshold, float multiplier, int release, int interval)
    : m_amplitude {0}, m_decrease{0}
    {
        m_threshold = threshold;
        m_multiplier = multiplier;
        m_release = release;
        m_interval = interval;

        setProcessingInterval(sf::milliseconds(m_interval));
        setDevice(device);

        int amp = m_threshold;
        amp *= m_screenWidth;
        amp /= 32800;

        std::cout << "    ";

        for(int i=0; i<amp; ++i)
            std::cout << " ";

        std::cout << "Threshold\n    ";

        for(int i=0; i<m_screenWidth; ++i)
            std::cout << (i == amp ? "T" : "-");

        std::cout << std::endl;
    }

private:
    short m_threshold;
    short m_amplitude;
    int m_interval;
    int m_release;
    short m_decrease;
    float m_multiplier;

    bool m_pressed = false;

    sf::Clock clock;

    const int m_screenWidth = 80;

    bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount) override
    {
        if(m_amplitude - m_decrease < 0)
            m_amplitude = 0;
        else
            m_amplitude -= m_decrease;

        std::vector<short> buffer(samples, samples + sampleCount);

        auto max = *std::max_element(buffer.begin(), buffer.end(),
            [](int a, int b){ return std::abs(a) < std::abs(b); });

        max = std::min(std::abs(max) * m_multiplier,
            static_cast<float>(std::numeric_limits<decltype(max)>::max()));

        if(max > m_amplitude)
        {
            m_amplitude = max;
            m_decrease = m_amplitude / (m_release / m_interval);
        }

        INPUT ip;
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0;
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;
        ip.ki.wVk = VK_CONTROL;

        if(m_amplitude > m_threshold)
        {
            if(!m_pressed)
            {
                m_pressed = true;

                ip.ki.dwFlags = 0;
                SendInput(1, &ip, sizeof(INPUT));
            }

            std::cout << "ON  ";
        }
        else
        {
            if(m_pressed)
            {
                m_pressed = false;

                ip.ki.dwFlags = KEYEVENTF_KEYUP;
                SendInput(1, &ip, sizeof(INPUT));
            }

            std::cout << "OFF ";
        }

        int amp = m_amplitude;
        amp *= m_screenWidth;
        amp /= 32800;

        for(int i=0; i<amp; ++i)
            std::cout << "O";

        for(int i=0; i<m_screenWidth - amp; ++i)
            std::cout << "-";

        std::cout << "\r" << std::flush;

        return true;
    }
};

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
    MyRecorder rec(device, 8000, 700, 100, 60);
    rec.start();

    signal(SIGINT, [](int signum){ running = 0;});
    while(running)
        std::this_thread::sleep_for (std::chrono::milliseconds(60));

    rec.stop();

    std::cout << "\nEnd of reccord. Exiting..." << std::endl;
}
