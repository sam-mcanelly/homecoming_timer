#include "setup_wizard.h"
#include "ui_setup_wizard.h"

SetupWizard::SetupWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::SetupWizard)
{
    ui->setupUi(this);
}

SetupWizard::~SetupWizard()
{
    delete ui;
}
