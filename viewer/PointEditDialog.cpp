#include "PointEditDialog.hpp"

PointEditDialog::PointEditDialog(QWidget *parent, const Point& point) :
    QDialog(parent), m_point(point)
{
    ui.setupUi(this);
}
