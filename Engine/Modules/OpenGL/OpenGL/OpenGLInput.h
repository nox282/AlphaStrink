#pragma once

#include <OpenGL/OpenGL.h>
#include <Inputs/Interfaces/InputGenerator.h>
#include <memory>

struct GLFWwindow;

namespace ECSEngine
{
	class OpenGLSystem;

	class OpenGLInput: public IInputGenerator
	{
	public:
		void initialize(const std::shared_ptr<OpenGLSystem>& openGLSystem);
		void deinitialize();

		virtual std::string getName() const override;

		virtual bool consumeInputBuffer(std::vector<ButtonControl>& outBuffer) override;
		virtual bool getAxis(std::vector<AxisControl>& outAxis) override;

		virtual void onInputSystemTick(float deltaTime, EntityRegistry& registry) override;
		
		AxisControl wasd = { "WASD" };
		AxisControl mouse = { "Mouse" };

	private:
		std::shared_ptr<OpenGLSystem> m_openGLSystem = nullptr;
		std::vector<ButtonControl> m_inputBuffer;

		static void glfwCallback_onMouseMoved(GLFWwindow* window, double x, double y);
	};
}

