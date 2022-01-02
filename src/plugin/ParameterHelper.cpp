#include "ParameterHelper.h"
#include "LcdScreen.h"

juce::AudioProcessorValueTreeState::ParameterLayout ParameterHelper::createParameterLayout() {
    // TODO change all on off parameters to bools
    // auto attributes = AudioParameterBoolAttributes().withStringFromValueFunction ([] (auto x, auto) { return x ? "On" : "Off"; })
    //                                                .withLabel ("enabled");

    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    juce::StringArray waves = {
            "SAW", "BELL", "SINE", "SQUARE", "PULSE", "NOISE1", "NOISE2", "NOISE3", "BASS", "PIANO", "EL PNO",
            "VOICE1", "VOICE2", "KICK", "REED", "ORGAN", "SYNTH1", "SYNTH2", "SYNTH3", "FORMT1", "FORMT2",
            "FORMT3", "FORMT4", "FORMT5", "PULSE2", "SQR 2", "4 OCTS", "PRIME", "BASS 2", "E PNO2", "OCTAVE",
            "OCT+5", "SAW 2", "TRIANG", "REED 2", "REED 3", "GRIT 1", "GRIT 2", "GRIT 3", "GLINT1", "GLINT2",
            "GLINT3", "CLAV", "BRASS", "STRING", "DIGIT1", "DIGIT2", "BELL 2", "ALIEN", "BREATH", "VOICE3",
            "STEAM", "METAL", "CHIME", "BOWING", "PICK 1", "PICK 2", "MALLET", "SLAP", "PLINK", "PLUCK",
            "PLUNK", "CLICK", "CHIFF", "THUMP", "LOGDRM", "KICK 2", "SNARE", "TOMTOM", "HI-HAT", "DRUMS1",
            "DRUMS2", "DRUMS3", "DRUMS4", "DRUMS5"
    };

    // TODO there is something weird about the formatting for these
    juce::StringArray lfoWaves = { "TRI", "SAW", "SQR", "NOI", "BIP" };
    lfoWaves.addArray(waves);
    lfoWaves.addArray({ "EXP", "EXP2", "EXP3", "EXP4", "TAN", "TAN2", "TAN3", "TAN4" });

    juce::StringArray modulationSources = {
            "*OFF*", "LFO1", "LFO2", "LFO3", "LFO4", "ENV1", "ENV2", "ENV3", "ENV4", "VEL",
            "VEL-X", "KEYB", "KEYB2", "PRESS", "MAT1", "MAT2", "MAT3", "BANK", "WHEEL", "BREATH",
            "CTR 3", "PEDAL", "PORTA", "DATA", "VOLUME", "BALANC", "CTR 9", "PAN", "EXPRES", "EFFCT1",
            "EFFCT2", "CTR 14", "CTR 15", "GENP 1", "GENP 2", "GENP 3", "GENP 4", "CTR 20", "CTR 21", "CTR 22",
            "CTR 23", "CTR 24", "CTR 25", "CTR 26", "CTR 27", "CTR 28", "CTR 29", "CTR 30", "CTR 31", "BANKLO",
            "WHEELL", "BRTHLO", "CTR 35", "PEDALL", "PORTAL", "DATALO", "VOL LO", "BANKLO", "CTR 41", "PAN LO",
            "EXPRLO", "EFF1L0", "EFF2LO", "CTR 46", "CTR 47", "CTR 48", "CTR 49", "CTR 50", "CTR 51", "CTR 52",
            "CTR 53", "CTR 54", "CTR 55", "CTR 56", "CTR 57", "CTR 58", "CTR 59", "CTR 60", "CTR 61", "CTR 62",
            "CTR 63", "HLDPED", "PORTON", "SUSPED", "SFTPED", "LEGPED", "HLD2PD", "VARIAT", "TIMBRE", "RELEAS",
            "ATTACK", "BRIGHT", "SND 6", "SND 7", "SND 8", "SND 9", "SND 10", "GENP1H", "GENP2H", "GENP3H",
            "GENP4H", "CTR 84", "CTR 85", "CTR 86", "CTR 87", "CTR 88", "CTR 89", "CTR 90", "EFFLEV", "TREMOL",
            "CHORUS", "CELEST", "PHASER", "DATINC", "DATDEC", "NREGLO", "NREGHI", "REGLO", "REGHI", "CTR102",
            "CTR103", "CTR104", "CTR105", "CTR106", "CTR107", "CTR108", "CTR109", "CTR110", "CTR111", "CTR112",
            "CTR113", "CTR114", "CTR115", "CTR116", "CTR117", "CTR118", "CTR119", "SNDOFF", "CTROFF", "LOCAL",
            "ALLOFF", "OMNOFF", "OMNION", "MONO", "POLY", "CHPRES", "BEND"
    };

    juce::StringArray saturationValues = {
            "OFF", "EMU", "+01", "+02", "+03", "+04", "+05", "+06", "+07",
            "+08", "+09", "+10", "+11", "+12", "+13", "+14", "+15",
    };

    juce::StringArray pitchBendModes = {
            "ALL  ", "HELD ", "NEW  ", "NEWH ", "HELD2", "NEW2 ", "NEWH2"
    };

    juce::StringArray offOn = { "OFF", "ON" };
    juce::StringArray hardSoft = { "HARD", "SOFT" };
    juce::StringArray emuFast = { "EMU", "FAST" };
    juce::StringArray dcBlocking = { "SMART", "ON", "OFF" };
    juce::StringArray emuSmooth = { "EMU", "SMTH" };
    juce::StringArray lfoModulatorValues = {"UNI", "BIP", "PHS", "SMT" };
    juce::StringArray lfoPlayModeValues = { "FWD", "REV", "1XF", "1XR" };
    juce::StringArray lfoHuman = { "OFF", "ON", "X1", "X2", "X4", "X8", "X16" };
    juce::StringArray envT1VModes = { "T1", "SMT" };
    juce::StringArray envShapes = { "OFF", "EXP", "EXP2", "EXP3", "EXP4", "TAN", "TAN2", "TAN3" };

    // OSC
    for (int i = 0; i < 3; i++) {
        auto osc = "OSC" + std::to_string(i + 1);
        auto g = std::make_unique<juce::AudioProcessorParameterGroup>(osc, osc, "");

        g->addChild(makeParamInt(osc + ".OCT", "OCT", -3, 5, 0));
        g->addChild(makeParamInt(osc + ".SEMI", "SEMI", -12, 12, 0));
        g->addChild(makeParamInt(osc + ".FINE", "FINE", -31, 31, 0));
        g->addChild(makeParamChoice(osc + ".WAVE", "WAVE", waves, 0));
        g->addChild(makeParamChoice(osc + ".MOD1.SEL", "MOD1.SEL", modulationSources, 0));
        g->addChild(makeParamInt(osc + ".MOD1.AMT", "MOD1.AMT", -63, 63, 0));
        g->addChild(makeParamChoice(osc + ".MOD2.SEL", "MOD1.SEL", modulationSources, 0));
        g->addChild(makeParamInt(osc + ".MOD2.AMT", "MOD1.AMT", -63, 63, 0));

        layout.add(std::move(g));
    }

    // DCA
    for (int i = 0; i < 3; i++) {
        auto dca = "DCA" + std::to_string(i + 1);
        auto g = std::make_unique<juce::AudioProcessorParameterGroup>(dca, dca, "");

        g->addChild(makeParamInt(dca + ".LEVEL", "LEVEL", 0, 63, 50));
        g->addChild(makeParamChoice(dca + ".OUTPUT", "OUTPUT", offOn, (i == 0) ? 1 : 0));
        g->addChild(makeParamChoice(dca + ".MOD1.SEL", "MOD1.SEL", modulationSources, 0));
        g->addChild(makeParamInt(dca + ".MOD1.AMT", "MOD1.AMT", -63, 63, 0));
        g->addChild(makeParamChoice(dca + ".MOD2.SEL", "MOD2.SEL", modulationSources, 0));
        g->addChild(makeParamInt(dca + ".MOD2.AMT", "MOD2.AMT", -63, 63, 0));

        layout.add(std::move(g));
    }

    // Filter
    {
        auto g = std::make_unique<juce::AudioProcessorParameterGroup>("FILT", "FILT", "");

        g->addChild(makeParamInt("FILT.FREQ", "FREQ", 0, 127, 127));
        g->addChild(makeParamInt("FILT.RES", "RES(Q)", 0, 31, 0));
        g->addChild(makeParamInt("FILT.KEYBD", "KEYBD", -63, 63, 0));
        g->addChild(makeParamChoice("FILT.MOD1.SEL", "MOD1.SEL", modulationSources, 0));
        g->addChild(makeParamInt("FILT.MOD1.AMT", "MOD1.AMT", -63, 63, 0));
        g->addChild(makeParamChoice("FILT.MOD2.SEL", "MOD2.SEL", modulationSources, 0));
        g->addChild(makeParamInt("FILT.MOD2.AMT", "MOD2.AMT", -63, 63, 0));

        layout.add(std::move(g));
    }

    // DCA4
    {
        auto g = std::make_unique<juce::AudioProcessorParameterGroup>("DCA4", "DCA4", "");

        g->addChild(makeParamInt("DCA4.AMT", "ENV4", 0, 63, 63));
        g->addChild(makeParamChoice("DCA4.SAT", "SAT", saturationValues, 1));
        g->addChild(makeParamInt("DCA4.PAN", "PAN", -63, 63, 0));
        g->addChild(makeParamChoice("DCA4.AMOD.SEL", "AMOD.SEL", modulationSources, 0));
        g->addChild(makeParamInt("DCA4.AMOD.AMT", "AMOD.AMT", -63, 63, 0));
        g->addChild(makeParamChoice("DCA4.PMOD.SEL", "PMOD.SEL", modulationSources, 0));
        g->addChild(makeParamInt("DCA4.PMOD.AMT", "PMOD.AMT", -63, 63, 0));

        layout.add(std::move(g));
    }

    // Mode
    {
        auto g = std::make_unique<juce::AudioProcessorParameterGroup>("MODE", "MODE", "");

        g->addChild(makeParamChoice("MODE.SYNC", "SYNC", offOn, 0));
        g->addChild(makeParamChoice("MODE.AM", "AM", offOn, 0));
        g->addChild(makeParamChoice("MODE.MONO", "MONO", offOn, 0));
        g->addChild(makeParamInt("MODE.GLIDE", "GLIDE", 0, 63, 63));
        g->addChild(makeParamChoice("MODE.REST.VC", "RESTART VC", offOn, 0));
        g->addChild(makeParamChoice("MODE.REST.ENV", "RESTART ENV", offOn, 0));
        g->addChild(makeParamChoice("MODE.REST.OSC", "RESTART OSC", offOn, 0));
        g->addChild(makeParamChoice("MODE.CYC", "CYC", offOn, 0));

        layout.add(std::move(g));
    }

    // LFO
    for (int i = 0; i < 4; i++) {
        auto lfo = "LFO" + std::to_string(i + 1);
        auto g = std::make_unique<juce::AudioProcessorParameterGroup>(lfo, lfo, "");

        g->addChild(makeParamInt(lfo + ".FREQ", "FREQ", 0, 127, 24));
        g->addChild(makeParamInt(lfo + ".RESET", "RESET", -1, 63, -1)); // TODO custom print function
        g->addChild(makeParamChoice(lfo + ".HUMAN", "HUMAN", lfoHuman, 0));
        g->addChild(makeParamChoice(lfo + ".WAVE", "WAVE", lfoWaves, 0));
        g->addChild(makeParamInt(lfo + ".L1", "L1", 0, 63, 0));
        g->addChild(makeParamInt(lfo + ".DELAY", "DELAY", 0, 63, 63));
        g->addChild(makeParamInt(lfo + ".L2", "L2", 0, 63, 63));
        g->addChild(makeParamChoice(lfo + ".AMOD.SEL", "AMOD.SEL", modulationSources, 0));
        g->addChild(makeParamInt(lfo + ".AMOD.AMT", "AMOD.AMT", -63, 63, 0));
        g->addChild(makeParamChoice(lfo + ".FMOD.SEL", "FMOD.SEL", modulationSources, 0));
        g->addChild(makeParamInt(lfo + ".FMOD.AMT", "FMOD.AMT", -63, 63, 0));
        g->addChild(makeParamInt(lfo + ".PHASE", "PHASE", 0, 127, 0)); // TODO custom print function
        g->addChild(makeParamInt(lfo + ".SMTH", "SMOOTHNESS", 0, 63, 0));
        g->addChild(makeParamChoice(lfo + ".MODE.DELAY", "DELAY MODE", emuSmooth, 0));
        g->addChild(makeParamChoice(lfo + ".MODE.AM", "MAIN MODULATOR", lfoModulatorValues, 0));
        g->addChild(makeParamChoice(lfo + ".MODE.FM", "FREQUENCY MODULATOR", lfoModulatorValues, 0));
        g->addChild(makeParamChoice(lfo + ".MODE.PLAY", "PLAY MODE", lfoPlayModeValues, 0));

        layout.add(std::move(g));
    }

    // ENV
    for (int i = 0; i < 4; i++) {
        auto env = "ENV" + std::to_string(i + 1);
        auto g = std::make_unique<juce::AudioProcessorParameterGroup>(env, env, "");

        g->addChild(makeParamInt(env + ".L0", "L0", -63, 63, 63));
        g->addChild(makeParamInt(env + ".L1", "L1", -63, 63, 63));
        g->addChild(makeParamInt(env + ".L2", "L3", -63, 63, 63));
        g->addChild(makeParamInt(env + ".L3", "L3", -63, 63, 63));
        g->addChild(makeParamInt(env + ".LV", "LV", 0, 127, 0)); // TODO custom print function
        g->addChild(makeParamInt(env + ".T1V", "T1V", 0, 63, 0));
        g->addChild(makeParamInt(env + ".T1", "T1", 0, 63, 0));
        g->addChild(makeParamInt(env + ".T2", "T3", 0, 63, 0));
        g->addChild(makeParamInt(env + ".T3", "T3", 0, 63, 0));
        g->addChild(makeParamInt(env + ".T4", "T4", 0, 127, 0)); // TODO custom print formatter
        g->addChild(makeParamInt(env + ".TK", "TK", 0, 63, 0));
        g->addChild(makeParamChoice(env + ".CYC", "CYCLE", offOn, 0));
        g->addChild(makeParamChoice(env + ".SHAPE", "SHAPE", envShapes, 0));
        g->addChild(makeParamInt(env + ".SMTH", "SMOOTHNESS", 0, 63, 0));
        g->addChild(makeParamChoice(env + ".MODE.T1V", "T1V MODE", envT1VModes, 0));

        layout.add(std::move(g));
    }

    // Modulation matrix
    for (int i = 0; i < 3; i++) {
        auto mat = "MAT" + std::to_string(i + 1);
        auto g = std::make_unique<juce::AudioProcessorParameterGroup>(mat, mat, "");

        g->addChild(makeParamChoice(mat + ".MOD1.SEL", "MOD1.SEL", modulationSources, 0));
        g->addChild(makeParamInt(mat + ".MOD1.AMT", "MOD1.AMT", -63, 63, 0));
        g->addChild(makeParamChoice(mat + ".MOD2.SEL", "MOD2.SEL", modulationSources, 0));
        g->addChild(makeParamInt(mat + ".MOD2.AMT", "MOD2.AMT", -63, 63, 0));
        g->addChild(makeParamChoice(mat + ".MOD3.SEL", "MOD3.SEL", modulationSources, 0));
        g->addChild(makeParamInt(mat + ".MOD3.AMT", "MOD3.AMT", -63, 63, 0));
        g->addChild(makeParamChoice(mat + ".AMOD.SEL", "AMOD.SEL", modulationSources, 0));
        g->addChild(makeParamInt(mat + ".AMOD.AMT", "AMOD.AMT", -63, 63, 0));

        layout.add(std::move(g));
    }

    // Emulation
    {
        auto g = std::make_unique<juce::AudioProcessorParameterGroup>("EMU", "EMU", "");

        g->addChild(makeParamInt("EMU.BRANGE", "BEND", 0, 36, 2));
        g->addChild(makeParamChoice("EMU.BMODE", "MODE", pitchBendModes, 0));
        g->addChild(makeParamChoice("EMU.VSTEAL", "VSTEAL", hardSoft, 1));
        g->addChild(makeParamChoice("EMU.AMBUG", "AMBUG", offOn, 1));
        g->addChild(makeParamChoice("EMU.DCA13", "DCA1-3 SMOOTHING", emuFast, 0));
        g->addChild(makeParamChoice("EMU.DCA4", "DCA4 SMOOTHING", emuFast, 0));
        g->addChild(makeParamChoice("EMU.MUF", "MUFFLE", offOn, 0));
        g->addChild(makeParamChoice("EMU.DCB", "DC-BLOCKING", dcBlocking, 0));

        layout.add(std::move(g));
    }

    return layout;
}


std::unique_ptr<LcdScreen::PageLayout> ParameterHelper::createPageLayout() {
    auto pageLayout = std::make_unique<LcdScreen::PageLayout>();

    {
        auto page1 = LcdScreen::Page::Builder()
                .param("OSC1-3")
                .skip(1).param("OUTPUT=")
                .skip(1).param("", 1, "DCA1.OUTPUT", 3)
                .skip(5).param("", 2, "DCA2.OUTPUT", 3)
                .skip(5).param("", 3, "DCA3.OUTPUT", 3)
                .skip(2).param("FINAL=", 4, "DCA4.AMT", 2)
                .newline()
                .skip(7).param("LEVEL=")
                .skip(2).param("", 6, "DCA1.LEVEL", 2)
                .skip(6).param("", 7, "DCA2.LEVEL", 2)
                .skip(6).param("", 8, "DCA3.LEVEL", 2)
                .skip(3).param("SAT=", 9, "DCA4.SAT", 3)
                .build();

        auto page2 = LcdScreen::Page::Builder()
                .param("OSC1-3")
                .skip(1).param("WAVE=")
                .skip(1).param("", 1, "OSC1.WAVE", 6)
                .skip(2).param("", 2, "OSC2.WAVE", 6)
                .skip(2).param("", 3, "OSC3.WAVE", 6)
                .skip(1).param("SYNC=", 4, "MODE.SYNC", 3)
                .newline()
                .skip(7).param("OCT=")
                .skip(4).param("", 6, "OSC1.OCT", 2)
                .skip(6).param("", 7, "OSC2.OCT", 2)
                .skip(6).param("", 8, "OSC3.OCT", 2)
                .skip(3).param("AM=", 6, "MODE.AM", 3)
                .build();

        auto page3 = LcdScreen::Page::Builder()
                .param("OSC1-3")
                .skip(1).param("SEMI=")
                .skip(3).param("", 1, "OSC1.SEMI", 3)
                .skip(5).param("", 2, "OSC2.SEMI", 3)
                .skip(5).param("", 3, "OSC3.SEMI", 3)
                .skip(2).param("SYNC=", 4, "MODE.SYNC", 3).newline()
                .skip(7).param("FINE=")
                .skip(3).param("", 6, "OSC1.FINE", 3)
                .skip(5).param("", 7, "OSC2.FINE", 3)
                .skip(5).param("", 8, "OSC3.FINE", 3)
                .skip(2).param("AM=", 6, "MODE.AM", 3)
                .build();

        auto group = std::make_unique<LcdScreen::PageGroup>();
        group->addPage(std::move(page1));
        group->addPage(std::move(page2));
        group->addPage(std::move(page3));
        pageLayout->addPageGroup(std::move(group));
    }

    for (int i = 0; i < 3; i++) {
        auto pf = "OSC" + std::to_string(i+1);

        auto page1 = LcdScreen::Page::Builder()
                .param(pf)
                .skip(1).param("OCT=", 0, pf + ".OCT", 2)
                .skip(1).param("SEMI=", 1, pf + ".SEMI", 3)
                .skip(1).param("FINE=", 2, pf + ".FINE", 3)
                .skip(4).param("WAVE=", 4, pf + ".WAVE", 6)
                .newline()
                .skip(5).param("MODS=")
                .skip(4).param("", 6, pf + ".MOD1.SEL", 6)
                .skip(1).param("*")
                .skip(1).param("", 7, pf + ".MOD1.AMT", 3)
                .skip(4).param("", 8, pf + ".MOD2.SEL", 6)
                .skip(1).param("*")
                .skip(1).param("", 9, pf + ".MOD2.AMT", 3)
                .build();

        auto pf2 = "DCA" + std::to_string(i+1);

        auto page2 = LcdScreen::Page::Builder()
                .param(pf2)
                .skip(1).param("LEVEL=", 0, pf2 + ".LEVEL", 2)
                .skip(4).param("OUTPUT=", 2, pf2 + ".OUTPUT", 3)
                .newline()
                .skip(5).param("MODS=")
                .skip(4).param("", 6, pf2 + ".MOD1.SEL", 6)
                .skip(1).param("*")
                .skip(1).param("", 7, pf2 + ".MOD1.AMT", 3)
                .skip(4).param("", 8, pf2 + ".MOD2.SEL", 6)
                .skip(1).param("*")
                .skip(1).param("", 9, pf2 + ".MOD2.AMT", 3)
                .build();

        auto group = std::make_unique<LcdScreen::PageGroup>();
        group->addPage(std::move(page1));
        group->addPage(std::move(page2));

        pageLayout->addPageGroup(std::move(group));
    }

    {
        auto page = LcdScreen::Page::Builder()
                .param("FILT")
                .skip(1).param("FREQ=", 0, "FILT.FREQ", 3)
                .skip(1).param("RES(Q)=", 1, "FILT.RES", 2)
                .skip(3).param("KEYBD * ", 3, "FILT.KEYBD", 3).newline()
                .skip(5).param("MODS=")
                .skip(3).param("", 6, "FILT.MOD1.SEL", 6)
                .skip(1).param("*")
                .skip(1).param("", 7, "FILT.MOD1.AMT", 4)
                .skip(3).param("", 8, "FILT.MOD2.SEL", 6)
                .skip(1).param("*")
                .skip(1).param("", 9, "FILT.MOD2.AMT", 4)
                .build();

        auto group = std::make_unique<LcdScreen::PageGroup>();
        group->addPage(std::move(page));
        pageLayout->addPageGroup(std::move(group));
    }

    {
        auto page = LcdScreen::Page::Builder()
                .param("DCA4")
                .skip(1).param("ENV4 * ", 0, "DCA4.AMT", 2)
                .skip(6).param("SAT=", 2, "DCA4.SAT", 3)
                .skip(3).param("PAN=", 3, "DCA4.PAN", 3)
                .newline()
                .skip(5).param("MODS=")
                .skip(4).param("", 6, "DCA4.AMOD.SEL", 6)
                .skip(1).param("*")
                .skip(1).param("", 7, "DCA4.AMOD.AMT", 3)
                .skip(4).param("", 8, "DCA4.PMOD.SEL", 6)
                .skip(1).param("*")
                .skip(1).param("", 9, "DCA4.PMOD.AMT", 4)
                .build();

        auto group = std::make_unique<LcdScreen::PageGroup>();
        group->addPage(std::move(page));
        pageLayout->addPageGroup(std::move(group));
    }

    {
        auto page1 = LcdScreen::Page::Builder()
                .param("MODE")
                .skip(1).param("SYNC=", 0, "MODE.SYNC", 3)
                .skip(1).param("AM=", 1, "MODE.AM", 3)
                .skip(6).param("MONO=", 3, "MODE.MONO", 3)
                .skip(1).param("GLIDE=", 4, "MODE.GLIDE", 2).newline()
                .skip(5).param("RESTART-")
                .skip(1).param("VC=", 6, "MODE.REST.VC", 3)
                .skip(1).param("ENV=", 7, "MODE.REST.ENV", 3)
                .skip(1).param("OSC=", 8, "MODE.REST.OSC", 3)
                .skip(1).param("CYC=", 9, "MODE.CYC", 3)
                .build();

        auto page2 = LcdScreen::Page::Builder()
                .param("EMU")
                .skip(2).param("BEND=", 0, "EMU.BRANGE", 2)
                .skip(1).param("MODE=", 1, "EMU.BMODE", 5)
                .skip(10).param("VSTEAL=", 4, "EMU.VSTEAL", 4)
                .newline()
                .skip(5).param("AMBUG=", 5, "EMU.AMBUG", 3)
                .build();

        auto page3 = LcdScreen::Page::Builder()
                .param("EMU")
                .skip(1).param("SMOOTH-")
                .skip(1).param("DCA1-3=", 1, "EMU.DCA13", 4)
                .skip(4).param("MUFFLE=", 3, "EMU.MUF", 3)
                .newline()
                .skip(12).param("DCA4=", 6, "EMU.DCA4", 4)
                .skip(5).param("DC-BLOCK=", 8, "EMU.DCB", 5)
                .build();

        auto group = std::make_unique<LcdScreen::PageGroup>();
        group->addPage(std::move(page1));
        group->addPage(std::move(page2));
        group->addPage(std::move(page3));
        pageLayout->addPageGroup(std::move(group));
    }

    for (int i = 0; i < 4; i++) {
        auto lfo = "LFO" + std::to_string(i+1);

        auto page1 = LcdScreen::Page::Builder()
                .param(lfo)
                .skip(1).param("FREQ=", 0, lfo + ".FREQ", 3)
                .skip(1).param("RESET=", 1, lfo + ".RESET", 3)
                .skip(2).param("HUMAN=", 3, lfo + ".HUMAN", 3)
                .skip(2).param("WAV=", 4, lfo + ".WAVE", 4)
                .newline()
                .skip(5).param("L1=", 5, lfo + ".L1", 2)
                .skip(1).param("DELAY=", 6, lfo + ".DELAY", 2)
                .skip(1).param("L2=", 7, lfo + ".L2", 2)
                .skip(2).param("MOD=")
                .skip(1).param("", 8, lfo + ".AMOD.SEL", 6)
                .skip(1).param("*")
                .skip(1).param("", 9, lfo + ".AMOD.AMT", 3)
                .build();

        auto page2 = LcdScreen::Page::Builder()
                .param(lfo)
                .skip(1).param("FREQMOD=")
                .skip(1).param("", 1, lfo + ".FMOD.SEL", 6)
                .skip(1).param("*")
                .skip(1).param("", 2, lfo + ".FMOD.AMT", 4)
                .skip(2).param("PHS=", 3, lfo + ".PHASE", 3)
                .skip(1).param("SMTH=", 4, lfo + ".SMTH", 2)
                .newline()
                .skip(5).param("MODE-")
                .skip(1).param("DELAY=", 6, lfo + ".MODE.DELAY", 4)
                .skip(1).param("AM=", 7, lfo + ".MODE.AM", 3)
                .skip(1).param("FM=", 8, lfo + ".MODE.FM", 3)
                .skip(1).param("PLAY=", 9, lfo + ".MODE.PLAY", 3)
                .build();

        auto group = std::make_unique<LcdScreen::PageGroup>();
        group->addPage(std::move(page1));
        group->addPage(std::move(page2));
        pageLayout->addPageGroup(std::move(group));
    }

    for (int i = 0; i < 4; i++) {
        auto env = "ENV" + std::to_string(i+1);

        auto page1 = LcdScreen::Page::Builder()
                .param(env)
                .skip(1).param("L1=", 0, env + ".L1", 3)
                .skip(2).param("L2=", 1, env + ".L2", 3)
                .skip(2).param("L3=", 2, env + ".L3", 3)
                .skip(2).param("LV=", 3, env + ".LV", 3)
                .skip(3).param("T1V=", 4, env + ".T1V", 2)
                .newline()
                .skip(5).param("T1=", 5, env + ".T1", 2)
                .skip(3).param("T2=", 6, env + ".T2", 2)
                .skip(3).param("T3=", 7, env + ".T3", 2)
                .skip(3).param("T4=", 8, env + ".T4", 3)
                .skip(3).param("TK=", 9, env + ".TK", 2)
                .build();

        auto page2 = LcdScreen::Page::Builder()
                .param(env)
                .skip(1).param("L0=", 0, env + ".L0", 3)
                .skip(2).param("CYC=", 1, env + ".CYC", 3)
                .skip(6).param("SHAPE=", 3, env + ".SHAPE", 4)
                .skip(1).param("SMTH=", 4, env + ".SMTH", 2)
                .newline()
                .skip(5).param("MODE-")
                .skip(3).param("T1V=", 6, env + ".T1V", 3)
                .build();

        auto group = std::make_unique<LcdScreen::PageGroup>();
        group->addPage(std::move(page1));
        group->addPage(std::move(page2));
        pageLayout->addPageGroup(std::move(group));
    }

    for (int i = 0; i < 4; i++) {
        auto mat = "MAT" + std::to_string(i + 1);

        auto page = LcdScreen::Page::Builder()
                .param(mat)
                .skip(6).param("M1=")
                .skip(1).param("", 1, mat + ".MOD1.SEL", 6)
                .skip(1).param("*")
                .skip(1).param("", 2, mat + ".MOD1.AMT", 3)
                .skip(2).param("M2=")
                .skip(1).param("", 3, mat + ".MOD2.SEL", 6)
                .skip(1).param("*")
                .skip(1).param("", 4, mat + ".MOD2.AMT", 3)
                .newline()
                .skip(10).param("M3=")
                .skip(1).param("", 6, mat + ".MOD3.SEL", 6)
                .skip(1).param("*")
                .skip(1).param("", 7, mat + ".MOD3.AMT", 3)
                .skip(1).param("AMP=")
                .skip(1).param("", 8, mat + ".AMOD.SEL", 6)
                .skip(1).param("*")
                .skip(1).param("", 9, mat + ".AMOD.AMT", 3)
                .build();

        auto group = std::make_unique<LcdScreen::PageGroup>();
        group->addPage(std::move(page));
        pageLayout->addPageGroup(std::move(group));
    }


    return pageLayout;
}
