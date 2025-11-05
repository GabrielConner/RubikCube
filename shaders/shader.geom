#version 460 core
#extension GL_NV_gpu_shader5: enable
layout(points) in;
layout(triangle_strip, max_vertices=24) out;

layout(std430, binding = 0) buffer colorBuffer {
  vec4 colors[];
};

in uint geomDir[];
in uint geomColor[];

out vec4 vertColor;

uniform mat4 cubeTransform;
uniform mat4 transform;
uniform mat4 projection;

uniform float cubieHalfSize;


void main() {
  vec3 pos = gl_in[0].gl_Position.xyz;
  vec4 sColor = vec4(0, 0, 0, 1);
  if (geomColor[0] != -1)
    sColor = colors[geomColor[0]];

  uint dir = geomDir[0];


  if (dir == 0) {
    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y - cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y - cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y + cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y + cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    EndPrimitive();


  } else if (dir == 1) {
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y - cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y - cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y + cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y + cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    EndPrimitive();


  } else if (dir == 2) {
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y - cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y - cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y - cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y - cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    EndPrimitive();


  } else if (dir == 3) {
    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y - cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y - cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y + cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y + cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    EndPrimitive();


  }else if (dir == 4) {
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y - cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y - cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y + cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y + cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    EndPrimitive();


  }else if (dir == 5) {
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y + cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y + cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y + cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y + cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    EndPrimitive();


  } else if (dir == 6) {
    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y - cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y - cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y + cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y + cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();

    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y + cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y + cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();


    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y - cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y - cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    EndPrimitive();



    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y + cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y + cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y - cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x - cubieHalfSize, pos.y - cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();

    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y - cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y - cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();

    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y + cubieHalfSize, pos.z + cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    gl_Position = projection * cubeTransform * transform * vec4(pos.x + cubieHalfSize, pos.y + cubieHalfSize, pos.z - cubieHalfSize, 1);
    vertColor = sColor;
    EmitVertex();
    EndPrimitive();
  }
}