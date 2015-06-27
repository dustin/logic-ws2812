#ifndef WS2812_ANALYZER_SETTINGS
#define WS2812_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

#define COLOR_GRB 1
#define COLOR_RGB 2

class WS2812AnalyzerSettings : public AnalyzerSettings
{
public:
    WS2812AnalyzerSettings();
    virtual ~WS2812AnalyzerSettings();

    virtual bool SetSettingsFromInterfaces();
    void UpdateInterfacesFromSettings();
    virtual void LoadSettings(const char *settings);
    virtual const char *SaveSettings();


    Channel mInputChannel;
    int mColorOrder;

protected:
    std::auto_ptr< AnalyzerSettingInterfaceChannel >    mInputChannelInterface;
    std::auto_ptr< AnalyzerSettingInterfaceNumberList > mColorOrderInterface;
};

#endif //WS2812_ANALYZER_SETTINGS
