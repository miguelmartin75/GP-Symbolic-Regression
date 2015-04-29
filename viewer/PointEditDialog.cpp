#include "PointEditDialog.hpp"

PointEditDialog::PointEditDialog(QWidget *parent, const Point& point) :
    QDialog(parent)
{
    ui.setupUi(this);

    QValidator* v = new QIntValidator();
    ui.xLineEdit->setValidator(v);
    ui.yLineEdit->setValidator(v);

    this->point(point);
}

void PointEditDialog::point(const Point &point)
{
    m_point = point;
    ui.xLineEdit->setText(QString::number(point.x));
    ui.yLineEdit->setText(QString::number(point.y));
}

void PointEditDialog::on_buttonBox_accepted()
{
    m_onOk(point());
}

void PointEditDialog::on_buttonBox_rejected()
{
    this->close();
}

void PointEditDialog::on_xLineEdit_textChanged(const QString &arg1)
{
    m_point.x = arg1.toInt();
}

void PointEditDialog::on_yLineEdit_textChanged(const QString &arg1)
{
    m_point.y = arg1.toInt();
}
