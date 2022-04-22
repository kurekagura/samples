#ifndef GLPLAYWIDGET_H
#define GLPLAYWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

class GLPlayWidget : public QOpenGLWidget,
        protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GLPlayWidget(QWidget *parent);
    void setTexture(const QImage& image);
protected:
    void initializeGL() override;
    void paintGL() override;

private:
    QImage texture_;
    QOpenGLShaderProgram *prog_;
    QOpenGLBuffer vboSquare_;
    QOpenGLVertexArrayObject vaoSquare_;

    const char* vertexSource_ =
            "#version 450 core\n"
            "layout(location=0) in vec2 aPos;\n"
            "layout(location=1) in vec2 aTexCoord;\n"
            "out vec4 textureCoord;\n"
            "void main( void )\n"
            "{\n"
            "  gl_Position=vec4(aPos,0.0,1.0);\n"
            "  textureCoord=vec4(aTexCoord,0.0,1.0);\n"
            "}\n";

    const char* fragmentSource_ =
            "#version 450 core\n"
            "uniform sampler2D texture;\n"
            "in vec4 textureCoord;\n"
            "out vec4 fragColor;\n"
            "void main( void )\n"
            "{\n"
            "  fragColor = texture2D(texture, textureCoord.st);\n"
            "}\n";
};

#endif // GLPLAYWIDGET_H
