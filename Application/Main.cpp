#include "pch.h"
#include "Engine/Engine.h"

int main(int argc, char** argv)
{
	nc::Engine engine;
	engine.Startup();

	// initialization
	//float vertices[] =
	//{
	//	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // point1
	//	 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // point2
	//	 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f  // point3
	//};
	static float vertices[] = {
		// front
		-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		// back
		-1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
	};

	// unsigned short - 16bits (0-16000)
	// unsigned int - 32 bits (0, 400000000)

	static GLushort indices[] =
	{
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};


	nc::Program program;
	program.CreateShaderFromFile("shaders\\basic.vert", GL_VERTEX_SHADER);
	program.CreateShaderFromFile("shaders\\basic.frag", GL_FRAGMENT_SHADER);
	program.Link();
	program.Use();

	// create vertex buffers
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// set position pipeline (vertex attribute)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// set uv pipeline (vertex attribute)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// create index buffers
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// uniform
	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800 / 600.0f, 0.01f, 1000.0f);
	
	glm::vec3 eye{0, 0, 5};
	glm::mat4 view = glm::lookAt(eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	nc::Texture texture;
	texture.CreateTexture("textures\\llama.jpg");

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
		engine.Update();

		float angle = 0;
		if (engine.GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_E) == nc::InputSystem::eButtonState::HELD)
		{
			angle = 2.0f;
		}
		if (engine.GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_Q) == nc::InputSystem::eButtonState::HELD)
		{
			angle = -2.0f;
		}
		model = glm::rotate(model, angle * engine.GetTimer().DeltaTime(), glm::vec3(0, 1, 0));

		if (engine.GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_A) == nc::InputSystem::eButtonState::HELD)
		{
			eye.x -= 4 * engine.GetTimer().DeltaTime();
		}
		if (engine.GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_D) == nc::InputSystem::eButtonState::HELD)
		{
			eye.x += 4 * engine.GetTimer().DeltaTime();
		}
		if (engine.GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_W) == nc::InputSystem::eButtonState::HELD)
		{
			eye.z -= 4 * engine.GetTimer().DeltaTime();
		}
		if (engine.GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_S) == nc::InputSystem::eButtonState::HELD)
		{
			eye.z += 4 * engine.GetTimer().DeltaTime();
		}

		view = glm::lookAt(eye, eye + glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });

		glm::mat4 mvp = projection * view * model;
		program.SetUniform("transform", mvp);
		
		engine.GetSystem<nc::Renderer>()->BeginFrame();

		// render triangle
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		GLsizei numElements = sizeof(indices) / sizeof(GLushort);
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);

		engine.GetSystem<nc::Renderer>()->EndFrame();
	}

	engine.Shutdown();

	return 0;
}
