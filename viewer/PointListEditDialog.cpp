#include "PointListEditDialog.hpp"

PointListEditDialog::PointListEditDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
}

void PointListEditDialog::on_addButton_clicked()
{
    m_pointEditDialog = new PointEditDialog(this);
}
