#include "mainwindow.h"
#include "setup_wizard.h"
#include <QApplication>
#include <QPushButton>
#include <stdio.h>              /* defines FILENAME_MAX */

#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif

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

char * curr_path;

static bool f_exists(const char *fileName);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    SetupWizard s;

    if (!f_exists("settings.hmt"))
    {
       w.show();
       s.show();
    }
    else
    {
       w.show();
    }


    return a.exec();
}

bool f_exists(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

