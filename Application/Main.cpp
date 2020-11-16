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
		-0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f, 
		 0.5f, -0.5f, 0.0f
	};

	const char* vertexShaderSource = "#version 430 core\n"
		"layout (location = 0) in vec3 vs_position;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(vs_position.x, vs_position.y, vs_position.z, 1.0);\n"
		"}\0";

	const char* fragmentShaderSource = "#version 430 core\n"
		"out vec4 out_color;\n"
		"void main()\n"
		"{\n"
		"   out_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
		"}\0";

	// create vertex buffers
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// create fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
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
