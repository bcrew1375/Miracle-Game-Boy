#include <qfile.h>
#include <iostream>

#include "opengl.h"


OpenGlWidget::OpenGlWidget(QWidget* parent) : QOpenGLWidget(parent),
    mVertexShaderSource(
        "#version 110\n" \

        "attribute vec2 vertexCoordinates;\n" \
        "varying vec2 textureCoordinates;\n" \
        "uniform float scaleX;\n" \
        "uniform float scaleY;\n" \

        "void main()\n" \
        "{\n" \
            "vec2 vertexVerticalFlip = vec2(vertexCoordinates.x, vertexCoordinates.y * -1.0);\n" \
            "vec2 vertexScaleCoordinates = vec2(vertexVerticalFlip.x * scaleX, vertexVerticalFlip.y * scaleY);\n" \
            "gl_Position = vec4(vertexScaleCoordinates, 0.0, 1.0);\n" \
            "textureCoordinates = vertexCoordinates * vec2(0.5, 0.5) + vec2(0.5, 0.5);\n" \
        "}" \
    ),
    mFragmentShaderSource(
        "#version 110\n" \
        "uniform sampler2D texture;\n" \
        "varying vec2 textureCoordinates;\n" \

        "void main(void)\n" \
        "{\n" \
        "    gl_FragColor = texture2D(texture, textureCoordinates);\n" \
        "}"
    )
 {
}


void OpenGlWidget::initializeGL() {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glClearColor(0, 0, 0, 0);


    GLfloat vertexCoordinates [] = {
                                    -1.0, -1.0,
                                     1.0, -1.0,
                                    -1.0,  1.0,
                                     1.0,  1.0
                                   };

    mVertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    mFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    mShaderProgram = new QOpenGLShaderProgram();
    mVertexBufferObject = new QOpenGLBuffer();
    mEmulatedScreenTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);

    mVertexShader->compileSourceCode(mVertexShaderSource);
    mFragmentShader->compileSourceCode(mFragmentShaderSource);

    mShaderProgram->addShader(mVertexShader);
    mShaderProgram->addShader(mFragmentShader);
    mShaderProgram->link();
    mShaderProgram->bind();

    mVertexBufferObject->create();
    mVertexBufferObject->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    mVertexBufferObject->bind();
    mVertexBufferObject->allocate(vertexCoordinates, 8 * sizeof(float));

    mShaderProgram->enableAttributeArray("vertexCoordinates");
    mShaderProgram->setAttributeBuffer("vertexCoordinates", GL_FLOAT, 0, 2);

    mShaderProgram->setUniformValue("scaleX", (GLfloat)1.0);
    mShaderProgram->setUniformValue("scaleY", (GLfloat)1.0);

    mShaderProgram->release();
    mVertexBufferObject->release();

    mEmulatedScreenTexture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    mEmulatedScreenTexture->setSize(EMULATED_SCREEN_RESOLUTION_X, EMULATED_SCREEN_RESOLUTION_Y);
    mEmulatedScreenTexture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    mEmulatedScreenTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
    mEmulatedScreenTexture->allocateStorage();

    clearEmulatedScreen();
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

    mShaderProgram->bind();
    mShaderProgram->setUniformValue("scaleX", scaleX);
    mShaderProgram->setUniformValue("scaleY", scaleY);
    mShaderProgram->release();
}


void OpenGlWidget::paintGL() {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    mShaderProgram->bind();
    mVertexBufferObject->bind();
    mEmulatedScreenTexture->bind();

    f->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    mShaderProgram->release();
    mVertexBufferObject->release();
    mEmulatedScreenTexture->release();
}


void OpenGlWidget::clearEmulatedScreen() {
    uint32_t clearBuffer[EMULATED_SCREEN_RESOLUTION_X * EMULATED_SCREEN_RESOLUTION_Y];

    memset(clearBuffer, 0xFF, (EMULATED_SCREEN_RESOLUTION_X * EMULATED_SCREEN_RESOLUTION_Y) * sizeof(uint32_t));
    updateEmulatedScreen(clearBuffer);
}


void OpenGlWidget::updateEmulatedScreen(uint32_t *screenData) {
    mEmulatedScreenTexture->destroy();
    mEmulatedScreenTexture->create();
    mEmulatedScreenTexture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    mEmulatedScreenTexture->setSize(EMULATED_SCREEN_RESOLUTION_X, EMULATED_SCREEN_RESOLUTION_Y);
    mEmulatedScreenTexture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    mEmulatedScreenTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
    mEmulatedScreenTexture->allocateStorage();

    mEmulatedScreenTexture->setData(QOpenGLTexture::PixelFormat::RGBA, QOpenGLTexture::PixelType::UInt32_RGBA8, screenData);
    this->update();
}
