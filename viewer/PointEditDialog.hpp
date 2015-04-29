#ifndef POINTEDITDIALOG_HPP
#define POINTEDITDIALOG_HPP

#include <QDialog>

#include <Point.hpp>

#include "ui_PointEditDialog.h"

class PointEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PointEditDialog(QWidget *parent = 0, const Point& point = Point{});

    const Point& point() { return m_point; }

private:
    Ui::PointEditDialog ui;

    Point m_point;
};

#endif // POINTEDITDIALOG_HPP
