#pragma once
#include "Engine/Graphics/Program.h"
#include "Engine/Graphics/VertexArray.h"
#include "Engine/Graphics/Material.h"
#include "Object.h"

/*
namespace nc
{
	class Model : public Object
	{
	public:
		Model(const std::string& name, const Transform& transform, const VertexArray& vertexArray, const Program& program, const Material& material);

		void Draw(GLenum primitiveType = GL_TRIANGLES);

		Program& program() { return m_program; }

	public:
		static bool Load(const std::string& filename, 
						std::vector<glm::vec3>& positions, 
						std::vector<glm::vec3>& normals, 
						std::vector<glm::vec2>& texcoords);

		static VertexArray Create(const std::string& filename);

	protected:
		VertexArray m_vertexArray;
		Program m_program;
		Material m_material;
	};
};
*/