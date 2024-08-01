#pragma once
#include <glad/glad.h>
#include "glm/ext/matrix_transform.hpp"

enum camera_movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// default
constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f; // aka FOV

struct camera
{
	// attributes
	glm::vec3 camera_position;
	glm::vec3 camera_front;
	glm::vec3 camera_up;
	glm::vec3 camera_right;
	glm::vec3 world_up;
	// Euler angles
	float yaw;
	float pitch;
	// camera options
	float movement_speed;
	float sensitivity;
	float zoom_level;
	float max_fov;
	float x_last;
	float y_last;

	camera( glm::vec3 camera_position = glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3 world_up = glm::vec3( 0.0f, 1.0f, 0.0f ), float yaw = YAW, float pitch = PITCH, float max_fov = 45.0f )
		: camera_front( glm::vec3( 0.0f, 0.0f, -1.0f ) ), movement_speed( SPEED ), sensitivity( SENSITIVITY ), zoom_level( ZOOM ), camera_position { camera_position }, world_up { world_up }, yaw { yaw }, pitch { pitch }, max_fov { max_fov }, x_last { 400 }, y_last { 300 }
	{
		update_camera_vectors();
	}
	camera( float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch, float max_fov = 45.0f )
		: camera_front( glm::vec3( 0.0f, 0.0f, -1.0f ) ), movement_speed( SPEED ), sensitivity( SENSITIVITY ), zoom_level( ZOOM ), camera_position { glm::vec3( pos_x, pos_y, pos_z ) }, world_up { glm::vec3( up_x, up_y, up_z ) }, yaw { yaw }, pitch { pitch }, max_fov { max_fov }
	{
		update_camera_vectors();
	}
	glm::mat4 get_view_matrix()
	{
		return glm::lookAt( camera_position, camera_position + camera_front, camera_up );
	}

	void process_keyboard( camera_movement direction, float delta_time )
	{
		float velocity = movement_speed * delta_time;
		switch( direction )
		{
		case FORWARD:
			camera_position += camera_front * velocity;
			break;
		case BACKWARD:
			camera_position -= camera_front * velocity;
			break;
		case LEFT:
			camera_position -= camera_right * velocity;
			break;
		case RIGHT:
			camera_position += camera_right * velocity;
			break;
		case UP:
			camera_position += camera_up * velocity;
			break;
		case DOWN:
			camera_position -= camera_up * velocity;
			break;
		}
	}

	void process_mouse( float x_pos, float y_pos, const bool constraint_pitch = true )
	{
		float x_offset = x_pos - x_last;
		float y_offset = y_last - y_pos;

		x_last = x_pos;
		y_last = y_pos;

		x_offset *= sensitivity;
		y_offset *= sensitivity;

		yaw += x_offset;
		pitch += y_offset;

		if( constraint_pitch )
		{
			pitch = glm::clamp( pitch, -89.0f, 89.0f );
		}

		update_camera_vectors();
	}

	void process_scroll( float y_offset )
	{
		zoom_level -= y_offset;
		zoom_level = glm::clamp( zoom_level, 1.0f, max_fov );
	}

private:
	void update_camera_vectors()
	{
		// calculate direction vector front facing camera
		glm::vec3 front_calc;
		front_calc.x = std::cos( glm::radians( yaw ) ) * std::cos( glm::radians( pitch ) );
		front_calc.y = std::sin( glm::radians( pitch ) );
		front_calc.z = std::sin( glm::radians( yaw ) ) * std::cos( glm::radians( pitch ) );
		camera_front = glm::normalize( front_calc );

		try
		{
			// calculate right and up relative to front facing direction
			camera_right = glm::normalize( glm::cross( camera_front, world_up ) );
			camera_up = glm::normalize( glm::cross( camera_right, camera_front ) );
		}
		catch( std::exception e )
		{
			throw;
		}
	}
};
