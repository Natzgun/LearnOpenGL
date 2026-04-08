#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform float time;

void main() {
  gl_Position = vec4(aPos, 1.0);
  // float angle = time;
  // mat3 rotation = mat3(
  //     cos(angle), -sin(angle), 0,
  //     sin(angle), cos(angle), 0,
  //     0, 0, 1
  //   );
  // ourColor = rotation * aColor;
  ourColor = aColor;
  TexCoord = aTexCoord;
}
