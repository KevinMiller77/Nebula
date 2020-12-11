#pragma once

#include <Core/PlatformInfo.h>
#include <Core/FileDialog.h>
#include <Core/Layer.h>
#include <Core/Application.h>
#include <Core/Window.h>
#include <Core/VFS.h>
#include <Core/Ref.h>

#include <Events/Event.h>
#include <Events/InputCodes.h>
#include <Events/KeyEvent.h>
#include <Events/MouseEvent.h>
#include <Events/WindowEvent.h>

#include <Utils/Timer.h>
#include <Utils/Logging.h>
#include <Utils/Timer.h>
#include <Utils/StringOps.h>

#include <Math/Math.h>

#include <Graphics/Buffer.h>
#include <Graphics/Shader.h>
#include <Graphics/Texture.h>
#include <Graphics/TextureLibrary.h>
#include <Graphics/RendererConfig.h>
#include <Graphics/Renderer.h>
#include <Graphics/Renderer2D.h>
#include <Graphics/Camera.h>
#include <Graphics/OrthographicCamera.h>
#include <Graphics/OrthographicCameraController.h>
#include <Graphics/Framebuffer.h>

#include <Scene/Scene.h>
#include <Scene/SceneSerializer.h>
#include <Scene/Scriptable.h>
#include <Scene/Components.h>

#include <ext/imgui/imgui.h>

