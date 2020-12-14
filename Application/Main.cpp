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

	nc::VertexArray cubeVertexArray = nc::Model::Load("models/sphere.obj");
	nc::VertexArray planeVertexArray = nc::Model::Load("models/plane.obj");

	// wood material
	nc::Texture texture;
	texture.CreateTexture("textures/lava.png");

	nc::Material woodMaterial{ glm::vec3{ 1 }, glm::vec3{ 1 }, glm::vec3{ 1 }, 32.0f };
	woodMaterial.AddTexture(texture);
	woodMaterial.SetProgram(program);

	// grass material
	texture.CreateTexture("textures/grass.jpg");

	nc::Material grassMaterial{ glm::vec3{ 1 }, glm::vec3{ 1 }, glm::vec3{ 0.2f }, 32.0f };
	grassMaterial.AddTexture(texture);
	grassMaterial.SetProgram(program);

	// create models
	nc::Model* model;

	model = new nc::Model{ "floor", nc::Transform{ {0, -2, 0}, glm::vec3{0}, glm::vec3{400} }, planeVertexArray, program, grassMaterial };
	scene.Add(model);

	float range = 10;
	for (int i = 0; i < 100; i++)
	{
		float x = nc::random(-range, range);
		float y = nc::random(0, range);
		float z = nc::random(-range, range);

		model = new nc::Model{ "cube", nc::Transform{ {x, y, z} }, cubeVertexArray, program, woodMaterial };
		scene.Add(model);
	}


	// camera
	nc::Camera camera{ "camera" };
	scene.Add(&camera);
	camera.SetProjection(45.0f, 800.0f / 600.0f, 0.01f, 1000.0f);
	glm::vec3 eye{0, 0, 5};
	camera.SetLookAt(eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	nc::Light light{ "light", 
		nc::Transform{ glm::vec3{0, 40, 0} }, 
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
		float dt = engine.GetTimer().DeltaTime();
		scene.Update(dt);

		std::vector<nc::Model*> models = scene.Get<nc::Model>();
		for (auto model : models)
		{
			if (model->name() != "floor")
			{
				model->transform().rotation.y += 2 * dt;
			}
		}

		if (engine.GetSystem<nc::InputSystem>()->GetMouseButtonState(SDL_BUTTON_LEFT) == nc::InputSystem::eButtonState::PRESSED)
		{
			nc::Camera* camera = scene.Get<nc::Camera>("camera");

			glm::vec3 offset = glm::quat(camera->transform().rotation) * glm::vec3{0, 0, -2};

			model = new nc::Model{ "cube", nc::Transform{ camera->transform().translation + offset }, cubeVertexArray, program, woodMaterial };
			scene.Add(model);
		}

		// update program lights
		std::vector<nc::Light*> lights = scene.Get<nc::Light>();
		for (nc::Light* light : lights)
		{
			light->SetProgram(program);
		}
		
		engine.GetSystem<nc::Renderer>()->BeginFrame();
				
		scene.Draw();

		engine.GetSystem<nc::Renderer>()->EndFrame();
	}

	engine.Shutdown();

	return 0;
}
