#ifndef MANUALDIALOG_H
#define MANUALDIALOG_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class ManualDialog;
}

class ManualDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManualDialog(QWidget *parent = nullptr);
    ~ManualDialog();

private:
    Ui::ManualDialog *ui;
};

#endif // MANUALDIALOG_H
