#ifndef POINTEDITDIALOG_HPP
#define POINTEDITDIALOG_HPP

#include "ui_PointEditDialog.h"

class PointEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PointEditDialog(QWidget *parent = 0);

private:
    Ui::PointEditDialog ui;
};

#endif // POINTEDITDIALOG_HPP
