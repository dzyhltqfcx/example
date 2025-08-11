#include "mainwindow.h"
#include "interfaces.h"
#include <opencv2/opencv.hpp>  // 添加OpenCV头文件
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("3D点云处理工具");
    resize(1200, 900);

    createMenuBar();
    createToolBar();
    createDockWidgets();
    createCentralWidget();
    loadLayout();
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
    openImagesAction = new QAction(tr("打开图片"), this);  // 创建打开图片动作

    resetLayoutAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_R));
    saveLayoutAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    loadLayoutAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    openImagesAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_I));  // 设置快捷键

    fileMenu->addAction(openImagesAction);  // 添加到文件菜单
    viewMenu->addAction(saveLayoutAction);
    viewMenu->addAction(loadLayoutAction);
    viewMenu->addSeparator();
    viewMenu->addAction(resetLayoutAction);

    connect(resetLayoutAction, &QAction::triggered, this, &MainWindow::resetLayout);
    connect(saveLayoutAction, &QAction::triggered, this, &MainWindow::saveLayout);
    connect(loadLayoutAction, &QAction::triggered, this, &MainWindow::loadLayout);
    connect(openImagesAction, &QAction::triggered, this, &MainWindow::openImageFiles);  // 连接信号
}

void MainWindow::createToolBar()
{
    mainToolBar = new QToolBar("主工具栏", this);
    mainToolBar->setObjectName("mainToolBar");
    addToolBar(Qt::TopToolBarArea, mainToolBar);

    // 添加打开图片按钮到工具栏
    QAction *openAction = mainToolBar->addAction(QIcon(":/icons/open.png"), "打开图片");
    connect(openAction, &QAction::triggered, this, &MainWindow::openImageFiles);

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
    filesListWidget = new QListWidget();  // 使用成员变量
    dockFiles->setWidget(filesListWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dockFiles);

    // 连接双击信号
    connect(filesListWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::displayImage);

    // 属性显示区
    dockProperties = new QDockWidget(tr("属性"), this);
    dockProperties->setObjectName("propertiesDock");
    QTextEdit *propertiesContent = new QTextEdit();
    propertiesContent->setText("点云属性:");
    dockProperties->setWidget(propertiesContent);
    addDockWidget(Qt::RightDockWidgetArea, dockProperties);

    // 操作按钮区域 - 放在底部
    dockActions = new QDockWidget(tr("操作面板"), this);
    dockActions->setObjectName("actionsDock");
    QWidget *actionsPanel = new QWidget();
    QHBoxLayout *actionsLayout = new QHBoxLayout(actionsPanel);

    QPushButton *btnProcess = new QPushButton("按钮1");
    QPushButton *btnSegment = new QPushButton("按钮2");
    QPushButton *btnFilter = new QPushButton("按钮3");
    QPushButton *btnExport = new QPushButton("按钮4");
    QPushButton *colorBtn = new QPushButton("红色，变！", this);
    QPushButton *restoreBtn = new QPushButton("恢复默认", this);

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
    resizeDocks({dockActions}, {100}, Qt::Vertical);
}

void MainWindow::createCentralWidget()
{
    centralSplitter = new QSplitter(Qt::Horizontal, this);
    pclWidget = new PCLWidget();
    pclWidget->setMinimumSize(400, 400);

    // 创建图像显示区域（使用滚动区域）
    imageScrollArea = new QScrollArea();
    imageLabel = new QLabel();
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setText("图像显示区域");
    imageLabel->setStyleSheet("background-color: #333; color: #EEE;");

    imageScrollArea->setBackgroundRole(QPalette::Dark);
    imageScrollArea->setWidget(imageLabel);
    imageScrollArea->setWidgetResizable(true);

    centralSplitter->addWidget(pclWidget);
    centralSplitter->addWidget(imageScrollArea);
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

// 打开图片文件
void MainWindow::openImageFiles()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(
        this,
        tr("打开图片"),
        QDir::homePath(),
        tr("图片文件 (*.png *.jpg *.jpeg *.bmp *.tif)")
        );

    if (fileNames.isEmpty()) return;

    foreach (const QString &fileName, fileNames) {
        // 只添加不在列表中的文件
        QList<QListWidgetItem*> items = filesListWidget->findItems(fileName, Qt::MatchExactly);
        if (items.isEmpty()) {
            QFileInfo fileInfo(fileName);
            QListWidgetItem *item = new QListWidgetItem(fileInfo.fileName());
            item->setData(Qt::UserRole, fileName); // 存储完整路径
            filesListWidget->addItem(item);
        }
    }
}

// 显示图片
void MainWindow::displayImage(QListWidgetItem *item)
{
    QString filePath = item->data(Qt::UserRole).toString();
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "错误", "无法获取文件路径");
        return;
    }

    // 使用OpenCV加载图像
    cv::Mat image = cv::imread(filePath.toStdString());
    if (image.empty()) {
        QMessageBox::critical(this, "错误", "无法加载图像: " + filePath);
        return;
    }

    // 转换颜色空间 BGR -> RGB
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);

    // 创建QImage
    QImage qimg(image.data,
                image.cols,
                image.rows,
                image.step,
                QImage::Format_RGB888);

    // 显示图像
    imageLabel->setPixmap(QPixmap::fromImage(qimg));
    imageLabel->adjustSize();

    // 更新窗口标题显示图像信息
    setWindowTitle(QString("图像显示: %1 | 尺寸: %2x%3")
                       .arg(item->text())
                       .arg(image.cols)
                       .arg(image.rows));
}
