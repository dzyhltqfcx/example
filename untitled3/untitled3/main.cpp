#include "mainwindow.h"

#include <QApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    cv::Mat img = cv::imread("D:\\versions\\1.png");
      namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
      imshow( "Display window", img );
    w.show();
    return a.exec();
}
