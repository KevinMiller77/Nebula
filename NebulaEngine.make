# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

RESCOMP = windres
INCLUDES += -isystem Nebula -isystem Nebula/src -isystem Nebula/src/Nebula -isystem Nebula/include -isystem Nebula/ext/imgui -isystem Nebula/ext/glfw/include -isystem Nebula/ext/glad/include -isystem Nebula/ext/yaml-cpp/include -isystem Nebula/ext/nativefiledialog/src/include
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LINKCMD = $(AR) -rcs "$@" $(OBJECTS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug)
TARGETDIR = bin/Debug/linux/NebulaEngine
TARGET = $(TARGETDIR)/libNebulaEngine.a
OBJDIR = bin-int/Debug/linux/NebulaEngine
DEFINES += -D_CRT_SECURE_NO_WARNINGS -DGLFW_SUPPLIED -D_LIBS_SUPPLIED -DNEB_DEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -g -std=c++17
LIBS += Nebula/ext/imgui/bin/Debug/linux/imgui/libimgui.a Nebula/ext/glad/bin/Debug/linux/glad/libglad.a Nebula/ext/glfw/bin/Debug/linux/glfw/libglfw.a Nebula/ext/yaml-cpp/bin/Debug/linux/yaml-cpp/libyaml-cpp.a Nebula/ext/nativefiledialog/bin/Debug/linux/nfd/libnfd_d.a -lX11 -lGL -lGLU -ldl
LDDEPS += Nebula/ext/imgui/bin/Debug/linux/imgui/libimgui.a Nebula/ext/glad/bin/Debug/linux/glad/libglad.a Nebula/ext/glfw/bin/Debug/linux/glfw/libglfw.a Nebula/ext/yaml-cpp/bin/Debug/linux/yaml-cpp/libyaml-cpp.a Nebula/ext/nativefiledialog/bin/Debug/linux/nfd/libnfd_d.a
ALL_LDFLAGS += $(LDFLAGS)

else ifeq ($(config),release)
TARGETDIR = bin/Release/linux/NebulaEngine
TARGET = $(TARGETDIR)/libNebulaEngine.a
OBJDIR = bin-int/Release/linux/NebulaEngine
DEFINES += -D_CRT_SECURE_NO_WARNINGS -DGLFW_SUPPLIED -D_LIBS_SUPPLIED -DNEB_RELEASE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -O2 -std=c++17
LIBS += Nebula/ext/imgui/bin/Release/linux/imgui/libimgui.a Nebula/ext/glad/bin/Release/linux/glad/libglad.a Nebula/ext/glfw/bin/Release/linux/glfw/libglfw.a Nebula/ext/yaml-cpp/bin/Release/linux/yaml-cpp/libyaml-cpp.a Nebula/ext/nativefiledialog/bin/Release/linux/nfd/libnfd.a -lX11 -lGL -lGLU -ldl
LDDEPS += Nebula/ext/imgui/bin/Release/linux/imgui/libimgui.a Nebula/ext/glad/bin/Release/linux/glad/libglad.a Nebula/ext/glfw/bin/Release/linux/glfw/libglfw.a Nebula/ext/yaml-cpp/bin/Release/linux/yaml-cpp/libyaml-cpp.a Nebula/ext/nativefiledialog/bin/Release/linux/nfd/libnfd.a
ALL_LDFLAGS += $(LDFLAGS) -s

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/Application.o
GENERATED += $(OBJDIR)/Buffer.o
GENERATED += $(OBJDIR)/Entity.o
GENERATED += $(OBJDIR)/FileDialog.o
GENERATED += $(OBJDIR)/FileIO.o
GENERATED += $(OBJDIR)/Framebuffer.o
GENERATED += $(OBJDIR)/GLBuffers.o
GENERATED += $(OBJDIR)/GLContext.o
GENERATED += $(OBJDIR)/GLFramebuffer.o
GENERATED += $(OBJDIR)/GLImGuiBuild.o
GENERATED += $(OBJDIR)/GLImGuiLayer.o
GENERATED += $(OBJDIR)/GLPipeline.o
GENERATED += $(OBJDIR)/GLRendererAPI.o
GENERATED += $(OBJDIR)/GLShader.o
GENERATED += $(OBJDIR)/GLTexture2D.o
GENERATED += $(OBJDIR)/GLVertexArray.o
GENERATED += $(OBJDIR)/GUID.o
GENERATED += $(OBJDIR)/GraphicsContext.o
GENERATED += $(OBJDIR)/ImGuiLayer.o
GENERATED += $(OBJDIR)/Input.o
GENERATED += $(OBJDIR)/LayerStack.o
GENERATED += $(OBJDIR)/LinuxWindow.o
GENERATED += $(OBJDIR)/MacWindow.o
GENERATED += $(OBJDIR)/MemoryTracker.o
GENERATED += $(OBJDIR)/Nebula_pch.o
GENERATED += $(OBJDIR)/OrthographicCamera.o
GENERATED += $(OBJDIR)/OrthographicCameraController.o
GENERATED += $(OBJDIR)/Pipeline.o
GENERATED += $(OBJDIR)/Renderer.o
GENERATED += $(OBJDIR)/Renderer2D.o
GENERATED += $(OBJDIR)/RendererAPI.o
GENERATED += $(OBJDIR)/RendererConfig.o
GENERATED += $(OBJDIR)/Scene.o
GENERATED += $(OBJDIR)/SceneCamera.o
GENERATED += $(OBJDIR)/SceneSerializer.o
GENERATED += $(OBJDIR)/Shader.o
GENERATED += $(OBJDIR)/StringOps.o
GENERATED += $(OBJDIR)/Texture.o
GENERATED += $(OBJDIR)/TextureLibrary.o
GENERATED += $(OBJDIR)/VFS.o
GENERATED += $(OBJDIR)/VertexArray.o
GENERATED += $(OBJDIR)/Window.o
GENERATED += $(OBJDIR)/WindowsWindow.o
GENERATED += $(OBJDIR)/stb_image.o
OBJECTS += $(OBJDIR)/Application.o
OBJECTS += $(OBJDIR)/Buffer.o
OBJECTS += $(OBJDIR)/Entity.o
OBJECTS += $(OBJDIR)/FileDialog.o
OBJECTS += $(OBJDIR)/FileIO.o
OBJECTS += $(OBJDIR)/Framebuffer.o
OBJECTS += $(OBJDIR)/GLBuffers.o
OBJECTS += $(OBJDIR)/GLContext.o
OBJECTS += $(OBJDIR)/GLFramebuffer.o
OBJECTS += $(OBJDIR)/GLImGuiBuild.o
OBJECTS += $(OBJDIR)/GLImGuiLayer.o
OBJECTS += $(OBJDIR)/GLPipeline.o
OBJECTS += $(OBJDIR)/GLRendererAPI.o
OBJECTS += $(OBJDIR)/GLShader.o
OBJECTS += $(OBJDIR)/GLTexture2D.o
OBJECTS += $(OBJDIR)/GLVertexArray.o
OBJECTS += $(OBJDIR)/GUID.o
OBJECTS += $(OBJDIR)/GraphicsContext.o
OBJECTS += $(OBJDIR)/ImGuiLayer.o
OBJECTS += $(OBJDIR)/Input.o
OBJECTS += $(OBJDIR)/LayerStack.o
OBJECTS += $(OBJDIR)/LinuxWindow.o
OBJECTS += $(OBJDIR)/MacWindow.o
OBJECTS += $(OBJDIR)/MemoryTracker.o
OBJECTS += $(OBJDIR)/Nebula_pch.o
OBJECTS += $(OBJDIR)/OrthographicCamera.o
OBJECTS += $(OBJDIR)/OrthographicCameraController.o
OBJECTS += $(OBJDIR)/Pipeline.o
OBJECTS += $(OBJDIR)/Renderer.o
OBJECTS += $(OBJDIR)/Renderer2D.o
OBJECTS += $(OBJDIR)/RendererAPI.o
OBJECTS += $(OBJDIR)/RendererConfig.o
OBJECTS += $(OBJDIR)/Scene.o
OBJECTS += $(OBJDIR)/SceneCamera.o
OBJECTS += $(OBJDIR)/SceneSerializer.o
OBJECTS += $(OBJDIR)/Shader.o
OBJECTS += $(OBJDIR)/StringOps.o
OBJECTS += $(OBJDIR)/Texture.o
OBJECTS += $(OBJDIR)/TextureLibrary.o
OBJECTS += $(OBJDIR)/VFS.o
OBJECTS += $(OBJDIR)/VertexArray.o
OBJECTS += $(OBJDIR)/Window.o
OBJECTS += $(OBJDIR)/WindowsWindow.o
OBJECTS += $(OBJDIR)/stb_image.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking NebulaEngine
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning NebulaEngine
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) rmdir /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/stb_image.o: Nebula/ext/stb_image/stb_image.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Application.o: Nebula/src/Nebula/Core/Application.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/FileDialog.o: Nebula/src/Nebula/Core/FileDialog.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Input.o: Nebula/src/Nebula/Core/Input.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/LayerStack.o: Nebula/src/Nebula/Core/LayerStack.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VFS.o: Nebula/src/Nebula/Core/VFS.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Window.o: Nebula/src/Nebula/Core/Window.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Buffer.o: Nebula/src/Nebula/Graphics/Buffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Framebuffer.o: Nebula/src/Nebula/Graphics/Framebuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GraphicsContext.o: Nebula/src/Nebula/Graphics/GraphicsContext.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OrthographicCamera.o: Nebula/src/Nebula/Graphics/OrthographicCamera.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OrthographicCameraController.o: Nebula/src/Nebula/Graphics/OrthographicCameraController.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Pipeline.o: Nebula/src/Nebula/Graphics/Pipeline.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Renderer.o: Nebula/src/Nebula/Graphics/Renderer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Renderer2D.o: Nebula/src/Nebula/Graphics/Renderer2D.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/RendererAPI.o: Nebula/src/Nebula/Graphics/RendererAPI.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/RendererConfig.o: Nebula/src/Nebula/Graphics/RendererConfig.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Shader.o: Nebula/src/Nebula/Graphics/Shader.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Texture.o: Nebula/src/Nebula/Graphics/Texture.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/TextureLibrary.o: Nebula/src/Nebula/Graphics/TextureLibrary.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VertexArray.o: Nebula/src/Nebula/Graphics/VertexArray.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ImGuiLayer.o: Nebula/src/Nebula/ImGui/ImGuiLayer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Nebula_pch.o: Nebula/src/Nebula/Nebula_pch.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Entity.o: Nebula/src/Nebula/Scene/Entity.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Scene.o: Nebula/src/Nebula/Scene/Scene.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/SceneCamera.o: Nebula/src/Nebula/Scene/SceneCamera.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/SceneSerializer.o: Nebula/src/Nebula/Scene/SceneSerializer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/FileIO.o: Nebula/src/Nebula/Utils/FileIO.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GUID.o: Nebula/src/Nebula/Utils/GUID.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/MemoryTracker.o: Nebula/src/Nebula/Utils/MemoryTracker.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/StringOps.o: Nebula/src/Nebula/Utils/StringOps.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GLBuffers.o: Nebula/src/Platform/Graphics/GL/GLBuffers.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GLContext.o: Nebula/src/Platform/Graphics/GL/GLContext.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GLFramebuffer.o: Nebula/src/Platform/Graphics/GL/GLFramebuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GLPipeline.o: Nebula/src/Platform/Graphics/GL/GLPipeline.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GLRendererAPI.o: Nebula/src/Platform/Graphics/GL/GLRendererAPI.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GLShader.o: Nebula/src/Platform/Graphics/GL/GLShader.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GLTexture2D.o: Nebula/src/Platform/Graphics/GL/GLTexture2D.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GLVertexArray.o: Nebula/src/Platform/Graphics/GL/GLVertexArray.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GLImGuiBuild.o: Nebula/src/Platform/Graphics/GL/ImGui/GLImGuiBuild.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GLImGuiLayer.o: Nebula/src/Platform/Graphics/GL/ImGui/GLImGuiLayer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/LinuxWindow.o: Nebula/src/Platform/OS/Linux/LinuxWindow.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/MacWindow.o: Nebula/src/Platform/OS/MacOS/MacWindow.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/WindowsWindow.o: Nebula/src/Platform/OS/Windows/WindowsWindow.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif