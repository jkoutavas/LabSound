// License: BSD 2 Clause
// Copyright (C) 2015+, The LabSound Authors. All rights reserved.

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
    #define _CRT_SECURE_NO_WARNINGS
#endif

#include "ExampleBaseApp.h"

#include "Simple.h"
#include "PeakCompressor.h"
#include "LiveGraphUpdate.h"
#include "RenderOffline.h"
#include "ConvolutionReverb.h"
#include "RedAlert.h"
#include "Groove.h"
#include "InfiniteFM.h"
#include "Tremolo.h"
#include "Spatialization.h"

/*
#include "MicrophoneDalek.h"
#include "MicrophoneLoopback.h"
#include "MicrophoneReverb.h"
#include "StereoPanning.h"

#include "Validation.h"
*/

SimpleApp g_simpleExample;
PeakCompressorApp g_peakCompressor;
LiveGraphUpdateApp g_liveGraphUpdateApp;
OfflineRenderApp g_offlineRenderApp;
ConvolutionReverbApp g_convolutionReverbExample;
RedAlertApp g_redAlert;
GrooveApp g_grooveExample;
InfiniteFMApp g_infiniteFM;
TremoloApp g_tremolo;
SpatializationApp g_spatialization;

/*
    MicrophoneDalekApp g_microphoneDalekApp;
    MicrophoneLoopbackApp g_microphoneLoopback;
    MicrophoneReverbApp g_microphoneReverb;

    ValidationApp g_validation;
    StereoPanningApp g_stereoPanning;
*/

// Windows users will need to set a valid working directory for the LabSoundExamples project, for instance $(ProjectDir)../../assets
int main (int argc, char *argv[])
{
    try
    {
        g_spatialization.PlayExample();
    }
    catch (const std::exception & e)
    {
        std::cout << "Uncaught fatal exception: " << e.what() << std::endl;
    }

    return 0;
}
