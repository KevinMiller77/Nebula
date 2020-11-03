#pragma once
#include <string>
#include <Core/Input.h>
#include <Events/Event.h>

namespace Nebula {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer") { this->name = name; }
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return name; }
	protected:
		std::string name;
	};

}