# WS2812 Analyzer for Saleae Logic

This plugin for [Saleae Logic][logic] allows you to decode WS5812 LED
control signals.

![logic analyzer](https://github.com/dustin/logic-ws2812/raw/master/docs/ws2812.png)

I use an export of this to play back a captured strandtest output in a
[bl.ocks.org block](http://bl.ocks.org/dustin/e7207542c69ecbb53ca9)
(since I don't currently have the 60 LEDs it used by default).

## Exporting

CSV export includes both the broken out values and a combined hex
color value:

```csv
Time [s],LED,R,G,B,RGB
0.023266583333333,0,1,0,108,#01006c
0.023296916666667,1,1,0,96,#010060
0.023326916666667,2,1,0,84,#010054
0.023356916666667,3,1,0,72,#010048
0.023386916666667,4,1,0,57,#010039
0.023417000000000,5,1,0,45,#01002d
0.023447000000000,6,1,0,33,#010021
0.023477000000000,7,1,0,21,#010015
```

## Building

Download the [Logic SDK][sdk] and extract it somewhere on your
machine.  In my case, it's `~/stuff/SaleaeAnalyzerSdk-1.1.32`.

Clone the repo and cd into the top level of it:

    git clone https://github.com/dustin/logic-ws2812
    cd logic-ws2812

Symlink the `include` and `lib` directories into your tree:

    ln -s ~/stuff/SaleaeAnalyzerSdk-1.1.32/{include,lib} .

Run the build script:

    ./build_analyzer.py

## Installing

In the Developer tab in Logic preferences, specify the path for
loading new plugins, then copy the built plugin into that location:

    cp release/* /path/specified/in/Logic/preferences

[logic]: https://www.saleae.com/downloads
[sdk]: http://support.saleae.com/hc/en-us/articles/201104644-Analyzer-SDK
