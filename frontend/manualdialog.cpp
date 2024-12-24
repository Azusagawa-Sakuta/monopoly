#include "manualdialog.h"
#include "ui_manualdialog.h"

ManualDialog::ManualDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ManualDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Manual");

    ui->textBrowser->set
}

ManualDialog::~ManualDialog()
{
    delete ui;
}
