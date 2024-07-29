#pragma once
#include <vector>
#include "shader.h"

namespace gfx
{
	enum DRAW_TYPE
	{
		LINE_STRIP,
		GL_TRIANGLE
	};

	struct mesh
	{
		mesh() = default;
		mesh( std::vector<glm::vec3> vertices ) : _vertices { vertices }
		{
		}

		void draw( GLuint draw_type, gfx::shader_program & shader_program, glm::mat4 model, glm::mat4 view, glm::mat4 projection )
		{
			glBindVertexArray( _VAO );

			shader_program.use();

			shader_program.set_mat4( "u_model", model );
			shader_program.set_mat4( "u_view", view );
			shader_program.set_mat4( "u_projection", projection );

			glDrawArrays( draw_type, 0, static_cast<unsigned int>( _vertices.size() ) );

			glBindVertexArray( 0 );
		}

		void init()
		{
			glGenVertexArrays( 1, &_VAO );
			glGenBuffers( 1, &_VBO );

			glBindVertexArray( _VAO );

			glBindBuffer( GL_ARRAY_BUFFER, _VBO );
			glBufferData( GL_ARRAY_BUFFER, _vertices.size() * sizeof( glm::vec3 ), _vertices.data(), GL_STATIC_DRAW );

			glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), (void *)( 0 ) );
			glEnableVertexAttribArray( 0 );

			glBindVertexArray( 0 );
		}

	private:
		std::vector<glm::vec3> _vertices;
		unsigned int _VAO, _VBO;
	};
} // namespace gfx
