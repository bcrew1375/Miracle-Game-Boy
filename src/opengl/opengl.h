#ifndef OPENGL_H
#define OPENGL_H

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtOpenGL>

#define VERTEX_SHADER_FILENAME "opengl/shaders/default_vertex.glsl"
#define FRAGMENT_SHADER_FILENAME "opengl/shaders/default_fragment.glsl"
#define EMULATED_SCREEN_RESOLUTION_X 160
#define EMULATED_SCREEN_RESOLUTION_Y 144


class OpenGlWidget : public QOpenGLWidget, protected QOpenGLFunctions {

public:
    OpenGlWidget(QWidget *parent);

    void clearEmulatedScreen();
    void updateEmulatedScreen(uint8_t *screenData);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void updateTexture(uint8_t *textureData);

    QOpenGLShader *vertexShader;
    QOpenGLShader *fragmentShader;
    QOpenGLShaderProgram *shaderProgram;
    QOpenGLBuffer *vertexBufferObject;
    QOpenGLTexture *emulatedScreenTexture;
};

#endif // OPENGL_H
