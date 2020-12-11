#pragma once

namespace nc
{
	class Model : public Object
	{
	public:
		Model(const std::string& name, const Transform& transform, const VertexArray& vertexArray, const Program& program, const Material& material) :
			Object{ name, transform },
			m_vertexArray{ vertexArray },
			m_program{ program },
			m_material{ material }
		{}
		
		void Draw() override;

	public:
		static bool Load(const std::string& filename, 
						std::vector<glm::vec3>& positions, 
						std::vector<glm::vec3>& normals, 
						std::vector<glm::vec2>& texcoords);

		static VertexArray Load(const std::string& filename);

	protected:
		Program m_program;
		Material m_material;
		VertexArray m_vertexArray;
	};
};
