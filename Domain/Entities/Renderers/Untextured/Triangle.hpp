/******************************************************************************************************************************************/
// File : Triangle.hpp
// Purpose : An interface for the triangle renderer
/******************************************************************************************************************************************/
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../../../Interfaces/IRenderer.hpp"

namespace Renderers {

	

	class Triangle : public IRenderer
	{
	public:
		Triangle() ;
		~Triangle();

		void Construct() override;
		void Draw() override;
		void Draw(std::vector<unsigned int> light_texture_ids) override;
		void Clean();

	protected:

		void Load() override;

	private:
		
		void Attach();
		typedef IRenderer base;
		
		
	};
}

#endif