#ifndef WS2812_ANALYZER_H
#define WS2812_ANALYZER_H

#include <Analyzer.h>
#include "WS2812AnalyzerResults.h"
#include "WS2812SimulationDataGenerator.h"

/*
From
http://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/

| Symbol | Parameter                 |   Min | Typical | Max   | Units |
| T0H    | 0 code ,high voltage time |   200 |     350 | 500   | ns    |
| T1H    | 1 code ,high voltage time |   550 |     700 | 5,500 | ns    |
| TLD    | data, low voltage time    |   450 |     600 | 5,000 | ns    |
| TLL    | latch, low voltage time   | 6,000 |         |       | ns    |
*/

#define T0H_MIN 200
#define T0H_TYP 350
#define T0H_MAX 500
#define T1H_MIN 550
#define T1H_TYP 700
#define T1H_MAX 5500
#define TLD_MIN 450
#define TLD_TYP 600
#define TLD_MAX 5500
#define TLL_MIN 50000

class WS2812AnalyzerSettings;
class ANALYZER_EXPORT WS2812Analyzer : public Analyzer2
{
public:
    WS2812Analyzer();
    virtual ~WS2812Analyzer();
    virtual void WorkerThread();
    virtual void SetupResults();

    virtual U32 GenerateSimulationData(U64 newest_sample_requested,
                                       U32 sample_rate,
                                       SimulationChannelDescriptor **simulation_channels);
    virtual U32 GetMinimumSampleRateHz();

    virtual const char *GetAnalyzerName() const;
    virtual bool NeedsRerun();

    U64 SamplesToNS(U64 samples) { return (samples * 1000000000) / mSampleRateHz; }

protected: //vars
    std::auto_ptr< WS2812AnalyzerSettings > mSettings;
    std::auto_ptr< WS2812AnalyzerResults > mResults;
    AnalyzerChannelData *mWS2812;

    WS2812SimulationDataGenerator mSimulationDataGenerator;
    bool mSimulationInitilized;

    //WS2812 analysis vars:
    U32 mSampleRateHz;
};

extern "C" ANALYZER_EXPORT const char *__cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer *__cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer(Analyzer *analyzer);

#endif //WS2812_ANALYZER_H
