#include "mainwindow.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QSplitter>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("3D点云处理工具");
    resize(1200, 800);

    // 创建菜单栏
    createMenuBar();

    // 创建工具栏
    createToolBar();

    // 创建中央区域
    createCentralWidget();

    // 创建停靠窗口
    createDockWidgets();

    // 加载布局
    loadLayout();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createMenuBar()
{
    // 创建菜单栏
    QMenuBar *menuBar = this->menuBar();

    // 文件菜单
    QMenu *fileMenu = menuBar->addMenu(tr("文件(&F)"));

    // 视图菜单
    QMenu *viewMenu = menuBar->addMenu(tr("视图(&V)"));

    // 创建动作
    resetLayoutAction = new QAction(tr("恢复默认布局"), this);
    saveLayoutAction = new QAction(tr("保存布局"), this);
    loadLayoutAction = new QAction(tr("加载布局"), this);

    // 添加快捷键
    resetLayoutAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_R));
    saveLayoutAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    loadLayoutAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));

    // 添加到菜单
    viewMenu->addAction(saveLayoutAction);
    viewMenu->addAction(loadLayoutAction);
    viewMenu->addSeparator();
    viewMenu->addAction(resetLayoutAction);

    // 连接信号槽
    connect(resetLayoutAction, &QAction::triggered, this, &MainWindow::resetLayout);
    connect(saveLayoutAction, &QAction::triggered, this, &MainWindow::saveLayout);
    connect(loadLayoutAction, &QAction::triggered, this, &MainWindow::loadLayout);
}

void MainWindow::createToolBar()
{
    // 创建主工具栏
    mainToolBar = new QToolBar("主工具栏", this);
    mainToolBar->setObjectName("mainToolBar");
    addToolBar(Qt::TopToolBarArea, mainToolBar);

    // 添加工具按钮
    mainToolBar->addAction(QIcon(":/icons/open.png"), "打开");
    mainToolBar->addAction(QIcon(":/icons/save.png"), "保存");
    mainToolBar->addSeparator();
    mainToolBar->addAction(QIcon(":/icons/run.png"), "处理");
    mainToolBar->addAction(QIcon(":/icons/reset.png"), "重置");
}

void MainWindow::createDockWidgets()
{
    // 1. 元素列表窗口 (左侧)
    dockElements = new QDockWidget(tr("元素列表"), this);
    dockElements->setObjectName("elementsDock");
    QListWidget *elementsList = new QListWidget();
    elementsList->addItems({"点云1", "点云2", "模型1", "标注1"});
    dockElements->setWidget(elementsList);
    addDockWidget(Qt::LeftDockWidgetArea, dockElements);

    // 2. 文件列表窗口 (左侧，在元素列表下方)
    dockFiles = new QDockWidget(tr("文件列表"), this);
    dockFiles->setObjectName("filesDock");
    QListWidget *filesList = new QListWidget();
    filesList->addItems({"/data/pointcloud1.pcd", "/data/pointcloud2.pcd", "/data/model1.obj"});
    dockFiles->setWidget(filesList);
    addDockWidget(Qt::LeftDockWidgetArea, dockFiles);

    // 3. 操作按钮区域 (右侧)
    dockActions = new QDockWidget(tr("操作"), this);
    dockActions->setObjectName("actionsDock");
    QWidget *actionsPanel = new QWidget();
    QVBoxLayout *actionsLayout = new QVBoxLayout(actionsPanel); // 设置布局到面板

    // 添加操作按钮
    QPushButton *btnProcess = new QPushButton("处理点云");
    QPushButton *btnSegment = new QPushButton("分割");
    QPushButton *btnFilter = new QPushButton("滤波");
    QPushButton *btnExport = new QPushButton("导出结果");

    actionsLayout->addWidget(btnProcess);
    actionsLayout->addWidget(btnSegment);
    actionsLayout->addWidget(btnFilter);
    actionsLayout->addWidget(btnExport);
    actionsLayout->addStretch();

    actionsPanel->setLayout(actionsLayout);
    dockActions->setWidget(actionsPanel);
    addDockWidget(Qt::RightDockWidgetArea, dockActions);

    // 4. 属性显示区 (右侧，在操作区域下方)
    dockProperties = new QDockWidget(tr("属性"), this);
    dockProperties->setObjectName("propertiesDock");
    QTextEdit *propertiesContent = new QTextEdit();
    propertiesContent->setText("点云属性:\n- 点数: 102400\n- 分辨率: 0.01m\n- 范围: 10x10x5m");
    dockProperties->setWidget(propertiesContent);
    addDockWidget(Qt::RightDockWidgetArea, dockProperties);

    // 初始布局设置
    tabifyDockWidget(dockElements, dockFiles);  // 元素列表和文件列表标签化
    dockElements->raise();

    tabifyDockWidget(dockActions, dockProperties);  // 操作区和属性区标签化
    dockActions->raise();

    // 设置初始大小
    resizeDocks({dockElements}, {200}, Qt::Horizontal);
    resizeDocks({dockActions}, {150}, Qt::Horizontal);
}

void MainWindow::createCentralWidget()
{
    // 创建分割器，将中央区域分为左右两部分
    centralSplitter = new QSplitter(Qt::Horizontal, this);

    // 左侧：VTK三维展示区
    vtkWidget = new QVTKOpenGLNativeWidget();
    vtkWidget->setMinimumSize(400, 400);

    // 这里添加VTK渲染代码
    // vtkNew<vtkRenderer> renderer;
    // vtkWidget->renderWindow()->AddRenderer(renderer);

    // 右侧：图像区域（PCL显示）
    // 错误修复：使用 QTextEdit 而不是 QWidget
    QTextEdit *imageWidget = new QTextEdit(); // 实际项目中替换为PCL显示组件
    imageWidget->setText("图像显示区域\n(实际使用PCL可视化组件)");
    imageWidget->setStyleSheet("background-color: #333; color: #EEE;");
    imageWidget->setReadOnly(true); // 设置为只读

    // 添加到分割器
    centralSplitter->addWidget(vtkWidget);
    centralSplitter->addWidget(imageWidget);

    // 设置分割比例
    centralSplitter->setStretchFactor(0, 2); // VTK区域占2/3
    centralSplitter->setStretchFactor(1, 1); // 图像区域占1/3

    // 设置中央部件
    setCentralWidget(centralSplitter);
}

void MainWindow::resetLayout()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("恢复默认布局"),
                                  tr("确定要恢复默认布局吗？当前布局将丢失。"),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // 移除所有停靠窗口
        removeDockWidget(dockElements);
        removeDockWidget(dockFiles);
        removeDockWidget(dockActions);
        removeDockWidget(dockProperties);

        // 重新创建停靠窗口
        createDockWidgets();

        // 恢复分割比例
        centralSplitter->setSizes({width() * 2 / 3, width() * 1 / 3});

        qDebug() << "已恢复默认布局";
    }
}

void MainWindow::saveLayout()
{
    QString strPath = QCoreApplication::applicationDirPath() + "/UILayout.dat";
    QFile file(strPath);

    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out << this->saveState();  // 保存窗口状态
        out << centralSplitter->saveState(); // 保存分割器状态
        file.close();
        qDebug() << "布局已保存到:" << strPath;
    } else {
        qWarning() << "无法保存布局文件:" << file.errorString();
    }
}

void MainWindow::loadLayout()
{
    QString strPath = QCoreApplication::applicationDirPath() + "/UILayout.dat";
    QFile file(strPath);

    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        QByteArray state;
        QByteArray splitterState;

        in >> state;
        in >> splitterState;
        file.close();

        if (!this->restoreState(state)) {
            qDebug() << "窗口布局恢复失败，使用默认布局";
        } else {
            qDebug() << "窗口布局恢复成功";
        }

        if (!centralSplitter->restoreState(splitterState)) {
            qDebug() << "中央区域分割状态恢复失败";
        } else {
            qDebug() << "中央区域分割状态恢复成功";
        }
    } else {
        qDebug() << "布局文件不存在，使用默认布局";
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveLayout();
    event->accept();
}
