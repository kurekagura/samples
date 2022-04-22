#include <spdlog/spdlog.h>
#include <QOpenGLTexture>
#include "glplaywidget.h"

GLPlayWidget::GLPlayWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{}

void GLPlayWidget::initializeGL() {
    spdlog::info("ENTER:initializeGL");
    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 1.0);

    prog_ = new QOpenGLShaderProgram;
    bool success = prog_->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexSource_);
    if(!success)
        throw "Failed to addShaderFromSourceCode";

    success = prog_->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentSource_);
    if(!success)
        throw "Failed to addShaderFromSourceCode";

    prog_->bindAttributeLocation("position", 0);
    prog_->link();
    prog_->bind();

    // Create VBO(Vertex Buffer Object)
    vboSquare_.create();
    vboSquare_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboSquare_.bind();
    QVector<GLfloat> vertVec = {
        -1.0, 1.0,  0.0, 0.0,
        1.0, 1.0,  1.0, 0.0,
        1.0, -1.0, 1.0, 1.0,
        -1.0, -1.0, 0.0, 1.0 };
    vboSquare_.allocate(vertVec.constData(), vertVec.count() * sizeof(GLfloat));

    // Create VAO(Vertex Array Object)
    vaoSquare_.create();
    vaoSquare_.bind();

    prog_->enableAttributeArray(0);
    prog_->enableAttributeArray(1);
    prog_->setAttributeBuffer(0, GL_FLOAT, 0, 2, 4 * sizeof(GLfloat));
    prog_->setAttributeBuffer(1, GL_FLOAT, 2 * sizeof(GLfloat), 2, 4 * sizeof(GLfloat));

    vaoSquare_.release();
    vboSquare_.release();
    prog_->release();

    QString errorLog = prog_->log();
    // Should not be called because of the possibility of low speed.
    GLenum err;
    while ((err=glGetError()) != GL_NO_ERROR)
    {
        throw "Failed to call OpengGL API.";
    }
}

void GLPlayWidget::setTexture(const QImage& image) {
    texture_ = image;
    update();
}

void GLPlayWidget::paintGL() {
    //spdlog::info("paintGL");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(texture_.isNull()){
        return;
    }
    prog_->bind();
    vaoSquare_.bind();

    QOpenGLTexture texture(texture_);
    //QOpenGLTexture texture(QImage("C:/dev/samplevideo/out-avframe2mat-fhd/000.bmp"));
    if(texture.isCreated())
        texture.bind();

    //QOpenGLTexture* texture = new QOpenGLTexture(texture_);
    //QOpenGLTexture* texture = new QOpenGLTexture(QImage("C:/dev/samplevideo/out-avframe2mat-fhd/000.bmp"));

//    if(texture->isCreated())
//        texture->bind();

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    texture.release();
    //texture->release();
    vaoSquare_.release();
    prog_->release();

    // Should not be called because of the possibility of low speed.
//    QString errorLog = prog_->log();
//    GLenum err;
//    while ((err=glGetError()) != GL_NO_ERROR)
//    {
//        throw "Failed to call OpengGL API.";
//    }
}

