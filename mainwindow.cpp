/*---------------------------------------------
 *                  INCLUDES
 * --------------------------------------------*/

/*      project file headers    */

#include "mainwindow.h"
#include "ui_mainwindow.h"

/*      c++ library headers    */

#include <fstream>
#include <iostream>

/*         QT headers          */

#include <QDir>
#include <QMessageBox>
#include <QScrollBar>

/*----------------------------------------------
 *
 *      ===========mainwindow.cpp==========
 *
 *
 * Function definitions for the mainwindow class.
 * These handle all ui events and control what the
 * user sees on the screen
 *
 * Author: Sam McAnelly
 * Last Revised: 5/21/2015
 *
 *
 * ---------------------------------------------*/

/*----------------------------------------------
 *          DEFINITIONS AND ENUMS
 * --------------------------------------------*/


/* ----------------------------------------------
 *          GLOBAL INSTANCE VARIABLES
 * ----------------------------------------------*/
const QChar MainWindow::first_card_char = '%';


/*------------------------------------------------
 *          MAINWINDOW FUNCTION DEFS
 * -----------------------------------------------*/

void MainWindow::clear_student_lists()
{
    ui->list_name->clear();
    ui->list_status->clear();
    ui->list_hrs->clear();
    ui->list_req->clear();
}

void MainWindow::refresh_student_lists()
{
    fill_name_list();
    fill_status_list();
    fill_hours_complete_list();
    fill_hours_required_list();
}


void MainWindow::fill_name_list()
{
    //clear the name list widgets first just in case
    ui->list_name->clear();

    int i    = 0;
    int end  = 0;

    end      = controller->get_student_count();

    QString curr_name;
    while ( i < end )
    {
        curr_name = QString::fromStdString(controller->get_name_from_index(i));
        QListWidgetItem *itm = new QListWidgetItem;
        itm->setText(curr_name);
        ui->list_name->insertItem(i, itm);
        ++i;
    }
}

void MainWindow::fill_status_list()
{
    //clear the status list first for good measure
    ui->list_status->clear();

    int i = 0;
    bool status;


    while(i < controller->get_student_count())
    {
        status = controller->get_status_from_index(i);
        QListWidgetItem *itm = new QListWidgetItem;
        if (status == false)
        {
            itm->setText(QString("Clocked Out"));
            itm->setTextColor(QColor(255,0,0));
        }
        else
        {
            itm->setText(QString("Clocked In"));
            itm->setTextColor(QColor(0, 205, 0));
        }
        ui->list_status->insertItem(i, itm);
        ++i;
    }

}
void MainWindow::fill_hours_required_list()
{
    //clear the name list widgets first just in case
    ui->list_req->clear();
    int i = 0;
    QString final;

    while (i < controller->get_student_count())
    {
        float curr_time = controller->get_hours_req_from_index(i);
        QListWidgetItem *itm = new QListWidgetItem;
        final.sprintf("%05.2f", curr_time);
        itm->setText(final);
        ui->list_req->insertItem(i, itm);
        ++i;
    }
}
void MainWindow::fill_hours_complete_list()
{
    //clear the hours complete list widgets first just in case
    ui->list_hrs->clear();
    int i = 0;
    float curr_time;
    QString final;

    while (i < controller->get_student_count())
    {
        curr_time = controller->get_hours_comp_from_index(i);
        QListWidgetItem *itm = new QListWidgetItem;
        final.sprintf("%05.2f", curr_time);
        itm->setText(final);
        ui->list_hrs->insertItem(i, itm);
        ++i;
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init       = false;
    controller = new DB_Controller();
}

MainWindow::~MainWindow()
{
    qDebug("> Deleting ui...");
    delete ui;
    qDebug("> ui deleted");
    if( init )
    {
        controller->end();
    }

    qDebug("> Deleting database controller memory...");
    delete controller;
    qDebug("> Database controller memory deleted");
}

//Syncing the QListViewWidgets
void MainWindow::on_list_name_currentRowChanged(int currentRow)
{
    QModelIndex index;
    int scroll_index;

    index = ui->list_name->currentIndex();
    QScrollBar *bar = ui->list_name->verticalScrollBar();
    scroll_index = bar->value();

    ui->list_status->setCurrentRow(currentRow);
    bar = ui->list_status->verticalScrollBar();
    bar->setValue(scroll_index);

    ui->list_hrs->setCurrentRow(currentRow);
    bar = ui->list_hrs->verticalScrollBar();
    bar->setValue(scroll_index);

    ui->list_req->setCurrentRow(currentRow);
    bar = ui->list_req->verticalScrollBar();
    bar->setValue(scroll_index);

    ui->list_name->setCurrentRow(currentRow);
    bar = ui->list_name->verticalScrollBar();
    bar->setValue(scroll_index);

    ui->txt_cwid_name->setText(QString::fromStdString(controller->get_name_from_index(ui->list_name->currentRow())));
}

void MainWindow::on_btn_find_clicked()
{
    if (!init)
    {
        QMessageBox box;
        box.setText("Select a database!");
        box.exec();
        return;
    }

    int index = controller->search_name(ui->txt_cwid_name->text().toUtf8().constData());
    if(index == -1)
        index = controller->search_card_number(ui->txt_cwid_name->text().toUtf8().constData());
    if(index != -1)
        ui->list_name->setCurrentRow(index);
    else
    {
        QMessageBox box;
        box.setText("Name not found!");
        box.exec();
    }
}

void MainWindow::on_btn_clk_in_out_clicked()
{
    if (!init)
    {
        QMessageBox box;
        box.setText("Select a database!");
        box.exec();
        return;
    }

    int index;

    std::string name = ui->txt_cwid_name->text().toUtf8().constData();
    index = ui->list_name->currentRow();
    if(name.compare("") == 0)
    {
        controller->toggle_status_from_index(index);
        fill_status_list();
        if(!controller->get_status_from_index(index))
            fill_hours_complete_list();
        ui->list_name->setCurrentRow(index);
    }
    else
    {
        index = controller->search_name(ui->txt_cwid_name->text().toUtf8().constData());
        if(index == -1)
            index = controller->search_card_number(ui->txt_cwid_name->text().toUtf8().constData());
        if(index == -1)
        {
            QMessageBox box;
            box.setText("Not found!");
            box.exec();
        }
        else
        {
            controller->toggle_status_from_index(index);
            fill_status_list();
            fill_hours_complete_list();

            /*----------------------------------------------
             *     Reset all rows to the appropriate
             *     index.
             * -------------------------------------------*/
            ui->list_name->setCurrentRow(index);
            ui->list_hrs->setCurrentRow(index);
            ui->list_req->setCurrentRow(index);
            ui->list_status->setCurrentRow(index);
        }
    }
}

void MainWindow::on_combo_db_selection_currentIndexChanged(int index)
{
    if (!init)
    {
       controller->begin();
       init = true;
    }

    switch(index)
    {
    case 0:
        clear_student_lists();
        return;
    case 1:
        controller->set_gender(DB_Controller::GUYS);
        break;
    case 2:
        controller->set_gender(DB_Controller::GIRLS);
        break;
    }


    refresh_student_lists();
}

void MainWindow::on_btn_add_clicked()
{
    if(!init)
    {
        QMessageBox box;
        box.setText("Select a database!");
        box.exec();
        return;
    }

    if( false == ui->txt_add_name->text().contains(" ") )
    {
        QMessageBox box;
        box.setText("Student name improperly formatted!");
        box.exec();
        return;
    }

    if( ui->txt_add_card->text().length() < 10 )
    {
        QMessageBox box;
        box.setText("Invalid card number!");
        box.exec();
        return;
    }

    controller->add_student(ui->txt_add_name->text().toStdString(), ui->txt_add_card->text().toStdString(), ::atof(ui->combo_hours_req->currentText().toStdString().c_str()));
    refresh_student_lists();

    /*------------------------
     * clear the text fields
     * ----------------------*/
    ui->txt_add_card->clear();
    ui->txt_add_name->clear();

}

void MainWindow::on_btn_sort_clicked()
{
    if (!init)
    {
        return;
    }

    controller->sort( (DB_Sort::sort_by)ui->combo_sort_by->currentIndex() );
    refresh_student_lists();
}

void MainWindow::on_txt_cwid_name_textChanged(const QString &arg1)
{
    if( ui->txt_cwid_name->text().contains(first_card_char) )
    {
        ui->txt_cwid_name->setEchoMode(QLineEdit::Password);
    }
    else
    {
        ui->txt_cwid_name->setEchoMode(QLineEdit::Normal);
    }
}

void MainWindow::on_btn_delete_clicked()
{
    if( !init )
    {
        return;
    }

    /*----------------------------
     * This action is costly
     * Use it sparingly
     * -------------------------*/
    controller->delete_student(ui->list_name->currentRow());
    refresh_student_lists();
}

void MainWindow::on_txt_add_card_textChanged(const QString &arg1)
{

    /*----------------------------
     * Generate the name from the
     * card information.
     * -------------------------*/

    QString first_name;
    QString last_name;
    QString final_name;
    const char*   name;
    int     idx;

    idx = 0;
    name = arg1.toStdString().c_str();

    qDebug("%s", name);

    /*----------------------------
     * Generate the name from the
     * card information.
     * -------------------------*/
    while( name[ idx ] != '^' )
    {
        if( name[ idx ] == '\0' )
        {
            return;
        }
        idx++;
    }
    idx++;
    last_name.append( QChar( name[ idx++ ] ) );
    while( name[ idx ] != '/' )
    {
        if( name[ idx ] == '\0' )
        {
            return;
        }
        last_name.append(QChar( tolower(name[ idx++ ] ) ));
        qDebug( last_name.toStdString().c_str() );
    }
    idx++;


    if( name[ idx ] == '\0' )
    {
        return;
    }
    first_name.append( name[ idx++ ] );
    if( name[ idx ] == '\0' )
    {
        return;
    }
    while( name[ idx ] != ' ')
    {
        if( name[ idx ] == '\0' )
        {
            return;
        }
        first_name.append( QChar( tolower( name[ idx++ ] ) ) );
    }
    first_name.append(' ');

    final_name = first_name + last_name;

    ui->txt_add_name->setText( final_name );

}

void MainWindow::on_btn_deduction_clicked()
{
    if( !init )
    {
        return;
    }

    controller->set_hours_deducted_from_index(ui->list_name->currentRow(), ::atof(ui->spin_deductions->text().toStdString().c_str() ) );
    refresh_student_lists();
}
