#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QToolBar>
#include <QAction>
#include <QCloseEvent>
#include <QFile>
#include <QDataStream>
#include <QCoreApplication>
#include <QTextEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QSplitter>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>  // 添加QLabel
#include <QScrollArea>  // 添加滚动区域
#include <QFileDialog>  // 添加文件对话框
#include "pclwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void resetLayout();
    void saveLayout();
    void loadLayout();
    void openImageFiles();  // 新添加的打开图片槽函数
    void displayImage(QListWidgetItem *item);  // 新添加的显示图片槽函数

private:
    void createMenuBar();
    void createToolBar();
    void createDockWidgets();
    void createCentralWidget();

    // 停靠窗口
    QDockWidget *dockElements;
    QDockWidget *dockFiles;
    QDockWidget *dockProperties;
    QDockWidget *dockActions;

    // 中央区域
    QSplitter *centralSplitter;
    PCLWidget *pclWidget;
    QScrollArea *imageScrollArea;  // 改为滚动区域
    QLabel *imageLabel;            // 用于显示图像的标签

    // 文件列表
    QListWidget *filesListWidget;  // 文件列表控件

    // 工具栏
    QToolBar *mainToolBar;

    // 菜单项
    QAction *resetLayoutAction;
    QAction *saveLayoutAction;
    QAction *loadLayoutAction;
    QAction *openImagesAction;     // 打开图片动作
};

#endif // MAINWINDOW_H
