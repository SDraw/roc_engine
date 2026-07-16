#version 330 core
#define MAX_BONES 128

layout(location = 0) in vec3 gVertexPosition;
layout(location = 3) in vec4 gVertexBoneWeight;
layout(location = 4) in ivec4 gVertexBoneIndex;

uniform mat4 gModelMatrix;
uniform mat4 gViewProjectionMatrix;
uniform bool gAnimated;
uniform mat4 gBoneMatrix[MAX_BONES];

void main()
{
    mat4 l_animatedMatrix = mat4(1.f);
    if(gAnimated)
    {
        l_animatedMatrix = gBoneMatrix[gVertexBoneIndex.x]*gVertexBoneWeight.x;
        l_animatedMatrix += gBoneMatrix[gVertexBoneIndex.y]*gVertexBoneWeight.y;
        l_animatedMatrix += gBoneMatrix[gVertexBoneIndex.z]*gVertexBoneWeight.z;
        l_animatedMatrix += gBoneMatrix[gVertexBoneIndex.w]*gVertexBoneWeight.w;
    }
    l_animatedMatrix = gModelMatrix * l_animatedMatrix;

    gl_Position = gViewProjectionMatrix * l_animatedMatrix * vec4(gVertexPosition, 1.0);
}