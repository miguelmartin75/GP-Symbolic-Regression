#ifndef POINTEDITDIALOG_HPP
#define POINTEDITDIALOG_HPP

#include "ui_PointEditDialog.h"

class PointListEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PointListEditDialog(QWidget *parent = 0);

private slots:
    void on_yLineEdit_textChanged(const QString &arg1);

private:
    Ui::PointEditDialog ui;
};

#endif // POINTEDITDIALOG_HPP
