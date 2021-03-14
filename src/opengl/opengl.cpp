#include "opengl.h"

#include <qfile.h>
#include <iostream>

OpenGlWidget::OpenGlWidget(QWidget *parent) : QOpenGLWidget(parent) {}


void OpenGlWidget::initializeGL() {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glClearColor(1, 1, 1, 0);

    vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    shaderProgram = new QOpenGLShaderProgram();
    vertexBufferObject = new QOpenGLBuffer();

    GLfloat vertexCoordinates [] = {
                                    -1.0, -1.0,
                                     1.0, -1.0,
                                    -1.0,  1.0,
                                     1.0,  1.0
                                   };

    vertexShader->compileSourceFile(VERTEX_SHADER_FILENAME);
    fragmentShader->compileSourceFile(FRAGMENT_SHADER_FILENAME);

    shaderProgram->addShader(vertexShader);
    shaderProgram->addShader(fragmentShader);
    shaderProgram->link();
    shaderProgram->bind();

    vertexBufferObject->create();
    vertexBufferObject->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertexBufferObject->bind();
    vertexBufferObject->allocate(vertexCoordinates, 8 * sizeof(float));

    shaderProgram->enableAttributeArray("vertexCoordinates");
    shaderProgram->setAttributeBuffer("vertexCoordinates", GL_FLOAT, 0, 2);

    shaderProgram->setUniformValue("scaleX", (GLfloat)1.0);
    shaderProgram->setUniformValue("scaleY", (GLfloat)1.0);

    shaderProgram->release();
    vertexBufferObject->release();

    emulatedScreenTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    emulatedScreenTexture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    emulatedScreenTexture->setSize(EMULATED_SCREEN_RESOLUTION_X, EMULATED_SCREEN_RESOLUTION_Y);
    emulatedScreenTexture->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    emulatedScreenTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
    emulatedScreenTexture->allocateStorage();
}


void OpenGlWidget::resizeGL(int w, int h) {
    // Determine a scalar value for the shader to constrain the aspect ratio.
    double emulatedAspectRatio = (double)EMULATED_SCREEN_RESOLUTION_Y / (double)EMULATED_SCREEN_RESOLUTION_X;
    double viewportAspectRatio = (double)h / (double)w;
    GLfloat scaleX;
    GLfloat scaleY;

    if (viewportAspectRatio < emulatedAspectRatio) {
        scaleX = viewportAspectRatio / emulatedAspectRatio;
        scaleY = 1.0;
    }
    else {
        scaleX = 1.0;
        scaleY = viewportAspectRatio / emulatedAspectRatio;
    }

    shaderProgram->bind();
    shaderProgram->setUniformValue("scaleX", scaleX);
    shaderProgram->setUniformValue("scaleY", scaleY);
    shaderProgram->release();
}


void OpenGlWidget::paintGL() {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    shaderProgram->bind();
    vertexBufferObject->bind();
    emulatedScreenTexture->bind();

    f->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    shaderProgram->release();
    vertexBufferObject->release();
    //emulatedScreenTexture->release();
    //emulatedScreenTexture->destroy();
}


void OpenGlWidget::updateTexture(uint8_t *textureData) {
    emulatedScreenTexture->destroy();
    emulatedScreenTexture->create();
    emulatedScreenTexture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    emulatedScreenTexture->setSize(EMULATED_SCREEN_RESOLUTION_X, EMULATED_SCREEN_RESOLUTION_Y);
    emulatedScreenTexture->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    emulatedScreenTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
    emulatedScreenTexture->allocateStorage();

    //emulatedScreenTexture->setData(QImage("texture.png"));
    emulatedScreenTexture->setData(QOpenGLTexture::PixelFormat::RGBA, QOpenGLTexture::PixelType::UInt32_RGBA8_Rev, textureData);
}


void OpenGlWidget::clearEmulatedScreen() {
    uint8_t clearBuffer[(EMULATED_SCREEN_RESOLUTION_X * 4) * (EMULATED_SCREEN_RESOLUTION_Y * 4)];
    memset(&clearBuffer, 0, sizeof(clearBuffer));
    updateTexture(clearBuffer);
}


void OpenGlWidget::updateEmulatedScreen(uint8_t *screenData) {
    uint8_t textureData[(EMULATED_SCREEN_RESOLUTION_X * 4) * (EMULATED_SCREEN_RESOLUTION_Y * 4)];

    for (int i = 0; i < 11520; i++) {
        textureData[(i * 8)] = 255;
        textureData[(i * 8) + 1] = 0;
        textureData[(i * 8) + 2] = 0;
        textureData[(i * 8) + 3] = 0;
        textureData[(i * 8) + 4] = 0;
        textureData[(i * 8) + 5] = 0;
        textureData[(i * 8) + 6] = 0;
        textureData[(i * 8) + 7] = 0;
    }
    //emulatedScreenTexture->setData(QOpenGLTexture::PixelFormat::RGBA, QOpenGLTexture::PixelType::UInt32_RGBA8_Rev, &textureData);
    this->updateTexture(textureData);
}
