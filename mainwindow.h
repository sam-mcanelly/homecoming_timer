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

    void on_list_name_currentRowChanged(int currentRow);

    void on_btn_find_clicked();

    void on_btn_clk_in_out_clicked();

    void on_combo_db_selection_currentIndexChanged(int index);

    void on_btn_add_clicked();
    void on_btn_sort_clicked();

    void on_txt_cwid_name_textChanged(const QString &arg1);

    void on_btn_delete_clicked();

    void on_txt_add_card_textChanged(const QString &arg1);

    void on_btn_deduction_clicked();

    void on_btn_generate_report_clicked();

private:
    //Database Controller
    DB_Controller *controller;
    bool init;

    //Don't update certain things if we are sorting
    bool sorting;

    //current pomping week
    int curr_week;

    //database selection for either guys or girls
    int db_selection;

    static const QChar first_card_char;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
