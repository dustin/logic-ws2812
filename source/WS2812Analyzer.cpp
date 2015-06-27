#include <cmath>
#include <cassert>

#include "WS2812Analyzer.h"
#include "WS2812AnalyzerSettings.h"
#include <AnalyzerChannelData.h>

/*
| Symbol | Parameter                 |   Min | Typical | Max   | Units |
| T0H    | 0 code ,high voltage time |   200 |     350 | 500   | ns    |
| T1H    | 1 code ,high voltage time |   550 |     700 | 5,500 | ns    |
| TLD    | data, low voltage time    |   450 |     600 | 5,000 | ns    |
| TLL    | latch, low voltage time   | 6,000 |         |       | ns    |
*/

#define T0H_MIN 200
#define T0H_MAX 500
#define T1H_MIN 550
#define T1H_MAX 5500
#define TLD_MIN 450
#define TLD_MAX 5500
#define TLL_MIN 50000

WS2812Analyzer::WS2812Analyzer()
    :   Analyzer2(),
        mSettings(new WS2812AnalyzerSettings()),
        mSimulationInitilized(false),
        mSampleRateHz(0)
{
    SetAnalyzerSettings(mSettings.get());
}

WS2812Analyzer::~WS2812Analyzer()
{
    KillThread();
}

void WS2812Analyzer::SetupResults()
{
    mResults.reset(new WS2812AnalyzerResults(this, mSettings.get()));
    SetAnalyzerResults(mResults.get());
    mResults->AddChannelBubblesWillAppearOn(mSettings->mInputChannel);
}

void WS2812Analyzer::WorkerThread()
{
    mSampleRateHz = GetSampleRate();
    mWS2812 = GetAnalyzerChannelData(mSettings->mInputChannel);

    // Look for a start
    while (SamplesToNS(mWS2812->GetSampleOfNextEdge() - mWS2812->GetSampleNumber()) < TLL_MIN) {
        mWS2812->AdvanceToNextEdge();
    }
    mWS2812->AdvanceToNextEdge(); // rising


    U32 ledval = 0;
    int lednum = 0;
    int bitnum = 0;
    U64 ledstart = mWS2812->GetSampleNumber();
    for (;;) {
        assert(mWS2812->GetBitState() == BIT_HIGH);

        U64 start = mWS2812->GetSampleNumber();
        mWS2812->AdvanceToNextEdge(); // falling
        U64 mid = mWS2812->GetSampleNumber(); // mid - start == half bit
        mWS2812->AdvanceToNextEdge(); // rising
        U64 end = mWS2812->GetSampleNumber();

        U64 th = SamplesToNS(mid - start);
        U64 tl = SamplesToNS(end - mid);

        bitnum++;
        ledval <<= 1;
        ledval |= (th > T1H_MIN ? 1 : 0);

        if (bitnum == 24) {
            mResults->AddMarker(end - ((end - ledstart) / 2),
                                AnalyzerResults::Dot,
                                mSettings->mInputChannel);

            Frame frame;
            frame.mData1 = lednum;
            frame.mData2 = ledval;
            frame.mStartingSampleInclusive = ledstart;
            frame.mEndingSampleInclusive = end;
            mResults->AddFrame(frame);
            mResults->CommitResults();
            ReportProgress(frame.mEndingSampleInclusive);

            ledval = 0;
            bitnum = 0;
            ledstart = mWS2812->GetSampleOfNextEdge();
            lednum++;
        }

        if (tl >= TLL_MIN) {
            ledval = 0;
            bitnum = 0;
            lednum = 0;
            ledstart = mWS2812->GetSampleOfNextEdge();
        }
    }
}

bool WS2812Analyzer::NeedsRerun()
{
    return false;
}

U32 WS2812Analyzer::GenerateSimulationData(U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor **simulation_channels)
{
    if (mSimulationInitilized == false) {
        mSimulationDataGenerator.Initialize(GetSimulationSampleRate(), mSettings.get());
        mSimulationInitilized = true;
    }

    return mSimulationDataGenerator.GenerateSimulationData(minimum_sample_index, device_sample_rate, simulation_channels);
}

U32 WS2812Analyzer::GetMinimumSampleRateHz()
{
    return 1000000;
}

const char *WS2812Analyzer::GetAnalyzerName() const
{
    return "WS2812";
}

const char *GetAnalyzerName()
{
    return "WS2812";
}

Analyzer *CreateAnalyzer()
{
    return new WS2812Analyzer();
}

void DestroyAnalyzer(Analyzer *analyzer)
{
    delete analyzer;
}
