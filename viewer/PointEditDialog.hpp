#ifndef POINTEDITDIALOG_HPP
#define POINTEDITDIALOG_HPP

#include <QDialog>

#include <functional>

#include <Point.hpp>

#include "ui_PointEditDialog.h"

class PointEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PointEditDialog(QWidget *parent = 0, const Point& point = Point{});

    void point(const Point& point);
    const Point& point() { return m_point; }

    void onOk(std::function<void(const Point&)> fn) { m_onOk = fn; }

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_xLineEdit_textChanged(const QString &arg1);

    void on_yLineEdit_textChanged(const QString &arg1);

private:
    Ui::PointEditDialog ui;

    Point m_point;
    std::function<void(const Point&)> m_onOk;
};

#endif // POINTEDITDIALOG_HPP
