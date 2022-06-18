#pragma once

namespace Nebula {

    enum class ResizeEventType {
        None,
        Resize,
        Maximize,
        Minimize
    };
    
    struct WindowEvents {
        void (*resizeEvent)(unsigned short, unsigned short, ResizeEventType) = nullptr;
    };

    
}