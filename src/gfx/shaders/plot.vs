#version 330 core

layout(location = 0) in vec3 vs_position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform float offset_x = 1.0;
uniform float scale_x = 1.0;

out vec4 fs_color;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4((vs_position.x + offset_x) * scale_x, vs_position.y, 0, 1);
    fs_color = vec4(vs_position.xy / 2.0 + 0.5, 1, 1);
}
