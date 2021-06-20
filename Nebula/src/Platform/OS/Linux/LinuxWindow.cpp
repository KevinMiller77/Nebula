#include "LinuxWindow.h"

#ifdef NEB_PLATFORM_LINUX
#include <stb_image/stb_image.h>
#include <GLFW/glfw3.h>

#include <Core/VFS.h>
#include <Nebula_pch.h>

namespace Nebula{
    WindowsData LinuxWindow::data = WindowsData();

    void GLFWErrorCallback(int error, const char* decsription)
    {
        LOG_ERR("GLFW ERR [%X]: %s\n", error, decsription);
    }

    LinuxWindow::~LinuxWindow()
    {
        ShutDown();
    }

    void LinuxWindow::CallWindowHints()
    {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_DECORATED, true);
    }

    void LinuxWindow::SwapIO(std::string in, std::string out, std::string err)
    {
        freopen(in.c_str(), "r+", stdin);
        freopen(out.c_str(), "w", stdout);
        freopen(err.c_str(), "w", stderr);

        std::ifstream t("tmpout.txt");
        if (t.is_open())
        {
            std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
            printf("%s", str.c_str());
        }

    }

    void LinuxWindow::EnableConsole()
    {
        // ::ShowWindow(::GetConsoleWindow(), SW_SHOW);
    }

    void LinuxWindow::DisableConsole()
    {
        // ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    }

    LinuxWindow::LinuxWindow(WindowInfo inf)
        : context(nullptr), GLFWWinCount(0)
    {
        NEB_PROFILE_FUNCTION();
        info = inf;
        data.height = info.Height; data.width = info.Width;

        LOG_INF("Creating window\n");

        if (GLFWWinCount == 0)
        {
            if (!glfwInit())

            {
                LOG_ERR("Could not init GLFW!!\n");
            }
            else
            {
                LOG_INF("GLFW Init\n");
            }
            glfwSetErrorCallback(GLFWErrorCallback);
        }
        CallWindowHints();

        window = glfwCreateWindow((int)data.width, (int)data.height, info.Title, nullptr, nullptr);

        context = GraphicsContext::Create((void*)window);
        context->Init();

        glfwSetWindowUserPointer(window, &data);
        SetVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
        {
            WindowsData& data = *(WindowsData*)glfwGetWindowUserPointer(window);
            data.width = width;
            data.height = height;

            WindowResizeEvent event(Vec2u(data.width, data.height));
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
        {
            WindowsData& data = *(WindowsData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowsData& data = *(WindowsData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event((key), 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event((key));
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event((key), 1);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(window, [](GLFWwindow* window, uint32 keycode)
        {
            WindowsData& data = *(WindowsData*)glfwGetWindowUserPointer(window);

            KeyTypedEvent event((keycode));
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowsData& data = *(WindowsData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event((MouseCode)(button));
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event((MouseCode)(button));
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowsData& data = *(WindowsData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowsData& data = *(WindowsData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event(Vec2f((float)xPos, (float)yPos));
            data.EventCallback(event);
        });
    }


    void LinuxWindow::ToggleFullscreen()
    {    
        if (glfwGetWindowMonitor(window))
        {
            glfwSetWindowMonitor(window, NULL, data.windowed_x, data.windowed_y, data.w_width, data.w_height, 0);
            data.windowed = true;
        }
        else
        {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            if (monitor)
            {
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                glfwGetWindowPos(window, &(data.windowed_x), &(data.windowed_y));
                glfwGetWindowSize(window, &(data.w_width), &(data.w_height));
                glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            }
            data.windowed = false;
        }
    }

    void LinuxWindow::OnUpdate()
    {
        fflush(stdout);
        fflush(stderr);

        NEB_PROFILE_FUNCTION();
        if (data.windowed)
        {
            data.w_width = data.width;
            data.w_height = data.height;
        }
        glfwPollEvents();
        context->SwapBuffers();
    }

    uint32 LinuxWindow::GetWidth() const
    {
        return data.width;
    }


    uint32 LinuxWindow::GetHeight() const
    {
        
        return data.height;
    }

    uint32* LinuxWindow::GetWidthPtr() const 
    {
        return &(data.width);
    }
    uint32* LinuxWindow::GetHeightPtr() const
    {
        return &(data.height);
    }

    
    void LinuxWindow::SetWindowSize(uint32 width, uint32 height)
    {
        if (window)
        {
            glfwSetWindowSize(window, width, height);
            data.width = width;
            data.height = height;
        }

    }

    void LinuxWindow::ShutDown()
    {
        NEB_PROFILE_FUNCTION();
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void LinuxWindow::SetVSync(bool enabled)
    {
        if (enabled)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }

        data.VSync = enabled;
    }

    bool LinuxWindow::IsVSync() const
    {
        return data.VSync;
    }

    void LinuxWindow::SetIcon(std::string filepath)
    {
        int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		}
        if(!data) 
        {
            LOG_ERR("Failed to load image for icon!\n");
        }
        else
        {
            LOG_INF("Loaded image for icon\n");
        }

        GLFWimage image;
        image.width = width;
        image.height = height;
        image.pixels = data;

        glfwSetWindowIcon(window, 1, &image);
    }

    bool LinuxWindow::IsMaximized()
    {
        return maximized;
    }

    void LinuxWindow::MaximizeWindow()
    {
        glfwMaximizeWindow(window);
        maximized = true;
    }
    void LinuxWindow::RestoreWindow()
    {
        glfwRestoreWindow(window);
        maximized = false;
    }

    void LinuxWindow::MinimizeWindow()
    {
    }
}
#endif