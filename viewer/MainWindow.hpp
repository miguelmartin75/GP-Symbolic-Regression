#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private:
    Ui::MainWindow ui;
};

#endif // MAINWINDOW_HPP
