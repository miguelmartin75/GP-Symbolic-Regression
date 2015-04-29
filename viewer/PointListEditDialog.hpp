#ifndef POINTLISTEDITDIALOG_HPP
#define POINTLISTEDITDIALOG_HPP

#include <memory>

#include "PointEditDialog.hpp"

#include "ui_PointListEditDialog.h"

#include <iostream>

class PointListEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PointListEditDialog(QWidget *parent = 0);
    ~PointListEditDialog(){
        std::cout << "testing 1 2 3" << std::endl;
    }

private slots:
    void on_addButton_clicked();

private:
    Ui::PointListEditDialog ui;

    PointEditDialog* m_pointEditDialog;
};

#endif // POINTLISTEDITDIALOG_HPP
