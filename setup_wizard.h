#ifndef SETUP_WIZARD_H
#define SETUP_WIZARD_H

#include <QWizard>
#include <QObject>

#include <iostream>
#include <fstream>

#include "mainwindow.h"

namespace Ui {
class SetupWizard;
}

class SetupWizard : public QWizard
{
    Q_OBJECT

public:
    explicit SetupWizard(QWidget *parent = 0);
    ~SetupWizard();

private slots:
    void on_SetupWizard_accepted();

    void on_ln_frat_textChanged();

    void on_ln_sor_textChanged();

    void on_spin_guy_fine_valueChanged();

    void on_spin_girl_fine_valueChanged();

private:
    QString guy_db;
    QString girl_db;

    float guy_fine;
    float girl_fine;

    Ui::SetupWizard *ui;
};

#endif // SETUP_WIZARD_H
