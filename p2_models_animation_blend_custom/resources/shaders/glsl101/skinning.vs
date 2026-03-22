#version 100

#define MAX_BONE_NUM 64

attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord;
attribute vec4 vertexColor;
attribute vec4 vertexBoneIndices;
attribute vec4 vertexBoneWeights;

uniform mat4 mvp;
uniform mat4 boneMatrices[MAX_BONE_NUM];

varying vec2 fragTexCoord;
varying vec4 fragColor;

void main()
{
    vec4 pos = vec4(vertexPosition, 1.0);

    int i0 = int(vertexBoneIndices.x);
    int i1 = int(vertexBoneIndices.y);
    int i2 = int(vertexBoneIndices.z);
    int i3 = int(vertexBoneIndices.w);

    float w0 = vertexBoneWeights.x;
    float w1 = vertexBoneWeights.y;
    float w2 = vertexBoneWeights.z;
    float w3 = vertexBoneWeights.w;

    vec4 skinnedPosition =
          w0 * (boneMatrices[i0] * pos)
        + w1 * (boneMatrices[i1] * pos)
        + w2 * (boneMatrices[i2] * pos)
        + w3 * (boneMatrices[i3] * pos);

    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;

    gl_Position = mvp * skinnedPosition;
}
