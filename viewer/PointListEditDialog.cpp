#include "pointeditdialog.h"
#include "ui_pointeditdialog.h"

PointListEditDialog::PointListEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PointEditDialog)
{
    ui->setupUi(this);
}

PointListEditDialog::~PointListEditDialog()
{
    delete ui;
}
