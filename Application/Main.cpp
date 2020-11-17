#include "pch.h"
#include <glad\glad.h>
#include "Engine\Graphics\Renderer.h"

int main(int argc, char** argv)
{
	nc::Renderer renderer;
	renderer.Startup();
	renderer.Create("OpenGL", 800, 600);

	// initialization
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // point1
		 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // point2
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // point3
	};

	std::string vertexShaderSource;
	nc::ReadFileToString("shaders\\basic.vert", vertexShaderSource);

	std::string fragmentShaderSource;
	nc::ReadFileToString("shaders\\basic.frag", fragmentShaderSource);

	// create vertex buffers
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// set position pipeline (vertex attribute)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// set color pipeline (vertex attribute)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* str = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &str, nullptr);
	glCompileShader(vertexShader);

	// create fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	str = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &str, nullptr);
	glCompileShader(fragmentShader);

	// create program
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glUseProgram(program);

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}

		SDL_PumpEvents();

		renderer.BeginFrame();

		// render triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		renderer.EndFrame();
	}

	return 0;
}
