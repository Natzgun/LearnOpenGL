#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

// uniform vec4 customColor;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
  FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.7);
  // FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
  // FragColor = vec4(ourColor, 1.0);
  // FragColor = customColor;
}
