// Copyright © Matt Jones and Contributors. Licensed under the MIT Licence (MIT). See LICENCE.md in the repository root for more information.
#ifndef NOVELRT_SHADERPROGRAM_H
#define NOVELRT_SHADERPROGRAM_H
#include <vector>
#include <glad/glad.h>

namespace NovelRT {
  struct ShaderProgram {
  public:
    GLuint shaderProgramId = 0;
    GLuint finalViewMatrixBufferUboId = 0;
    std::vector<GLuint> uboIds;

    ShaderProgram() {};
  };
}
#endif