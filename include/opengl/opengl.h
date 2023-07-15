#ifndef OPENGL_H
#define OPENGL_H

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtOpenGL/QtOpenGL>
#include <QString>


class OpenGlWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    OpenGlWidget(QWidget *parent);

    void clearEmulatedScreen();
    void updateEmulatedScreen(uint32_t* screenData);

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

private:
    static const uint16_t emulatedScreenResolutionX = 160;
    static const uint16_t emulatedScreenResolutionY = 144;
    static const uint32_t emulatedScreenBufferSize = emulatedScreenResolutionX * emulatedScreenResolutionY;

    const QString vertexShaderSource; 
    const QString fragmentShaderSource;

    QOpenGLShader* vertexShader;
    QOpenGLShader* fragmentShader;
    QOpenGLShaderProgram* shaderProgram;
    QOpenGLBuffer* vertexBufferObject;
    QOpenGLTexture* emulatedScreenTexture;
};

#endif // OPENGL_H
