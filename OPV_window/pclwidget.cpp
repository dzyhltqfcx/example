#include "pclwidget.h"
#include "interfaces.h"

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <cstdlib>
#include <ctime>
#include <QTimer>
PCLWidget::PCLWidget(QWidget* parent)
    : QVTKOpenGLNativeWidget(parent)
{
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_renderer->SetBackground(0.2, 0.3, 0.4);
    this->renderWindow()->AddRenderer(m_renderer);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    initSpheres();
}

PCLWidget::~PCLWidget() {}

vtkSmartPointer<vtkActor> PCLWidget::createActor(vtkSphereSource* sphere) {
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphere->GetOutputPort());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    double r = static_cast<double>(std::rand()) / RAND_MAX;
    double g = static_cast<double>(std::rand()) / RAND_MAX;
    double b = static_cast<double>(std::rand()) / RAND_MAX;

    actor->GetProperty()->SetColor(r, g, b);
    m_initialColors.append(QVector3D(r, g, b));

    return actor;
}

void PCLWidget::initSpheres() {
    for (int i = 0; i < 5; i++) {
        auto sphere = vtkSmartPointer<vtkSphereSource>::New();
        sphere->SetCenter(i * 2.0, 0, 0);
        sphere->SetRadius(0.5);
        sphere->SetPhiResolution(30);
        sphere->SetThetaResolution(30);

        auto actor = createActor(sphere);
        m_actors.append(actor);
        m_renderer->AddActor(actor);
    }

    connect(Interfaces::instance(), &Interfaces::pclChangeColor, [this]() {
        for (auto& actor : m_actors) {
            actor->GetProperty()->SetColor(1, 0, 0);
        }
        this->renderWindow()->Render();
    });

    connect(Interfaces::instance(), &Interfaces::pclRestoreColor, [this]() {
        for (int i = 0; i < m_actors.size(); ++i) {
            QVector3D color = m_initialColors[i];
            m_actors[i]->GetProperty()->SetColor(color.x(), color.y(), color.z());
        }
        this->renderWindow()->Render();
    });

    this->renderWindow()->Render();
}
