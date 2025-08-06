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
#include <QVBoxLayout>  // 添加布局头文件
#include <QPushButton>  // 添加按钮头文件

// VTK相关头文件
#include <QVTKOpenGLNativeWidget.h>

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

private:
    void createMenuBar();
    void createToolBar();
    void createDockWidgets();
    void createCentralWidget();

    // 停靠窗口对象
    QDockWidget *dockElements;
    QDockWidget *dockFiles;
    QDockWidget *dockProperties;
    QDockWidget *dockActions;
    //中央区域显示
    QSplitter *centralSplitter;
    QVTKOpenGLNativeWidget *vtkWidget;  // VTK 3D视图
    QWidget *imageWidget;               // 图像区域（PCL显示） - 保持为 QWidget*

    // 工具栏
    QToolBar *mainToolBar;

    // 菜单项
    QAction *resetLayoutAction;
    QAction *saveLayoutAction;
    QAction *loadLayoutAction;
};

#endif // MAINWINDOW_H
