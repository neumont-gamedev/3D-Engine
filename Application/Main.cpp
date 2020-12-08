#include "pch.h"
#include "Engine/Engine.h"

int main(int argc, char** argv)
{
	nc::Engine engine;
	engine.Startup();

	nc::Scene scene{&engine};

	// initialization
	nc::Program program;
	program.CreateShaderFromFile("shaders\\phong.vert", GL_VERTEX_SHADER);
	program.CreateShaderFromFile("shaders\\phong.frag", GL_FRAGMENT_SHADER);
	program.Link();
	program.Use();


	nc::VertexArray vertexArray;
	vertexArray.Create("vertex");

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	nc::Model::Load("models/ogre.obj", positions, normals, texcoords);

	if (!positions.empty())
	{
		vertexArray.CreateBuffer(positions.size() * sizeof(glm::vec3), positions.size(), positions.data());
		vertexArray.SetAttribute(0, 3, 0, 0);
	}
	if (!normals.empty())
	{
		vertexArray.CreateBuffer(normals.size() * sizeof(glm::vec3), normals.size(), normals.data());
		vertexArray.SetAttribute(1, 3, 0, 0);
	}
	if (!texcoords.empty())
	{
		vertexArray.CreateBuffer(texcoords.size() * sizeof(glm::vec2), texcoords.size(), texcoords.data());
		vertexArray.SetAttribute(2, 2, 0, 0);
	}

	// uniform
	glm::mat4 model = glm::mat4(1.0f);

	// camera
	glm::vec3 eye{0, 0, 5};

	nc::Camera camera{ "camera" };
	scene.Add(&camera);
	camera.SetProjection(45.0f, 800.0f / 600.0f, 0.01f, 1000.0f);
	camera.SetLookAt(eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	nc::Texture texture;
	texture.CreateTexture("textures\\ogre_diffuse_flip.bmp");

	nc::Material material{ glm::vec3{ 1 }, glm::vec3{ 1 }, glm::vec3{ 1 }, 32.0f };
	material.AddTexture(texture);
	material.SetProgram(program);

	program.SetUniform("light.ambient", glm::vec3{ 0.1f, 0.1f, 0.1f });
	program.SetUniform("light.diffuse", glm::vec3{ 1, 1, 1 });
	program.SetUniform("light.specular", glm::vec3{ 1, 1, 1 });

	glm::vec4 light{ 5, 2, 5, 1 };

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

		scene.Update(engine.GetTimer().DeltaTime());

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
			camera.transform().translation.x -= 4 * engine.GetTimer().DeltaTime();
		}
		if (engine.GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_D) == nc::InputSystem::eButtonState::HELD)
		{
			camera.transform().translation.x += 4 * engine.GetTimer().DeltaTime();
		}
		if (engine.GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_W) == nc::InputSystem::eButtonState::HELD)
		{
			camera.transform().translation.z -= 4 * engine.GetTimer().DeltaTime();
		}
		if (engine.GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_S) == nc::InputSystem::eButtonState::HELD)
		{
			camera.transform().translation.z += 4 * engine.GetTimer().DeltaTime();
		}


		glm::mat4 mvp = camera.projection() * camera.view() * model;
		program.SetUniform("mvp", mvp);

		glm::mat4 model_view = camera.view() * model;
		program.SetUniform("model_view", model_view);

		glm::vec4 position = camera.view() * light;
		program.SetUniform("light.position", position);
		
		engine.GetSystem<nc::Renderer>()->BeginFrame();

		vertexArray.Draw();
		scene.Draw();

		engine.GetSystem<nc::Renderer>()->EndFrame();
	}

	engine.Shutdown();

	return 0;
}
