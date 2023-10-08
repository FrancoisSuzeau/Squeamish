/******************************************************************************************************************************************/
// File : Grid.hpp
// Purpose : An interface for the Grid renderer
/******************************************************************************************************************************************/
#ifndef GRID_H
#define GRID_H

#include "../../../Interfaces/IRenderer.hpp"

namespace Renderers {



	class Grid : public IRenderer
	{
	public:
		Grid(int slices);
		~Grid();

		void Construct() override;
		void Clean();
		GLint GetLength() const;


	private:
		void Attach();
		void Load();

		void CleanEbo();
		GLuint m_ebo;

		typedef IRenderer base;

		std::vector<glm::vec3> m_vec_vertices;
		std::vector<glm::uvec4> m_vec_indices;
		GLint m_lenght;
		int m_slices;



	};
}

#endif