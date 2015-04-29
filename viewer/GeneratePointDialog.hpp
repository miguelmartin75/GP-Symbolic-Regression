#ifndef GENERATEPOINTDIALOG_HPP
#define GENERATEPOINTDIALOG_HPP

#include "ui_GeneratePointDialog.h"

#include <Function.hpp>
#include <Types.hpp>

class GeneratePointDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GeneratePointDialog(QWidget *parent, PointList& points);

signals:

    void addedPoints();

private slots:
    void on_addPointsButton_clicked();

    void on_eraseAndAddButton_clicked();

    void on_cancelButton_clicked();

private:

    void updateVars();

    Ui::GeneratePointDialog ui;

    int m_start, m_end, m_stepSize;
    PointList& m_points;
};

#endif // GENERATEPOINTDIALOG_HPP
