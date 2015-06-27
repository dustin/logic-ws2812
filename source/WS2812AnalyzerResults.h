#ifndef WS2812_ANALYZER_RESULTS
#define WS2812_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class WS2812Analyzer;
class WS2812AnalyzerSettings;

class WS2812AnalyzerResults : public AnalyzerResults
{
public:
    WS2812AnalyzerResults(WS2812Analyzer *analyzer, WS2812AnalyzerSettings *settings);
    virtual ~WS2812AnalyzerResults();

    virtual void GenerateBubbleText(U64 frame_index, Channel &channel, DisplayBase display_base);
    virtual void GenerateExportFile(const char *file, DisplayBase display_base, U32 export_type_user_id);

    virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base);
    virtual void GeneratePacketTabularText(U64 packet_id, DisplayBase display_base);
    virtual void GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base);

protected: //functions

    int c(U32 v, int pos);
    int r(U32);
    int g(U32);
    int b(U32 v) { return c(v, 2); };

protected:  //vars
    WS2812AnalyzerSettings *mSettings;
    WS2812Analyzer *mAnalyzer;
};

#endif //WS2812_ANALYZER_RESULTS
