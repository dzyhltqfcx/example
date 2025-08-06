#include "mainwindow.h"
#include "pclwidget.h"
#include "interfaces.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // 添加PCL显示部件（占4/5高度）
    PCLWidget* pclWidget = new PCLWidget(this);
    mainLayout->addWidget(pclWidget, 4);

    // 创建按钮容器（水平布局）
    QWidget* buttonContainer = new QWidget(this);
    QHBoxLayout* buttonLayout = new QHBoxLayout(buttonContainer);

    // 添加颜色控制按钮
    QPushButton* colorBtn = new QPushButton("红色，变！", this);
    QPushButton* restoreBtn = new QPushButton("恢复默认", this);

    buttonLayout->addWidget(colorBtn);
    buttonLayout->addWidget(restoreBtn);
    buttonLayout->addStretch(); // 添加伸缩因子使按钮靠左

    // 将按钮容器添加到主布局（占1/5高度）
    mainLayout->addWidget(buttonContainer, 1);

    // 连接按钮信号
    connect(colorBtn, &QPushButton::clicked, this, [] {
        Interfaces::instance()->pclChangeColor();  // 发出改变颜色信号
    });

    connect(restoreBtn, &QPushButton::clicked, this, [] {
        Interfaces::instance()->pclRestoreColor();  // 发出恢复颜色信号
    });

    setCentralWidget(central);
    resize(800, 600);
    setWindowTitle("PCL Sphere Viewer");
}
