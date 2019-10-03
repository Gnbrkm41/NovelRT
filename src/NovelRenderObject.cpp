//
// Created by matth on 23/02/2019.
//

#include <iostream>
#include "NovelRenderObject.h"
#include "GeoBounds.h"
#include <functional>

namespace NovelRT {
NovelRenderObject::NovelRenderObject(NovelLayeringService* layeringService,
                                     const GeoVector<float>& size,
                                     const NovelCommonArgs& args,
                                     const GLuint programId) : NovelObject(layeringService, size, args),
                                                               _modelTransform(Lazy<glm::mat3>(std::function<glm::mat3()>(
                                                                   std::bind(&NovelRenderObject::generateModelTransform,
                                                                             this)))),
 /*                                                              _modelTransformUniformBuffer(Lazy<GLuint>(std::function<GLuint()>([this]{
                                                                 auto bla = _modelTransform.getActual();
                                                                 GLuint ubo;
                                                                 glGenBuffers(1, &ubo);
                                                                 glBindBuffer(GL_UNIFORM_BUFFER, ubo);
                                                                 glBufferData(GL_UNIFORM_BUFFER, sizeof(bla), &bla, GL_DYNAMIC_DRAW);
                                                                 glBindBuffer(GL_UNIFORM_BUFFER, 0);
                                                                 return ubo;
                                                               }))),*/
                                                               _vertexBuffer(Lazy<GLuint>(std::function<GLuint()>(
                                                                   generateStandardBuffer))),
                                                               _vertexArrayObject(Lazy<GLuint>(std::function<GLuint()>([] {
                                                                 GLuint tempVao;
                                                                 glGenVertexArrays(1, &tempVao);
                                                                 return tempVao;
                                                               }))),
                                                               _programId(programId) {
}

void NovelRenderObject::executeObjectBehaviour() {
  if (!_bufferInitialised) {
    configureObjectBuffers();
    _bufferInitialised = true;
  }
  drawObject();
}

void NovelRenderObject::configureObjectBuffers() {
  auto topLeft = GeoVector<GLfloat>(-1.0f, 1.0f);
  auto bottomRight = GeoVector<GLfloat>(1.0f, -1.0f);
  auto topRight = GeoVector<GLfloat>(1.0f, 1.0f);
  auto bottomLeft = GeoVector<GLfloat>(-1.0f, -1.0f);

  _vertexBufferData = {
      topLeft.getX(), topLeft.getY(), 0.0f,
      bottomRight.getX(), bottomRight.getY(), 0.0f,
      topRight.getX(), topRight.getY(), 0.0f,
      topLeft.getX(), topLeft.getY(), 0.0f,
      bottomLeft.getX(), bottomLeft.getY(), 0.0f,
      bottomRight.getX(), bottomRight.getY(), 0.0f,
  };

  _vertexArrayObject.getActual(); //this is just here to force initialisation.

// The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer.getActual());

// Give our vertices to OpenGL.
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _vertexBufferData.size(), _vertexBufferData.data(), GL_STATIC_DRAW);
}
void NovelRenderObject::setSize(const GeoVector<float>& value) {
  _modelTransform.reset();
  NovelObject::setSize(value);
  configureObjectBuffers();
}
void NovelRenderObject::setRotation(const float value) {
  _modelTransform.reset();
  NovelObject::setRotation(value);
  configureObjectBuffers();
}
void NovelRenderObject::setScale(const GeoVector<float>& value) {
  _modelTransform.reset();
  NovelObject::setScale(value);
  configureObjectBuffers();
}
void NovelRenderObject::setPosition(const GeoVector<float>& value) {
  _modelTransform.reset();
  NovelObject::setPosition(value);
  configureObjectBuffers();
}
NovelRenderObject::~NovelRenderObject() {
  if (!_vertexArrayObject.isCreated())
    return;

  auto vao = _vertexArrayObject.getActual();
  glDeleteVertexArrays(1, &vao);
}

GLuint NovelRenderObject::generateStandardBuffer() {
  GLuint tempBuffer;
  glGenBuffers(1, &tempBuffer);
  return tempBuffer;
}
glm::mat3 NovelRenderObject::generateModelTransform() {
  auto size = (getSize() * getScale()).getValue();
  auto position = getPosition().getValue();
  auto matrix3D = glm::mat4();
  glm::rotate(matrix3D, glm::radians(getRotation()), glm::vec3(0.0f, 0.0f, 1.0f));
  glm::scale(matrix3D, glm::vec3(size, 1.0f));
  glm::translate(matrix3D, glm::vec3(position, 0.0f));
  return glm::mat3(matrix3D);
}
}
