#ifndef WS2812_ANALYZER_H
#define WS2812_ANALYZER_H

#include <Analyzer.h>
#include "WS2812AnalyzerResults.h"
#include "WS2812SimulationDataGenerator.h"

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
