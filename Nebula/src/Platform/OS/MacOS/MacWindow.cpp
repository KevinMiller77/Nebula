#include <Events/InputCodes.h>
#include <Events/KeyEvent.h>
#include <Foundation/NSTypes.hpp>
#include <vector>
#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include "MacWindow.h"
#include "MacInput.h"
#include <Utils/Logging.h>

#include "../../Graphics/Metal/MetalContext.h"
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include <AppKit/AppKit.hpp>
#include <AppKit/NSEvent.hpp>
#include <AppKit/NSWindow.hpp>
#include <Foundation/NSString.hpp>
#include <Foundation/NSNotification.hpp>
#include <Foundation/NSNotificationCenter.hpp>

#include <Platform/Graphics/Metal/MetalContext.h>

namespace Nebula {

    Vec2f s_CurrentMouseLocation = Vec2(0.0f, 0.0f);

    // Instantiate static data
    MacData MacWindow::m_Data;

    /* Mac Metal Context Internal below */
    void MetalContext::SetVSync_Int(bool vsync) {}
    bool MetalContext::SwapBuffers_Int() { return false; }
    bool MetalContext::InitContext_Int(void* window) {
        m_BoundView = (MTK::View*)(((NS::Window*)window)->contentView());
        m_DefaultContentView = m_BoundView;
        m_DefaultContentView->setFramebufferOnly(false);
        return (m_BoundView != nullptr);
    }

   /* End Mac Metal Context Internal */

    /* Begin App Delegate Internal */
    NS::Menu* MacWindow::CreateMenuBar() { return nullptr; };

    void MacWindow::handleNotification(NS::Notification* pNotification) {
        if (pNotification == nullptr) {
            return;
        }

        void* object = pNotification->object();
        if (object != m_Application && object != m_Window) {
            return;
        }

        // Application launched, lets try to create a window
        if (std::string(pNotification->name()->utf8String()) == "NSApplicationDidFinishLaunchingNotification") {
            LOG_INF("[MacOS] App launched. Stopping the app to take control of main event loop.\n");
            m_Application->stop(nullptr);
        }

        // Window closed, we should likely terminate.
        if (std::string(pNotification->name()->utf8String()) == "NSWindowDidCloseNotification") {
            LOG_INF("[MacOS] App terminating\n");
            // Window closed, we should likely close the app
            WindowCloseEvent event = WindowCloseEvent();
            m_Data.EventCallback(event);
        }

        // Window changed size
        if (std::string(pNotification->name()->utf8String()) == "NSWindowDidResizeNotification") {
            LOG_INF("[MacOS] Window resized\n");

            NS::Rect frame = m_Window->frame();
            m_Data.width = frame.size.width;
            m_Data.height = frame.size.height;

            // Window resized, we should likely update the window size
            WindowResizeEvent event = WindowResizeEvent( { m_Data.width, m_Data.height } );
            m_Data.EventCallback(event);
        }

        // TODO (Kevin): Support more notification types
        // LOG_INF("[MacOS] Notification: %s\n", pNotification->name()->utf8String());
    }

    /* End App Delegate Internal */
    NS::Event* MacWindow::HandleEvent_Int(NS::Event* event) {
        if (event == nullptr) {
            return nullptr;
        }

        NS::EventType type = event->type();
        // If event is a close event, close the window
        switch (type) {
            case(NS::EventTypeApplicationDefined):
            case(NS::EventTypeSystemDefined):
            case(NS::EventTypeAppKitDefined):
            case(NS::EventTypePeriodic): 
            {
                NS::EventSubtype subtype = event->subtype();
                break;
            }

            /*
                Mouse events
            */
            case (NS::EventTypeRightMouseDown): 
            case (NS::EventTypeLeftMouseDown): 
            case (NS::EventTypeOtherMouseDown): {
                MouseCode key;
                if (type == NS::EventTypeRightMouseDown) {
                    key = MouseCode::ButtonRight;
                } else if (type == NS::EventTypeLeftMouseDown) {
                    key = MouseCode::ButtonLeft;
                } else {
                    NS::Integer buttonNumber = event->m_ButtonNumber();
                    key = buttonNumber == 2 ? MouseCode::ButtonMiddle 
                        : buttonNumber == 3 ? MouseCode::ButtonLast : MouseCode::ButtonNext;
                }

                MouseButtonPressedEvent event(key);
                m_Data.EventCallback(event);
                Input::SetMouseButtonPressed(key);
                break;
            }
            case (NS::EventTypeLeftMouseUp):
            case (NS::EventTypeRightMouseUp):
            case(NS::EventTypeOtherMouseUp): {
                MouseCode key;
                if (type == NS::EventTypeRightMouseDown) {
                    key = MouseCode::ButtonRight;
                } else if (type == NS::EventTypeLeftMouseDown) {
                    key = MouseCode::ButtonLeft;
                } else {
                    NS::Integer buttonNumber = event->m_ButtonNumber();
                    key = buttonNumber == 2 ? MouseCode::ButtonMiddle 
                        : buttonNumber == 3 ? MouseCode::ButtonLast : MouseCode::ButtonNext;
                }

                MouseButtonReleasedEvent event(key);
                m_Data.EventCallback(event);
                Input::SetMouseButtonReleased(key);
                break;
            }
            case (NS::EventTypeLeftMouseDragged):
            case (NS::EventTypeRightMouseDragged):
            case (NS::EventTypeOtherMouseDragged): {
                NS::Point location = event->m_MouseLocation();
                Vec2f pos = Vec2f(location.x, m_Data.height - location.y);
                MouseMovedEvent event(pos);
                m_Data.EventCallback(event);

                s_CurrentMouseLocation = pos;
                break;
            }

            case (NS::EventTypeScrollWheel): {
                NS::Point location = event->m_MouseLocation();
                NS::Float deltaX = event->m_DeltaX();
                NS::Float deltaY = event->m_DeltaY();
                MouseScrolledEvent event(deltaX, deltaY);
                m_Data.EventCallback(event);
                break;
            }
            case (NS::EventTypeMouseMoved): {
                NS::Point location = event->m_MouseLocation();
                Vec2f pos = Vec2f(location.x, m_Data.height - location.y);
                MouseMovedEvent event(pos);
                m_Data.EventCallback(event);

                s_CurrentMouseLocation = pos;
                break;
            }

            /*
                Keyboard events
            */

            case(NS::EventTypeKeyDown): {
                auto appleKeycode = event->kb_Keycode();

                if (MacInput::KeyCodeMapMacToNeb.find(appleKeycode) != MacInput::KeyCodeMapMacToNeb.end()) {
                    auto nebulaKeycode = MacInput::KeyCodeMapMacToNeb[appleKeycode];
                    // LOG_INF("Key Down: %d\n", (int)nebulaKeycode);


                    Input::SetKeyPressed(nebulaKeycode);
                    KeyPressedEvent event((int)nebulaKeycode, 0); // TODO: Add repeatCount
                    m_Data.EventCallback(event);
                }
                break;
            }
            case(NS::EventTypeKeyUp): {
                auto appleKeycode = event->kb_Keycode();

                if (MacInput::KeyCodeMapMacToNeb.find(appleKeycode) != MacInput::KeyCodeMapMacToNeb.end()) {
                    auto nebulaKeycode = MacInput::KeyCodeMapMacToNeb[appleKeycode];
                    // LOG_INF("Key Up: %d\n", (int)nebulaKeycode);
                    
                    Input::SetKeyReleased(nebulaKeycode);
                    KeyReleasedEvent event((int)nebulaKeycode);
                    m_Data.EventCallback(event);
                }
                break;
            }
            case(NS::EventTypeFlagsChanged): {
                NS::Integer flag = event->kb_ModifierFlags();
                
                std::vector<KeyCode> keysOn = {};
                std::vector<KeyCode> keysOff = {};
                flag & NS::EventModifierFlagCapsLock ?
                    keysOn.push_back(KeyCode::CapsLock) :
                    keysOff.push_back(KeyCode::CapsLock);
                
                flag & NS::EventModifierFlagShift ?
                    keysOn.push_back(KeyCode::LeftShift) :
                    keysOff.push_back(KeyCode::LeftShift);

                flag & NS::EventModifierFlagControl ?
                    keysOn.push_back(KeyCode::LeftControl) :
                    keysOff.push_back(KeyCode::LeftControl);

                flag & NS::EventModifierFlagOption ?
                    keysOn.push_back(KeyCode::Alt) :
                    keysOff.push_back(KeyCode::Alt);

                flag & NS::EventModifierFlagCommand ?
                    keysOn.push_back(KeyCode::LeftSuper) :
                    keysOff.push_back(KeyCode::LeftSuper);

                flag & NS::EventModifierFlagNumericPad ?
                    keysOn.push_back(KeyCode::NumLock) :
                    keysOff.push_back(KeyCode::NumLock);

                // flag & NS::EventModifierFlagHelp ?
                //     keysOn.push_back(KeyCode::Help) :
                //     keysOff.push_back(KeyCode::Help);

                // flag & NS::EventModifierFlagFunction ?
                //     keysOn.push_back(KeyCode::Function) :
                //     keysOff.push_back(KeyCode::Function);

                for (KeyCode key: keysOn) {
                    KeyPressedEvent event((int)key, 0);
                    m_Data.EventCallback(event);
                    Input::SetKeyPressed(key);
                }

                for (KeyCode key: keysOff) {
                    KeyReleasedEvent event((int)key);
                    m_Data.EventCallback(event);
                    Input::SetKeyReleased(key);
                }

                // LOG_INF("Flags changed\n");
                // LOG_INF("Flags: %d\n", (int)event->kb_ModifierFlags());
            }
            default: {
                break;
            }
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

        if (m_Window == nullptr) {
            LOG_ERR("Could not create window!\n");
            return;
        }

        m_Context = CreateRef<MetalContext>(m_Window);
        
        MTL::Device* _pDevice = MTL::CreateSystemDefaultDevice();
        
        // Create a view from the device to draw to
        MTK::View* view = MTK::View::alloc()->init( frame, _pDevice );

        // Set the view as the main window content
        m_Window->setContentView( view );

        m_Context->Init();
        
        // Do all window metadata setup here - there is probably more than title
        m_Window->setTitle( NS::String::string( m_Data.title.c_str(), NS::StringEncoding::UTF8StringEncoding ) );

        // Show the window as the applications main screen
        m_Window->makeKey();
        m_Window->orderFrontRegardless();
    }

    MacWindow::MacWindow(WindowInfo info) {
        m_Data = {
            1600, 900,
            100, 100,
            info.Width, info.Height,
            100, 100
        };
        m_Data.title = info.Title;
        SetEventCallback(info.EventCallback);

        // Create the autorelease pool
        m_Pool = m_Pool ? m_Pool : NS::AutoreleasePool::alloc()->init();
        if (m_Pool == nullptr) {
            LOG_ERR("Could not create autorelease pool!\n");
        }

        // Create the application
        m_Application = NS::Application::sharedApplication();

        // Set this as the main notification center observer
        this->m_pNotificationCenter = NS::NotificationCenter::defaultCenter();
        this->m_pNotificationCenter->addObserver(this, nullptr, nullptr);

        // Setup some application parameters
        m_Application->setActivationPolicy(NS::ActivationPolicyRegular);
        m_Application->activateIgnoringOtherApps(true);

        // When application does run, we immediately stop it
        // This is a hack to get all of the benefits of the default
        // App delegate but we have our own event loop
        m_Application->run();

        CreateWindow_Int();
    }

    void MacWindow::WindowDidCreateCB (NS::Window* window) {
        m_Window = window;
    }

    MacWindow::~MacWindow() {
        if (m_Pool) {
            m_Pool->release();
        }
    }

    void MacWindow::HandleError() {
        // Fill in with your error handling code
    }

    void MacWindow::OnUpdate() {
        NS::Event* evt = m_Application->nextEventMatchingMask( 
            NS::EventMaskAny,
            nullptr,
            NS::String::string("kCFRunLoopDefaultMode", NS::StringEncoding::UTF8StringEncoding),
            true 
        );

        if (evt){
            // Handle event in Nebula, if return is null do not pass back to Apple
            if (HandleEvent_Int(evt)) {
                m_Application->sendEvent( evt );
                m_Application->updateWindows();
            }
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

        NS::Rect frame = m_Window->frame();
        frame.size.width = width;
        frame.size.height = height;
        
        m_Window->setFrame(frame, true);
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

    
    bool MacInput::IsKeyPressedAsyncInt(KeyCode key) { 
        return Input::IsKeyPressedInt(key); 
    }
    bool MacInput::IsMouseButtonPressedAsyncInt(MouseCode key) { 
        return Input::IsMouseButtonPressedInt(key);
    }
    Vec2f MacInput::GetMousePosInt() { 
        // LOG_INF("Got mouse pos %f, %f\n", s_CurrentMouseLocation.X, s_CurrentMouseLocation.Y);
        return s_CurrentMouseLocation;
    }
    void  MacInput::SetMousePosInt(Vec2f newPos) {
        
    }

} // namespace Nebula
