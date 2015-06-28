#ifndef WS2812_SIMULATION_DATA_GENERATOR
#define WS2812_SIMULATION_DATA_GENERATOR

#include <AnalyzerHelpers.h>

#include <SimulationChannelDescriptor.h>
class WS2812AnalyzerSettings;

#define N_LED 60

class WS2812SimulationDataGenerator
{
public:
    WS2812SimulationDataGenerator();
    ~WS2812SimulationDataGenerator();

    void Initialize(U32 simulation_sample_rate, WS2812AnalyzerSettings *settings);
    U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor **simulation_channel);

protected:
    WS2812AnalyzerSettings *mSettings;
    U32 mSimulationSampleRateHz;

protected:
    SimulationChannelDescriptor mWS2812SimulationData;
    ClockGenerator mClockGenerator;

    int lednum;
    int counter;

};
#endif //WS2812_SIMULATION_DATA_GENERATOR
