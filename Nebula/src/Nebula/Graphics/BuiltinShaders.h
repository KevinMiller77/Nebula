#pragma once
#include <string>

namespace Nebula
{
	namespace Builtin
	{
		const std::string TexQuad = 
		std::string("\r\n\
		#type vertex\r\n\
		#version 330 core\r\n\
		\r\n\
		layout(location = 0) in vec3 a_Position;\r\n\
		layout(location = 1) in vec4 a_Color;\r\n\
		layout(location = 2) in vec2 a_TexCoord;\r\n\
		layout(location = 3) in float a_TexIndex;\r\n\
		layout(location = 4) in float a_TilingFactor;\r\n\
		\r\n\
		uniform mat4 u_ViewProjection;\r\n\
		\r\n\
		out vec4 v_Color;\r\n\
		out vec2 v_TexCoord;\r\n\
		out float v_TexIndex;\r\n\
		out float v_TilingFactor;\r\n\
		\r\n\
		void main()\r\n\
		{\r\n\
			v_Color = a_Color;\r\n\
			v_TexCoord = a_TexCoord;\r\n\
			v_TexIndex = a_TexIndex;\r\n\
			v_TilingFactor = a_TilingFactor;\r\n\
			gl_Position = u_ViewProjection * vec4(a_Position, 1.0);\r\n\
		}\r\n\
		\r\n\
		#type fragment\r\n\
		#version 330 core\r\n\
		\r\n\
		layout(location = 0) out vec4 color;\r\n\
		\r\n\
		in vec4 v_Color;\r\n\
		in vec2 v_TexCoord;\r\n\
		in float v_TexIndex;\r\n\
		in float v_TilingFactor;\r\n\
		\r\n\
		uniform sampler2D u_Textures[32];\r\n\
		\r\n\
		void main()\r\n\
		{\r\n\
			vec4 texColor = v_Color;\r\n\
			switch(int(v_TexIndex))\r\n\
			{\r\n\
				case 0: texColor *= texture(u_Textures[0], v_TexCoord * v_TilingFactor); break;\r\n\
				case 1: texColor *= texture(u_Textures[1], v_TexCoord * v_TilingFactor); break;\r\n\
				case 2: texColor *= texture(u_Textures[2], v_TexCoord * v_TilingFactor); break;\r\n\
				case 3: texColor *= texture(u_Textures[3], v_TexCoord * v_TilingFactor); break;\r\n\
				case 4: texColor *= texture(u_Textures[4], v_TexCoord * v_TilingFactor); break;\r\n\
				case 5: texColor *= texture(u_Textures[5], v_TexCoord * v_TilingFactor); break;\r\n\
				case 6: texColor *= texture(u_Textures[6], v_TexCoord * v_TilingFactor); break;\r\n\
				case 7: texColor *= texture(u_Textures[7], v_TexCoord * v_TilingFactor); break;\r\n\
				case 8: texColor *= texture(u_Textures[8], v_TexCoord * v_TilingFactor); break;\r\n\
				case 9: texColor *= texture(u_Textures[9], v_TexCoord * v_TilingFactor); break;\r\n\
				case 10: texColor *= texture(u_Textures[10], v_TexCoord * v_TilingFactor); break;\r\n\
				case 11: texColor *= texture(u_Textures[11], v_TexCoord * v_TilingFactor); break;\r\n\
				case 12: texColor *= texture(u_Textures[12], v_TexCoord * v_TilingFactor); break;\r\n\
				case 13: texColor *= texture(u_Textures[13], v_TexCoord * v_TilingFactor); break;\r\n\
				case 14: texColor *= texture(u_Textures[14], v_TexCoord * v_TilingFactor); break;\r\n\
				case 15: texColor *= texture(u_Textures[15], v_TexCoord * v_TilingFactor); break;\r\n\
				case 16: texColor *= texture(u_Textures[16], v_TexCoord * v_TilingFactor); break;\r\n\
				case 17: texColor *= texture(u_Textures[17], v_TexCoord * v_TilingFactor); break;\r\n\
				case 18: texColor *= texture(u_Textures[18], v_TexCoord * v_TilingFactor); break;\r\n\
				case 19: texColor *= texture(u_Textures[19], v_TexCoord * v_TilingFactor); break;\r\n\
				case 20: texColor *= texture(u_Textures[20], v_TexCoord * v_TilingFactor); break;\r\n\
				case 21: texColor *= texture(u_Textures[21], v_TexCoord * v_TilingFactor); break;\r\n\
				case 22: texColor *= texture(u_Textures[22], v_TexCoord * v_TilingFactor); break;\r\n\
				case 23: texColor *= texture(u_Textures[23], v_TexCoord * v_TilingFactor); break;\r\n\
				case 24: texColor *= texture(u_Textures[24], v_TexCoord * v_TilingFactor); break;\r\n\
				case 25: texColor *= texture(u_Textures[25], v_TexCoord * v_TilingFactor); break;\r\n\
				case 26: texColor *= texture(u_Textures[26], v_TexCoord * v_TilingFactor); break;\r\n\
				case 27: texColor *= texture(u_Textures[27], v_TexCoord * v_TilingFactor); break;\r\n\
				case 28: texColor *= texture(u_Textures[28], v_TexCoord * v_TilingFactor); break;\r\n\
				case 29: texColor *= texture(u_Textures[29], v_TexCoord * v_TilingFactor); break;\r\n\
				case 30: texColor *= texture(u_Textures[30], v_TexCoord * v_TilingFactor); break;\r\n\
				case 31: texColor *= texture(u_Textures[31], v_TexCoord * v_TilingFactor); break;\r\n\
			}\r\n\
			color = texColor;\r\n\
		}\r\n\
		");

		const std::string Line = 
		std::string("\
			#type vertex\r\n\
			#version 430 core\r\n\
			\r\n\
			layout(location = 0) in vec3 a_Position;\r\n\
			layout(location = 1) in vec4 a_Color;\r\n\
			\r\n\
			uniform mat4 u_ViewProjection;\r\n\
			\r\n\
			out vec4 v_Color;\r\n\
			\r\n\
			void main()\r\n\
			{\r\n\
				v_Color = a_Color;\r\n\
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);\r\n\
			}\r\n\
			\r\n\
			#type fragment\r\n\
			#version 430 core\r\n\
			\r\n\
			layout(location = 0) out vec4 color;\r\n\
			\r\n\
			in vec4 v_Color;\r\n\
			\r\n\
			void main()\r\n\
			{\r\n\
				color = v_Color;\r\n\
			}\r\n\
		");

	}
}