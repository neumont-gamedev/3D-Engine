#pragma once
#include "../Resources/Resource.h"
#include <SDL.h>
#include <string>

namespace nc
{
	class Texture : public Resource
	{
	public:
		bool Create(const std::string& name, void* renderer) override;
		void Destroy() override;

	protected:
		SDL_Texture* m_texture{ nullptr };
		SDL_Renderer* m_renderer{ nullptr };
	};
}
