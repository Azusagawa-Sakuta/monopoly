#include "helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
}
