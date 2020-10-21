#pragma once
#include <imgui.h>

#include <Utils/Timer.h>
#include <Utils/Logging.h>

#include <fstream>
#include <iostream>

namespace Nebula
{
    class LogPanel
    {
        //Log props
        int filePlace;
        FILE* logFile = nullptr;
        
        ImGuiTextBuffer     Buf;
        ImGuiTextFilter     Filter;
        ImVector<int>       LineOffsets;    // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
        bool                AutoScroll;     // Keep scrolling if already at the bottom

    public:

        LogPanel();
        ~LogPanel();
        void Clear();
        void AddLog(const char* fmt, ...) IM_FMTARGS(2);
        void UpdateBuffer();
        void Update(bool& LogOpen);
        void Draw(const char* title, bool* p_open = NULL);
    };
}