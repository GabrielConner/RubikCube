#version 460 core

out vec4 fragColor;

in vec4 vertColor;


void main() {
  if (gl_FrontFacing)
    fragColor = vertColor;
  else
    fragColor = vec4(0, 0, 0, 1);
}