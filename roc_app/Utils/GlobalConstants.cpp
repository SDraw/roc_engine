#include "stdafx.h"

namespace ROC
{

extern const glm::mat4 g_identityMatrix(1.f);
extern const glm::mat4 g_emptyMat4(0.f);
extern const glm::vec2 g_emptyVec2(0.f);
extern const glm::vec3 g_emptyVec3(0.f);
extern const glm::vec4 g_emptyVec4(0.f);
extern const glm::vec3 g_defaultScale(1.f, 1.f, 1.f);
extern const glm::quat g_defaultRotation(1.f, 0.f, 0.f, 0.f);
extern const glm::vec4 g_zeroPoint(0.f, 0.f, 0.f, 1.f);
extern const glm::vec3 g_defaultDirection(0.f, 0.f, -1.f);
extern const glm::vec3 g_defaultDown(0.f, -1.f, 0.f);
extern const float g_epsilon = glm::epsilon<float>();

extern const std::array<float, 12U> g_quadVertexUV =
{
    0.f, 0.f, 0.f, 1.f, 1.f, 1.f,
    0.f, 0.f, 1.f, 1.f, 1.f, 0.f
};
extern const std::array<float, 18U> g_quadVertexNormals =
{
    0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f,
    0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f
};

extern const std::string g_shadowVertexShader = "layout(location = 0) in vec3 gVertexPosition;\
layout(location = 3) in vec4 gVertexBoneWeight;\
layout(location = 4) in ivec4 gVertexBoneIndex;\
\
uniform mat4 gViewProjectionMatrix;\
uniform mat4 gModelMatrix;\
uniform bool gAnimated;\
uniform mat4 gBoneMatrix[MAX_BONES];\
\
void main()\
{\
    mat4 l_rigMatrix = mat4(1.f);\
    if(gAnimated == true)\
    {\
        l_rigMatrix = gBoneMatrix[gVertexBoneIndex.x] * gVertexBoneWeight.x;\
        l_rigMatrix += gBoneMatrix[gVertexBoneIndex.y] * gVertexBoneWeight.y;\
        l_rigMatrix += gBoneMatrix[gVertexBoneIndex.z] * gVertexBoneWeight.z;\
        l_rigMatrix += gBoneMatrix[gVertexBoneIndex.w] * gVertexBoneWeight.w;\
    }\
    gl_Position = gViewProjectionMatrix * gModelMatrix*l_rigMatrix*vec4(gVertexPosition, 1.0);\
}";
extern const std::string g_shadowFragmentShader = "void main(){}";

extern const std::string g_screenVertexShader = "layout(location = 0) in vec3 gVertexPosition;\
layout(location = 2) in vec2 gVertexUV;\
\
out vec2 tUV;\
\
uniform mat4 gModelMatrix;\
uniform mat4 gProjectionMatrix;\
\
void main()\
{\
    tUV = gVertexUV;\
    gl_Position = gProjectionMatrix * gModelMatrix*vec4(gVertexPosition, 1.0);\
}";
extern const std::string g_screenFragmentShader = "in vec2 tUV;\
out vec4 gOutput;\
\
uniform sampler2D gTexture0;\
uniform vec4 gColor;\
\
void main()\
{\
    gOutput = texture(gTexture0, tUV)*gColor;\
}";

}
