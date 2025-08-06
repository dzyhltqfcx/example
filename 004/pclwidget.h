#ifndef PCLWIDGET_H
#define PCLWIDGET_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <QVector>
#include <vtkRenderer.h>
#include <vtkSphereSource.h> // 包含头文件解决前向声明问题
#include <QVector3D>        // 包含QVector3D头文件

class PCLWidget : public QVTKOpenGLNativeWidget {
    Q_OBJECT
public:
    explicit PCLWidget(QWidget* parent = nullptr);
    ~PCLWidget();

    // 初始化球体点云
    void initSpheres();

private:
    // 创建球体Actor
    vtkSmartPointer<vtkActor> createActor(vtkSphereSource* sphere);

    vtkSmartPointer<vtkRenderer> m_renderer;       // VTK渲染器
    QVector<vtkSmartPointer<vtkActor>> m_actors;   // 球体Actor集合
    QVector<QVector3D> m_initialColors;            // 存储初始颜色值
};

#endif // PCLWIDGET_H
