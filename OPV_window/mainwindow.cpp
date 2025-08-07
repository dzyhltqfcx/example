#include "mainwindow.h"
#include "interfaces.h"  // 包含Interfaces头文件
#include <QLabel>  // 添加这行

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("3D点云处理工具");
    resize(1200, 900);

    createMenuBar();
    createToolBar();
    createDockWidgets(); // 先创建Dock部件
    createCentralWidget(); // 再创建中央部件
    //loadLayout();
}

MainWindow::~MainWindow() {}

void MainWindow::createMenuBar()
{
    QMenuBar *menuBar = this->menuBar();
    QMenu *fileMenu = menuBar->addMenu(tr("文件(&F)"));
    QMenu *viewMenu = menuBar->addMenu(tr("视图(&V)"));

    resetLayoutAction = new QAction(tr("恢复默认布局"), this);
    saveLayoutAction = new QAction(tr("保存布局"), this);
    loadLayoutAction = new QAction(tr("加载布局"), this);

    resetLayoutAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_R));
    saveLayoutAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    loadLayoutAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));

    viewMenu->addAction(saveLayoutAction);
    viewMenu->addAction(loadLayoutAction);
    viewMenu->addSeparator();
    viewMenu->addAction(resetLayoutAction);

    connect(resetLayoutAction, &QAction::triggered, this, &MainWindow::resetLayout);
    connect(saveLayoutAction, &QAction::triggered, this, &MainWindow::saveLayout);
    connect(loadLayoutAction, &QAction::triggered, this, &MainWindow::loadLayout);
}

void MainWindow::createToolBar()
{
    mainToolBar = new QToolBar("主工具栏", this);
    mainToolBar->setObjectName("mainToolBar");
    addToolBar(Qt::TopToolBarArea, mainToolBar);

    mainToolBar->addAction(QIcon(":/icons/open.png"), "打开");
    mainToolBar->addAction(QIcon(":/icons/save.png"), "保存");
    mainToolBar->addSeparator();
    mainToolBar->addAction(QIcon(":/icons/run.png"), "处理");
    mainToolBar->addAction(QIcon(":/icons/reset.png"), "重置");
}

void MainWindow::createDockWidgets()
{
    // 元素列表窗口
    dockElements = new QDockWidget(tr("元素列表"), this);
    dockElements->setObjectName("elementsDock");
    QListWidget *elementsList = new QListWidget();
    elementsList->addItems({"点云1", "点云2", "模型1", "标注1"});
    dockElements->setWidget(elementsList);
    addDockWidget(Qt::LeftDockWidgetArea, dockElements);

    // 文件列表窗口
    dockFiles = new QDockWidget(tr("文件列表"), this);
    dockFiles->setObjectName("filesDock");
    QListWidget *filesList = new QListWidget();
    filesList->addItems({"/data/pointcloud1.pcd", "/data/pointcloud2.pcd", "/data/model1.obj"});
    dockFiles->setWidget(filesList);
    addDockWidget(Qt::LeftDockWidgetArea, dockFiles);

    // 属性显示区（保留在右侧）
    dockProperties = new QDockWidget(tr("属性"), this);
    dockProperties->setObjectName("propertiesDock");
    QTextEdit *propertiesContent = new QTextEdit();
    propertiesContent->setText("点云属性:\n- 点数: 102400\n- 分辨率: 0.01m\n- 范围: 10x10x5m");
    dockProperties->setWidget(propertiesContent);
    addDockWidget(Qt::RightDockWidgetArea, dockProperties);

    // 操作按钮区域 - 现在放在底部
    dockActions = new QDockWidget(tr("操作面板"), this);
    dockActions->setObjectName("actionsDock");
    QWidget *actionsPanel = new QWidget();

    // 改为水平布局更适合底部面板
    QHBoxLayout *actionsLayout = new QHBoxLayout(actionsPanel);

    // 原有的操作按钮
    QPushButton *btnProcess = new QPushButton("处理点云");
    QPushButton *btnSegment = new QPushButton("分割");
    QPushButton *btnFilter = new QPushButton("滤波");
    QPushButton *btnExport = new QPushButton("导出结果");

    // 新增的颜色控制按钮
    QPushButton *colorBtn = new QPushButton("红色，变！", this);
    QPushButton *restoreBtn = new QPushButton("恢复默认", this);

    // 添加所有按钮（水平排列）
    actionsLayout->addWidget(btnProcess);
    actionsLayout->addWidget(btnSegment);
    actionsLayout->addWidget(btnFilter);
    actionsLayout->addWidget(btnExport);
    actionsLayout->addStretch();
    actionsLayout->addWidget(new QLabel("颜色控制:"));
    actionsLayout->addWidget(colorBtn);
    actionsLayout->addWidget(restoreBtn);

    actionsPanel->setLayout(actionsLayout);
    dockActions->setWidget(actionsPanel);
    addDockWidget(Qt::BottomDockWidgetArea, dockActions);

    // 连接颜色按钮信号
    connect(colorBtn, &QPushButton::clicked, [] {
        Interfaces::instance()->pclChangeColor();
    });
    connect(restoreBtn, &QPushButton::clicked, [] {
        Interfaces::instance()->pclRestoreColor();
    });

    // 初始布局设置
    tabifyDockWidget(dockElements, dockFiles);
    dockElements->raise();

    // 调整停靠窗口大小
    resizeDocks({dockElements}, {200}, Qt::Horizontal);
    resizeDocks({dockActions}, {100}, Qt::Vertical); // 底部面板高度设为100
}

void MainWindow::createCentralWidget()
{
    centralSplitter = new QSplitter(Qt::Horizontal, this);

    // 使用PCLWidget替换原来的VTK部件
    pclWidget = new PCLWidget();
    pclWidget->setMinimumSize(400, 400);

    // 右侧图像区域
    imageWidget = new QTextEdit();
    imageWidget->setText("图像显示区域\n(实际使用PCL可视化组件)");
    imageWidget->setStyleSheet("background-color: #333; color: #EEE;");
    imageWidget->setReadOnly(true);

    centralSplitter->addWidget(pclWidget);
    centralSplitter->addWidget(imageWidget);
    centralSplitter->setStretchFactor(0, 2);
    centralSplitter->setStretchFactor(1, 1);

    setCentralWidget(centralSplitter);
}

void MainWindow::resetLayout()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("恢复默认布局"),
                                  tr("确定要恢复默认布局吗？当前布局将丢失。"),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        removeDockWidget(dockElements);
        removeDockWidget(dockFiles);
        removeDockWidget(dockActions);
        removeDockWidget(dockProperties);

        createDockWidgets();
        centralSplitter->setSizes({width() * 2 / 3, width() * 1 / 3});
    }
}

void MainWindow::saveLayout()
{
    QString strPath = QCoreApplication::applicationDirPath() + "/UILayout.dat";
    QFile file(strPath);

    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out << this->saveState();
        out << centralSplitter->saveState();
        file.close();
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

        this->restoreState(state);
        centralSplitter->restoreState(splitterState);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveLayout();
    event->accept();
}
