#include "pclwidget.h"
#include "interfaces.h"

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <cstdlib>   // 用于rand()和srand()
#include <ctime>     // 用于time()

PCLWidget::PCLWidget(QWidget* parent)
    : QVTKOpenGLNativeWidget(parent)
{
    // 创建渲染器并设置背景色
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_renderer->SetBackground(0.2, 0.3, 0.4);
    this->renderWindow()->AddRenderer(m_renderer);

    // 初始化随机种子
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // 初始化球体
    initSpheres();
}

PCLWidget::~PCLWidget() {}

// 创建球体Actor并设置随机颜色
vtkSmartPointer<vtkActor> PCLWidget::createActor(vtkSphereSource* sphere) {
    // 创建映射器
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphere->GetOutputPort());

    // 创建Actor
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    // 生成随机颜色
    double r = static_cast<double>(std::rand()) / RAND_MAX;
    double g = static_cast<double>(std::rand()) / RAND_MAX;
    double b = static_cast<double>(std::rand()) / RAND_MAX;

    // 设置颜色并保存初始值
    actor->GetProperty()->SetColor(r, g, b);
    m_initialColors.append(QVector3D(r, g, b));  // 保存初始颜色

    return actor;
}

void PCLWidget::initSpheres() {
    // 创建5个球体
    for (int i = 0; i < 5; i++) {
        auto sphere = vtkSmartPointer<vtkSphereSource>::New();
        sphere->SetCenter(i * 2.0, 0, 0);  // X轴间隔2个单位
        sphere->SetRadius(0.5);
        sphere->SetPhiResolution(30);
        sphere->SetThetaResolution(30);

        auto actor = createActor(sphere);
        m_actors.append(actor);
        m_renderer->AddActor(actor);
    }

    // 连接颜色改变信号（设置为红色）
    connect(Interfaces::instance(), &Interfaces::pclChangeColor, [this]() {
        for (auto& actor : m_actors) {
            actor->GetProperty()->SetColor(1, 0, 0);  // 设置为红色
        }
        this->renderWindow()->Render();  // 立即渲染更新
    });

    // 连接颜色恢复信号（恢复初始随机颜色）
    connect(Interfaces::instance(), &Interfaces::pclRestoreColor, [this]() {
        for (int i = 0; i < m_actors.size(); ++i) {
            QVector3D color = m_initialColors[i];
            m_actors[i]->GetProperty()->SetColor(
                color.x(), color.y(), color.z()
                );
        }
        this->renderWindow()->Render();
    });

    // 初始渲染
    this->renderWindow()->Render();
}
