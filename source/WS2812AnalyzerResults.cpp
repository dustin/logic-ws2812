#include "WS2812AnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "WS2812Analyzer.h"
#include "WS2812AnalyzerSettings.h"
#include <fstream>

WS2812AnalyzerResults::WS2812AnalyzerResults(WS2812Analyzer *analyzer, WS2812AnalyzerSettings *settings)
    :   AnalyzerResults(),
        mSettings(settings),
        mAnalyzer(analyzer)
{
}

WS2812AnalyzerResults::~WS2812AnalyzerResults()
{
}

void WS2812AnalyzerResults::GenerateBubbleText(U64 frame_index, Channel &channel, DisplayBase display_base)
{
    ClearResultStrings();
    Frame frame = GetFrame(frame_index);

    char long_str[128] = {0};

    snprintf(long_str, sizeof(long_str), "%d: r=%02x g=%02x b=%02x", frame.mData1,
             r(frame.mData2), g(frame.mData2), b(frame.mData2));

    char short_str[128] = {0};
    snprintf(short_str, sizeof(long_str), "%d: #%02x%02x%02x", frame.mData1,
             r(frame.mData2), g(frame.mData2), b(frame.mData2));

    AddResultString(long_str);
    AddResultString(short_str);
}

void WS2812AnalyzerResults::GenerateExportFile(const char *file, DisplayBase display_base, U32 export_type_user_id)
{
    std::ofstream file_stream(file, std::ios::out);

    U64 trigger_sample = mAnalyzer->GetTriggerSample();
    U32 sample_rate = mAnalyzer->GetSampleRate();

    file_stream << "Time [s],LED,R,G,B,RGB" << std::endl;

    U64 num_frames = GetNumFrames();
    for (U32 i = 0; i < num_frames; i++) {
        Frame frame = GetFrame(i);

        char time_str[128];
        AnalyzerHelpers::GetTimeString(frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128);

        char number_str[128];
        snprintf(number_str, sizeof(number_str), "%d,%d,%d,%d,#%02x%02x%02x",
                 frame.mData1,
                 r(frame.mData2), g(frame.mData2), b(frame.mData2),
                 r(frame.mData2), g(frame.mData2), b(frame.mData2));

        file_stream << time_str << "," << number_str << std::endl;

        if (UpdateExportProgressAndCheckForCancel(i, num_frames) == true) {
            file_stream.close();
            return;
        }
    }

    file_stream.close();
}

void WS2812AnalyzerResults::GenerateFrameTabularText(U64 frame_index, DisplayBase display_base)
{
    Frame frame = GetFrame(frame_index);
    ClearTabularText();

    char long_str[128] = {0};

    snprintf(long_str, sizeof(long_str), "%d: r=%02x g=%02x b=%02x", frame.mData1,
             r(frame.mData2), g(frame.mData2), b(frame.mData2));

    AddTabularText(long_str);
}

void WS2812AnalyzerResults::GeneratePacketTabularText(U64 packet_id, DisplayBase display_base)
{
    ClearResultStrings();
    AddResultString("not supported");
}

void WS2812AnalyzerResults::GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base)
{
    ClearResultStrings();
    AddResultString("not supported");
}

int WS2812AnalyzerResults::c(U32 v, int pos) {
    switch (pos) {
    case 0:
        return v >> 16;
    case 1:
        return v >> 8 && 0xff;
    case 2:
        return v & 0xff;
    }
    abort();
}

int WS2812AnalyzerResults::r(U32 v) { return mSettings->mColorOrder == COLOR_RGB ? c(v, 0) : c(v, 1); };
int WS2812AnalyzerResults::g(U32 v) { return mSettings->mColorOrder == COLOR_RGB ? c(v, 1) : c(v, 0); };
