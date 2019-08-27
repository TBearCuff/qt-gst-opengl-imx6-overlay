#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <glib.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <QMatrix4x4>
#include "Transform3D.h"


/* Structure to contain all our information, so we can pass it around */
typedef struct _CustomData
{
    GstElement *playbin;


    GstState state;
    gint64 duration;
}CustomData;

class QOpenGLShaderProgram;

class MainWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
//    explicit MainWindow(QWidget *parent = 0);
    MainWindow();
    ~MainWindow();

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void teardownGL();

protected slots:
    void update();

private:
    // OpenGL State Information
    QOpenGLBuffer m_vertex;
    QOpenGLVertexArrayObject m_object;
    QOpenGLShaderProgram *m_program;

    // Shader Information
    int u_modelToWorld;
    int u_worldToView;
    QMatrix4x4 m_projection;
    Transform3D m_transform;

    //Private Helpers
    void printContextInformation();

signals:

public slots:
};

#endif // MAINWINDOW_H
