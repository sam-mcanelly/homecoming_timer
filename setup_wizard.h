#ifndef SETUP_WIZARD_H
#define SETUP_WIZARD_H

#include <QWizard>

namespace Ui {
class SetupWizard;
}

class SetupWizard : public QWizard
{
    Q_OBJECT

public:
    explicit SetupWizard(QWidget *parent = 0);
    ~SetupWizard();

private:
    Ui::SetupWizard *ui;
};

#endif // SETUP_WIZARD_H
