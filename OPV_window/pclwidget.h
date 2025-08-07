#ifndef PCLWIDGET_H
#define PCLWIDGET_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <QVector>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <QVector3D>

class PCLWidget : public QVTKOpenGLNativeWidget {
    Q_OBJECT
public:
    explicit PCLWidget(QWidget* parent = nullptr);
    ~PCLWidget();

    void initSpheres();

private:
    vtkSmartPointer<vtkActor> createActor(vtkSphereSource* sphere);

    vtkSmartPointer<vtkRenderer> m_renderer;
    QVector<vtkSmartPointer<vtkActor>> m_actors;
    QVector<QVector3D> m_initialColors;
};

#endif // PCLWIDGET_H
