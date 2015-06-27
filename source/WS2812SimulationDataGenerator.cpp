#include "WS2812SimulationDataGenerator.h"
#include "WS2812AnalyzerSettings.h"

#include <AnalyzerHelpers.h>

WS2812SimulationDataGenerator::WS2812SimulationDataGenerator() : rgen(),
                                                           rdist(.5),
                                                           pulseLen(1500)
{
}

WS2812SimulationDataGenerator::~WS2812SimulationDataGenerator()
{
}

void WS2812SimulationDataGenerator::Initialize(U32 simulation_sample_rate,
                                            WS2812AnalyzerSettings *settings)
{
    mSimulationSampleRateHz = simulation_sample_rate;
    mSettings = settings;

    mWS2812SimulationData.SetChannel(mSettings->mInputChannel);
    mWS2812SimulationData.SetSampleRate(simulation_sample_rate);
    mWS2812SimulationData.SetInitialBitState(BIT_LOW);

    mClockGenerator.Init(1000000, simulation_sample_rate);
}

U32 WS2812SimulationDataGenerator::GenerateSimulationData(U64 largest_sample_requested,
                                                       U32 sample_rate,
                                                       SimulationChannelDescriptor **simulation_channel)
{
    U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample(largest_sample_requested,
                                            sample_rate,
                                            mSimulationSampleRateHz);

    while (mWS2812SimulationData.GetCurrentSampleNumber() < adjusted_largest_sample_requested) {
        double by = 10*rdist(rgen)*(rgen()&1?1:-1);
        pulseLen += (pulseLen+by > 2000 || pulseLen+by < 1000) ? -by : by;

        mWS2812SimulationData.Advance(mSimulationSampleRateHz / 50);

        mWS2812SimulationData.TransitionIfNeeded(BIT_HIGH);
        mWS2812SimulationData.Advance(mClockGenerator.AdvanceByTimeS(pulseLen * 1E-6));
        mWS2812SimulationData.TransitionIfNeeded(BIT_LOW);
    }

    *simulation_channel = &mWS2812SimulationData;
    return 1;
}
