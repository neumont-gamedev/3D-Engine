#include "pch.h"
#include "Engine/Engine.h"

int main(int argc, char** argv)
{
	nc::Engine engine;
	engine.Startup();

	nc::Scene scene{&engine};

	// initialization
	nc::Program program;
	program.CreateShaderFromFile("shaders/phong.vert", GL_VERTEX_SHADER);
	program.CreateShaderFromFile("shaders/phong.frag", GL_FRAGMENT_SHADER);
	program.Link();
	program.Use();

	nc::VertexArray vertexArray = nc::Model::Load("models/sphere.obj");

	nc::Texture texture;
	texture.CreateTexture("textures/lava.png");

	nc::Material material{ glm::vec3{ 1 }, glm::vec3{ 1 }, glm::vec3{ 1 }, 32.0f };
	material.AddTexture(texture);
	material.SetProgram(program);

	nc::Model model{ "model", nc::Transform{ {0, 0, 0}, {glm::half_pi<float>(), 0, 0} }, vertexArray, program, material };
	scene.Add(&model);

	// camera
	nc::Camera camera{ "camera" };
	scene.Add(&camera);
	camera.SetProjection(45.0f, 800.0f / 600.0f, 0.01f, 1000.0f);
	glm::vec3 eye{0, 0, 5};
	camera.SetLookAt(eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	nc::Texture texture;
	texture.CreateTexture("textures/ogre_diffuse_flip.bmp");

	nc::Material material{ glm::vec3{ 1 }, glm::vec3{ 1 }, glm::vec3{ 1 }, 32.0f };
	material.AddTexture(texture);
	material.SetProgram(program);

	nc::Light light{ "light", 
		nc::Transform{ glm::vec3{5, 2, 5} }, 
		glm::vec3{ 0.1f }, 
		glm::vec3{ 1 }, 
		glm::vec3{ 1 } };
	scene.Add(&light);

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
		model.transform().rotation.y += angle * engine.GetTimer().DeltaTime();

		// update program lights
		std::vector<nc::Light*> lights = scene.Get<nc::Light>();
		for (nc::Light* light : lights)
		{
			light->SetProgram(program);
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
				
		scene.Draw();

		engine.GetSystem<nc::Renderer>()->EndFrame();
	}

	engine.Shutdown();

	return 0;
}
