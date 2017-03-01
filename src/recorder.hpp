// © 2017 Loris Witschard

#include <SFML/Audio/SoundBufferRecorder.hpp>
#include <windows.h> // SendInput
#include <iostream> // std::cout
#include <algorithm> // std::max_element
#include <limits> // std::numeric_limits

class SignalToKey : public sf::SoundRecorder
{
public:
    SignalToKey(WORD key, std::string device, short threshold, float multiplier, int release, int interval)
    : m_amplitude {0}, m_decrease{0}
    {
        m_threshold = threshold;
        m_multiplier = multiplier;
        m_release = release;
        m_interval = interval;
        m_key = key;

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
            std::cout << (i == amp ? 'T' : '-');

        std::cout << std::endl;
    }

private:
    short m_threshold;
    short m_amplitude;
    int m_interval;
    int m_release;
    short m_decrease;
    float m_multiplier;
    WORD m_key;

    bool m_pressed = false;

    const int m_screenWidth = 80;

    bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount) override
    {
        if(m_amplitude - m_decrease < 0)
            m_amplitude = 0;
        else
            m_amplitude -= m_decrease;

        std::vector<short> buffer(samples, samples + sampleCount);

        auto pick = *std::max_element(buffer.begin(), buffer.end(),
            [](int a, int b){ return std::abs(a) < std::abs(b); });

        pick = std::min(
            static_cast<float>(std::abs(pick) * m_multiplier),
            static_cast<float>(std::numeric_limits<decltype(pick)>::max()));

        if(pick > m_amplitude)
        {
            m_amplitude = pick;
            m_decrease = m_amplitude / std::max(m_release / m_interval, 1);
        }

        INPUT ip;
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0;
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;
        ip.ki.wVk = m_key;

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
        amp /= std::numeric_limits<decltype(m_amplitude)>::max();

        for(int i=0; i<m_screenWidth; ++i)
            std::cout << (i < amp ? 'O' : '-');

        std::cout << "\r" << std::flush;

        return true;
    }
};
