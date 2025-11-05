#version 460 core
#extension GL_NV_gpu_shader5: enable

layout(location = 0) in vec3 pos;
layout(location = 1) in uint colorIndex;
layout(location = 2) in uint8_t dir;

out uint geomDir;
out uint geomColor;


void main() {
  gl_Position = vec4(pos, 1.0f);
  geomColor = colorIndex;
  geomDir = uint(dir);
}