# WS2812 Analyzer for Saleae Logic

This plugin for [Saleae Logic][logic] allows you to decode WS5812 LED
control signals.

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

In the Develoepr tab in Logic preferences, specify the path for
loading new plugins, then copy the built plugin into that location:

    cp release/* /path/specified/in/Logic/preferences

[logic]: https://www.saleae.com/downloads
[sdk]: http://support.saleae.com/hc/en-us/articles/201104644-Analyzer-SDK
