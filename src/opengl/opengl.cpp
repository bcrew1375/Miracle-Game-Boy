#include <array>

#include "opengl.h"


OpenGlWidget::OpenGlWidget(QWidget* parent) : QOpenGLWidget(parent),
    vertexShaderSource(
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
    fragmentShaderSource(
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


void OpenGlWidget::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glClearColor(0, 0, 0, 0);


    GLfloat vertexCoordinates [] = {
                                    -1.0, -1.0,
                                     1.0, -1.0,
                                    -1.0,  1.0,
                                     1.0,  1.0
                                   };

    vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    shaderProgram = new QOpenGLShaderProgram();
    vertexBufferObject = new QOpenGLBuffer();
    emulatedScreenTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);

    vertexShader->compileSourceCode(vertexShaderSource);
    fragmentShader->compileSourceCode(fragmentShaderSource);

    shaderProgram->addShader(vertexShader);
    shaderProgram->addShader(fragmentShader);
    shaderProgram->link();
    shaderProgram->bind();

    vertexBufferObject->create();
    vertexBufferObject->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vertexBufferObject->bind();
    vertexBufferObject->allocate(vertexCoordinates, 8 * sizeof(float));

    shaderProgram->enableAttributeArray("vertexCoordinates");
    shaderProgram->setAttributeBuffer("vertexCoordinates", GL_FLOAT, 0, 2);

    shaderProgram->setUniformValue("scaleX", (GLfloat)1.0);
    shaderProgram->setUniformValue("scaleY", (GLfloat)1.0);

    shaderProgram->release();
    vertexBufferObject->release();

    emulatedScreenTexture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    emulatedScreenTexture->setSize(emulatedScreenResolutionX, emulatedScreenResolutionY);
    emulatedScreenTexture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    emulatedScreenTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
    emulatedScreenTexture->allocateStorage();

    clearEmulatedScreen();
}


void OpenGlWidget::resizeGL(int width, int height)
{
    // Determine a scalar value for the shader to constrain the aspect ratio.
    double emulatedAspectRatio = (double)emulatedScreenResolutionY / (double)emulatedScreenResolutionX;
    double viewportAspectRatio = (double)height / (double)width;
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


void OpenGlWidget::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    shaderProgram->bind();
    vertexBufferObject->bind();
    emulatedScreenTexture->bind();

    f->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    shaderProgram->release();
    vertexBufferObject->release();
    emulatedScreenTexture->release();
}


void OpenGlWidget::clearEmulatedScreen()
{
    std::unique_ptr<uint32_t> clearBuffer(new uint32_t[emulatedScreenBufferSize]);
    
    std::fill_n(clearBuffer.get(), emulatedScreenBufferSize, 0x00000000);

    updateEmulatedScreen(clearBuffer.get());
}


void OpenGlWidget::updateEmulatedScreen(uint32_t* screenData)
{
    emulatedScreenTexture->destroy();
    emulatedScreenTexture->create();
    emulatedScreenTexture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    emulatedScreenTexture->setSize(emulatedScreenResolutionX, emulatedScreenResolutionY);
    emulatedScreenTexture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    emulatedScreenTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
    emulatedScreenTexture->allocateStorage();

    emulatedScreenTexture->setData(QOpenGLTexture::PixelFormat::RGBA, QOpenGLTexture::PixelType::UInt32_RGBA8, screenData);
    this->update();
}
