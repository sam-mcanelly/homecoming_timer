#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>

/*----------------------------------------------
 *
 *      ===========main.cpp==========
 *
 *
 * main c++ file that is necessary to serve as
 * the starting point for the homecoming time
 * program. Creates the QApplication and
 * mainwindow then shows the window and
 * returns the output of the program.
 *
 * Author: Sam McAnelly
 * Last Revised: 5/21/2015
 *
 *
 * ---------------------------------------------*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
