﻿#define LONGUI_WITH_DEFAULT_HEADER
#define _CRT_SECURE_NO_WARNINGS
#include "../../LongUI/LongUI.h"
#include "../Demo3_handleeventex/demo.h"

#define LONGUI_NAMESPACE_BEGIN2 namespace LongUI { namespace
#define LONGUI_NAMESPACE_END2 }

// longui::demo namespace
LONGUI_NAMESPACE_BEGIN2 Demo {
    // config
    class MyConfig final : public CUIDefaultConfigure {
        // super class
        using Super = CUIDefaultConfigure;
    public:
        // ctor
        MyConfig() : Super(UIManager) { }
        // return true, if using cpu rendering
        virtual auto IsRenderByCPU() noexcept->bool override { 
            return true; 
        }
    };
}
LONGUI_NAMESPACE_END2

// Entry for App
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow) {
    // every windows desktop app should do this
    ::HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);
    // use OleInitialize to init ole and com
    if (SUCCEEDED(::OleInitialize(nullptr))) {
        // my config
        LongUI::Demo::MyConfig config;
        // init longui manager
        if (SUCCEEDED(UIManager.Initialize(&config))) {
            // my style
            UIManager << DL_Hint << L"Battle Control Online!" << LongUI::endl;
            // create main window, return nullptr for some error
            UIManager.CreateUIWindow<LongUI::Demo::MainWindow>(DEMO_XML);
            // run this app
            UIManager.Run();
            // my style
            UIManager << DL_Hint << L"Battle Control Terminated!" << LongUI::endl;
            // cleanup longui
            UIManager.UnInitialize();
        }
    }
    // cleanup ole and com
    ::OleUninitialize();
    // exit
    return EXIT_SUCCESS;
}
