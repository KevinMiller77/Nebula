#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include "MacWindow.h"

#include "../../Graphics/Metal/MetalContext.h"
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include <AppKit/AppKit.hpp>

namespace Nebula {

    // Instantiate static data
    MacData MacWindow::m_Data;

    /* Mac Metal Context Internal below */
    void MetalContext::SetVSync_Int(bool vsync) {}
    bool MetalContext::SwapBuffers_Int() { return false; }
    bool MetalContext::InitContext_Int(void* window) { return false; }
   /* End Mac Metal Context Internal */

    /* Begin App Delegate Internal */
    NS::Menu* MacWindow::CreateMenuBar() { return nullptr; };

    void MacWindow::applicationWillFinishLaunching( NS::Notification* pNotification ) {};
    bool MacWindow::applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender ) { 
        LOG_INF("Terminate\n");
        return true; 
    };
    
    void MacWindow::applicationDidFinishLaunching( NS::Notification* pNotification ) {
        LOG_INF("[MacOS] Creating window inside delegate\n");
        CreateWindow_Int();

        // Stop the application so we can do our own polling; Apple ate our run loop :( 
        m_Application->activateIgnoringOtherApps(true);
        m_Application->stop(pNotification->object());

        LOG_INF("[MacOS] Stopped Mac App Delegate\n");
    };
    /* End App Delegate Internal */

    NS::Event* MacWindow::HandleEvent_Int(NS::Event* event) {
        NS::EventType type = event->type();
        // If event is a close event, close the window
        switch (type) {
            case(NS::EventTypeApplicationDefined):
            case(NS::EventTypeSystemDefined):
            case(NS::EventTypeAppKitDefined):
            case(NS::EventTypePeriodic): 
            {
                NS::EventSubtype subtype = event->subtype();
                if (subtype & NS::EventSubtypeApplicationDeactivated) {
                    // WindowCloseEvent event = WindowCloseEvent();
                    // m_Data.EventCallback(event);
                }
                break;
            }
            default:
                // LOG_INF("Event type=%d\n", type);
                break;

        }

        return event;
    }

    void MacWindow::CreateWindow_Int() {
        CGRect frame = (CGRect){ 
            {(double)m_Data.posX,  (double)m_Data.posY}, 
            {(double)m_Data.width, (double)m_Data.height} 
        };
        m_Window = NS::Window::alloc()->init(
            frame,      // Size
            NS::WindowStyleMaskClosable|NS::WindowStyleMaskTitled|NS::WindowStyleMaskResizable, // Flags
            NS::BackingStoreBuffered, // (Only option): The window renders all drawing into a display buffer and then flushes it to the screen.
            false ); // Do not defer creation

        m_Context = CreateRef<MetalContext>(m_Window);
        // m_Context->Init();

        MTL::Device* _pDevice = MTL::CreateSystemDefaultDevice();

        // Create a view from the device to draw to
        m_ContentView = MTK::View::alloc()->init( frame, _pDevice );

        // Create and set custome view delegate which holds our renderer
        m_ViewDelegate = new NebulaViewDelegate();
        m_ContentView->setDelegate( m_ViewDelegate );

        // Set the view as the main window content
        m_Window->setContentView( m_ContentView );

        // Do all window metadata setup here - there is probably more than title
        m_Window->setTitle( NS::String::string( m_Data.title.c_str(), NS::StringEncoding::UTF8StringEncoding ) );

        // Show the window as the applications main screen
        m_Window->makeKeyAndOrderFront( nullptr );
    }

    MacWindow::MacWindow(WindowInfo info) {
        m_Data = {
            200, 200,
            100, 100,
            info.Width, info.Height,
            100, 100
        };
        m_Data.title = info.Title;
        SetEventCallback(info.EventCallback);

        // Begin MacOS Application (Pass to Mac then await it coming back to us)
        m_Pool = NS::AutoreleasePool::alloc()->init();

        m_Application = NS::Application::sharedApplication();
        m_Application->setDelegate(this);

        LOG_INF("[MacOS] Started Mac Application delegate\n");
        m_Application->run();

        m_Application->setActivationPolicy(NS::ActivationPolicyRegular);
    }

    void MacWindow::WindowDidCreateCB (NS::Window* window) {
        m_Window = window;
    }

    MacWindow::~MacWindow() {
        m_Pool->release();
    }

    void MacWindow::HandleError() {
        // Fill in with your error handling code
    }

    void MacWindow::OnUpdate() {
        // Poll MacOSX Events (To be moved)
        for (;;) {
            NS::Event* evt = m_Application->nextEventMatchingMask( NS::EventMaskAny, nullptr, NS::String::string( "NSDefaultRunLoopMode", NS::StringEncoding::UTF8StringEncoding ), true );
            if (evt == nullptr){
                break;
            }

            // Handle event in Nebula, if return is null do not pass back to Apple
            if (HandleEvent_Int(evt) == nullptr) {
                continue;
            }
            m_Application->sendEvent( evt );
        }

        // Fill in with your window update code
        m_Context->SwapBuffers();
    }

    float MacWindow::GetTime() {
        // Fill in with your time getting code
        return 0.0f;
    }

    uint32 MacWindow::GetWidth() const {
        return m_Data.width;
    }

    uint32 MacWindow::GetHeight() const {
        return m_Data.height;
    }

    void MacWindow::SetWindowSize(uint32 width, uint32 height) {
        m_Data.width = width;
        m_Data.height = height;
        // Also update the actual window size
    }

    void MacWindow::SetResizeable(bool resizeable) {
        m_Data.resizable = resizeable;
        // Also update the actual window property
    }

    void MacWindow::SetIcon(std::string filepath) {
        // Fill in with your window icon setting code
    }

    void MacWindow::MaximizeWindow() {
        m_Data.maximized = true;
        // Also update the actual window state
    }

    void MacWindow::RestoreWindow() {
        m_Data.maximized = false;
        m_Data.minimized = false;
        // Also update the actual window state
    }

    void MacWindow::MinimizeWindow() {
        m_Data.minimized = true;
        // Also update the actual window state
    }

    Vec2u MacWindow::GetMaxWindowSize() {
        // Fill in with your maximum window size calculation
        return Vec2u();
    }

    void MacWindow::SetPassthrough(bool enabled) {
        m_Data.mousePassthrough = enabled;
        // Also update the actual window property
    }

    void MacWindow::SetFloating(bool enabled) {
        m_Data.floating = enabled;
        // Also update the actual window property
    }

    void MacWindow::SetTransparentFramebuffer(bool enabled) {
        m_Data.transparent = enabled;
        // Also update the actual window property
    }

    void MacWindow::SetDecorated(bool enabled) {
        m_Data.decorated = enabled;
        // Also update the actual window property
    }

    void MacWindow::SetVSync(bool enabled) {
        m_Data.vsync = enabled;
        // Also update the actual window property
    }

    bool MacWindow::IsVSync() const {
        return m_Data.vsync;
    }

    void* MacWindow::GetNativeWindow() {
        return m_Window;
    }

    void MacWindow::SwapIO(std::string in, std::string out, std::string err) {
        // Fill in with your IO swapping code
    }

    void MacWindow::EnableConsole() {
        // Fill in with your console enabling code
    }

    void MacWindow::DisableConsole() {
        // Fill in with your console disabling code
    }

    void MacWindow::ShutDown() {
        // Fill in with your shutdown code
    }

    void MacWindow::ToggleFullscreen() {
        m_Data.fullscreen = !m_Data.fullscreen;
        // Also update the actual window state
    }

    uint32* MacWindow::GetWidthPtr() const {
        return &m_Data.width;
    }

    uint32* MacWindow::GetHeightPtr() const {
        return &m_Data.height;
    }

} // namespace Nebula
