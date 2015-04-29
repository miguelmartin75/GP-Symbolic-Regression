#include "PointEditDialog.hpp"
#include "ui_PointEditDialog.h"

PointEditDialog::PointEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PointEditDialog)
{
    ui->setupUi(this);
}

PointEditDialog::~PointEditDialog()
{
    delete ui;
}
