#include "WS2812AnalyzerSettings.h"
#include <AnalyzerHelpers.h>


WS2812AnalyzerSettings::WS2812AnalyzerSettings()
    :   mInputChannel(UNDEFINED_CHANNEL),
        mColorOrder(COLOR_GRB)
{
    mInputChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
    mInputChannelInterface->SetTitleAndTooltip("WS2812", "WS2812 LED Analyzer");
    mInputChannelInterface->SetChannel(mInputChannel);

    mColorOrderInterface.reset(new AnalyzerSettingInterfaceNumberList());
    mColorOrderInterface->SetTitleAndTooltip("Color Order",
                                          "What is the color order in these LEDs?");
    mColorOrderInterface->AddNumber(COLOR_GRB, "GRB", "Green, Red, Blue (normal)");
    mColorOrderInterface->AddNumber(COLOR_RGB, "RGB", "Red, Green, Blue");
    mColorOrderInterface->SetNumber(mColorOrder);

    AddInterface(mInputChannelInterface.get());
    AddInterface(mColorOrderInterface.get());

    AddExportOption(0, "Export as csv file");
    AddExportExtension(0, "csv", "csv");

    ClearChannels();
    AddChannel(mInputChannel, "WS2812", false);
}

WS2812AnalyzerSettings::~WS2812AnalyzerSettings()
{
}

bool WS2812AnalyzerSettings::SetSettingsFromInterfaces()
{
    mInputChannel = mInputChannelInterface->GetChannel();
    mColorOrder = mColorOrderInterface->GetNumber();

    ClearChannels();
    AddChannel(mInputChannel, "WS2812 Analyzer", true);

    return true;
}

void WS2812AnalyzerSettings::UpdateInterfacesFromSettings()
{
    mInputChannelInterface->SetChannel(mInputChannel);
    mColorOrderInterface->SetNumber(mColorOrder);
}

void WS2812AnalyzerSettings::LoadSettings(const char *settings)
{
    SimpleArchive text_archive;
    text_archive.SetString(settings);

    text_archive >> mInputChannel;
    text_archive >> mColorOrder;

    ClearChannels();
    AddChannel(mInputChannel, "Simple WS2812 Analyzer", true);

    UpdateInterfacesFromSettings();
}

const char *WS2812AnalyzerSettings::SaveSettings()
{
    SimpleArchive text_archive;

    text_archive << mInputChannel;
    text_archive << mColorOrder;

    return SetReturnString(text_archive.GetString());
}
