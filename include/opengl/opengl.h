#ifndef OPENGL_H
#define OPENGL_H

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtOpenGL/QtOpenGL>
#include <QString>

#define EMULATED_SCREEN_RESOLUTION_X 160
#define EMULATED_SCREEN_RESOLUTION_Y 144


class OpenGlWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    OpenGlWidget(QWidget *parent);

    void clearEmulatedScreen();
    void updateEmulatedScreen(uint32_t *screenData);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    const QString mVertexShaderSource; 
    const QString mFragmentShaderSource;

    QOpenGLShader *mVertexShader;
    QOpenGLShader *mFragmentShader;
    QOpenGLShaderProgram *mShaderProgram;
    QOpenGLBuffer *mVertexBufferObject;
    QOpenGLTexture *mEmulatedScreenTexture;
};

#endif // OPENGL_H
