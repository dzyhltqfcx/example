#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 设置应用程序信息
    QCoreApplication::setApplicationName("3D点云处理工具");
    QCoreApplication::setOrganizationName("YourCompany");

    MainWindow w;
    w.show();

    return a.exec();
}
