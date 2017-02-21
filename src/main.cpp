#include <SFML/Audio/SoundBufferRecorder.hpp>
#include <SFML/System/Clock.hpp>
#include <windows.h>
#include <iostream>

class MyRecorder : public sf::SoundRecorder
{
public:
    MyRecorder()
    {
        setProcessingInterval(sf::milliseconds(10));
    }

private:
    bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount) override
    {
        std::vector<short int> buffer(samples, samples + sampleCount);

        int max = 0;
        for(auto sample : buffer)
            if(std::abs(sample) > max)
                max = std::abs(sample);

        if(max > 8000)
        {
            keybd_event(0x47, 0x45, KEYEVENTF_EXTENDEDKEY, 0);
            keybd_event(0x47, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        }

        return true;
    }
};

int main()
{
    std::cout.sync_with_stdio(false);
    auto availableDevices = sf::SoundRecorder::getAvailableDevices();

    for(auto device : availableDevices)
        std::cout << device << std::endl;

    MyRecorder rec;
    sf::Clock clock;
    rec.start();

    std::cin.get();
    rec.stop();
}
