#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform float xOffset, yOffset;
uniform float time;

void main() {
  gl_Position = vec4(aPos.x + xOffset, aPos.y + yOffset, aPos.z, 1.0);
  float angle = time;
  mat3 rotation = mat3(
    cos(angle), -sin(angle), 0,
    sin(angle),  cos(angle), 0,
    0         ,  0         , 1
  );
  // mat3 rotation = mat3(
  //   cos(angle), 0, sin(angle),
  //   0         , 1,           0, 
  //   -sin(angle), 0, cos(angle)
  // );

  ourColor = rotation * aColor;
}
