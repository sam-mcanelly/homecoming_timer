#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "student.h"
#include "db_controller.h"

/*----------------------------------------------
 *
 *      ===========mainwindow.h==========
 *
 *
 * MainWindow class that contains all of the
 * necessary function definitions for handling
 * events that occur on the main window.
 *
 * Author: Sam McAnelly
 * Last Revised: 5/21/2015
 *
 *
 * ---------------------------------------------*/

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void refresh_student_lists();
    void clear_student_lists();

private slots:
    void fill_name_list();
    void fill_status_list();
    void fill_hours_required_list();
    void fill_hours_complete_list();

    void on_btn_refresh_clicked();
    void on_btn_tsting_clicked();

    void on_list_name_currentRowChanged(int currentRow);

    void on_btn_find_clicked();

    void on_btn_clk_in_out_clicked();

    void on_combo_db_selection_currentIndexChanged(int index);

private:
    //Database Controller
    DB_Controller *controller;

    //current pomping week
    int curr_week;

    //database selection for either guys or girls
    int db_selection;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
