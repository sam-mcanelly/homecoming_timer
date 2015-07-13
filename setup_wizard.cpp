#include "setup_wizard.h"
#include "ui_setup_wizard.h"
#include "mainwindow.h"

SetupWizard::SetupWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::SetupWizard)
{
    ui->setupUi(this);

    guy_db = "guys";
    girl_db = "girls";

    guy_fine = 10.0;
    girl_fine = 10.0;
}

SetupWizard::~SetupWizard()
{
    delete ui;
}

void SetupWizard::on_SetupWizard_accepted()
{
    QDir dir = QDir::currentPath();
    dir.mkdir("hoco_db");
    std::ofstream my_settings;
    my_settings.open("settings.hmt");
    my_settings << dir.absolutePath().toStdString() << "/hoco_db/" << guy_db.toStdString() << "/\n";
    my_settings << dir.absolutePath().toStdString() << "/hoco_db/" << girl_db.toStdString() << "/\n";
    my_settings << guy_fine << "\n";
    my_settings << girl_fine << "\n";
    dir.cd("hoco_db");
    dir.mkdir(guy_db.toStdString().c_str());
    dir.mkdir(girl_db.toStdString().c_str());
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    dir.mkdir("reports");
    my_settings << dir.absolutePath().toStdString() << "/reports/" << "\n";
    my_settings.close();
}

void SetupWizard::on_ln_frat_textChanged()
{
    guy_db = ui->ln_frat->text();
    qDebug("%s", guy_db.toStdString().c_str());
}

void SetupWizard::on_ln_sor_textChanged()
{
    girl_db = ui->ln_sor->text();
    qDebug("%s", girl_db.toStdString().c_str());
}

void SetupWizard::on_spin_guy_fine_valueChanged()
{
    guy_fine = (float) ui->spin_guy_fine->value();
    qDebug("%f", guy_fine);
}

void SetupWizard::on_spin_girl_fine_valueChanged()
{
    girl_fine = (float) ui->spin_girl_fine->value();
    qDebug("%f", girl_fine);
}
