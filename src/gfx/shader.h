#pragma once
#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <format>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "../err/scrooge_exception.h"

namespace gfx
{
	enum SHADER_TYPE
	{
		VERTEX,
		FRAGMENT
	};

	struct shader
	{
		int id;
		SHADER_TYPE type;
		std::string source;

		shader( std::string path, SHADER_TYPE type ) : type { type }
		{
			std::ifstream file;
			file.exceptions( std::ifstream::failbit | std::ifstream::badbit );

			try
			{
				file.open( path );
				std::stringstream str;
				str << file.rdbuf();
				file.close();
				source = str.str();
			}
			catch( std::ifstream::failure e )
			{
				throw err::scrooge_exception( __LINE__, __FILE__, std::format( "Could not read shader file source {}", e.what() ).c_str(), err::exception_type::GFX );
			}
		}
	};

	struct shader_program
	{
		int program_id;

		shader_program() = default;

		shader_program( std::vector<shader> shaders ) : _shaders { shaders }
		{
		}

		void build()
		{
			program_id = glCreateProgram();

			for( auto & s: _shaders )
			{
				auto source = s.source.c_str();
				s.id = glCreateShader( parse_shader( s.type ) );
				glShaderSource( s.id, 1, &source, NULL );
				glCompileShader( s.id );
				int success_compile;
				glGetShaderiv( s.id, GL_COMPILE_STATUS, &success_compile );
				if( ! success_compile )
				{
					char info_log[512];
					glGetShaderInfoLog( s.id, 512, NULL, info_log );
					throw err::scrooge_exception( __LINE__, __FILE__, std::format( "Could not compile shader: \n{}", info_log ).c_str(), err::exception_type::GFX );
				}
				glAttachShader( program_id, s.id );
			}

			glLinkProgram( program_id );
			int success_link;
			glGetProgramiv( program_id, GL_LINK_STATUS, &success_link );
			if( ! success_link )
			{
				char info_log[512];
				glGetProgramInfoLog( program_id, 512, NULL, info_log );
				throw err::scrooge_exception( __LINE__, __FILE__, std::format( "Could not link shader program: \n{}", info_log ).c_str(), err::exception_type::GFX );
			}

			for( auto & s: _shaders )
			{
				glDeleteShader( s.id );
				int success_delete;
				glGetShaderiv( s.id, GL_DELETE_STATUS, &success_delete );
				if( ! success_delete )
				{
					char info_log[512];
					glGetShaderInfoLog( s.id, 512, NULL, info_log );
					throw err::scrooge_exception( __LINE__, __FILE__, std::format( "Could not delete shader: \n{}", info_log ).c_str(), err::exception_type::GFX );
				}
			}
		}

		void use()
		{
			glUseProgram( program_id );
		}

		void set_bool( const std::string & name, bool value ) const
		{
			glUniform1i( get_uniform_location( name ), (int)value );
		}
		void set_int( const std::string & name, int value ) const
		{
			glUniform1i( get_uniform_location( name ), value );
		}
		void set_float( const std::string & name, float value ) const
		{
			glUniform1f( get_uniform_location( name ), value );
		}
		void set_vec3( const std::string & name, const float x, const float y, const float z )
		{
			glUniform3f( get_uniform_location( name ), x, y, z );
		}
		void set_mat3( const std::string & name, const glm::mat3 & mat ) const
		{
			glUniformMatrix3fv( get_uniform_location( name ), 1.0f, GL_FALSE, glm::value_ptr( mat ) );
		}
		void set_mat4( const std::string & name, const glm::mat4 & mat ) const
		{
			glUniformMatrix4fv( get_uniform_location( name ), 1.0f, GL_FALSE, glm::value_ptr( mat ) );
		}
		int get_uniform_location( const std::string name ) const
		{
			auto loc = glGetUniformLocation( program_id, name.c_str() );
			if( loc == -1 )
			{
				throw err::scrooge_exception( __LINE__, __FILE__, std::format( "Could not find location of uniform {}", name ).c_str(), err::exception_type::GFX );
			}
			return loc;
		}

	private:
		const int parse_shader( SHADER_TYPE type )
		{
			switch( type )
			{
			case VERTEX:
				return GL_VERTEX_SHADER;
			case FRAGMENT:
				return GL_FRAGMENT_SHADER;
			default:
				throw err::scrooge_exception( __LINE__, __FILE__, std::format( "Could not parse shader type: {}", (unsigned int)type ).c_str(), err::exception_type::GFX );
			}
		}

		std::vector<gfx::shader> _shaders;
	};
} // namespace gfx
