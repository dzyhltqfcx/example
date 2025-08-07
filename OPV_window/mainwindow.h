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
#include "pclwidget.h"  // 包含PCLWidget

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

    // 停靠窗口
    QDockWidget *dockElements;
    QDockWidget *dockFiles;
    QDockWidget *dockProperties;
    QDockWidget *dockActions;

    // 中央区域
    QSplitter *centralSplitter;
    PCLWidget *pclWidget;  // 使用PCLWidget替换原来的VTK部件
    QTextEdit *imageWidget;

    // 工具栏
    QToolBar *mainToolBar;

    // 菜单项
    QAction *resetLayoutAction;
    QAction *saveLayoutAction;
    QAction *loadLayoutAction;
};

#endif // MAINWINDOW_H
