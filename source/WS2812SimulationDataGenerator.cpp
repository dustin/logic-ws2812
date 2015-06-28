#include <cmath>

#include "WS2812SimulationDataGenerator.h"
#include "WS2812AnalyzerSettings.h"
#include "WS2812Analyzer.h"

#include <AnalyzerHelpers.h>

WS2812SimulationDataGenerator::WS2812SimulationDataGenerator() : lednum(0), counter(0)
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

    mClockGenerator.Init(1000000000, simulation_sample_rate);
}

U32 WS2812SimulationDataGenerator::GenerateSimulationData(U64 largest_sample_requested,
                                                       U32 sample_rate,
                                                       SimulationChannelDescriptor **simulation_channel)
{
    U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample(largest_sample_requested,
                                            sample_rate,
                                            mSimulationSampleRateHz);

    mWS2812SimulationData.Advance(mClockGenerator.AdvanceByTimeS(2.0*TLL_MIN / 1000000000.0));

    double frequency = 0.3;
    while (mWS2812SimulationData.GetCurrentSampleNumber() < adjusted_largest_sample_requested) {
        if (lednum == N_LED) {
            mWS2812SimulationData.TransitionIfNeeded(BIT_LOW);
            mWS2812SimulationData.Advance(mClockGenerator.AdvanceByTimeS(2.0*TLL_MIN / 1000000000.0));
            lednum = 0;
        }

        int red   = sin(frequency*counter + 0) * 127 + 128;
        int green = sin(frequency*counter + 2) * 127 + 128;
        int blue  = sin(frequency*counter + 4) * 127 + 128;
        U32 color = green << 16 | red << 8 | blue;

        for (int i = 23; i >= 0; i--) {
            int bit = (color >> i) & 0x1;
            int high = bit ? T1H_TYP : T0H_TYP;

            mWS2812SimulationData.TransitionIfNeeded(BIT_HIGH);
            mWS2812SimulationData.Advance(mClockGenerator.AdvanceByTimeS(high * 1E-9));
            mWS2812SimulationData.TransitionIfNeeded(BIT_LOW);
            mWS2812SimulationData.Advance(mClockGenerator.AdvanceByTimeS(TLD_TYP * 1E-9));
        }

        lednum++;
        counter++;
    }

    *simulation_channel = &mWS2812SimulationData;
    return 1;
}
