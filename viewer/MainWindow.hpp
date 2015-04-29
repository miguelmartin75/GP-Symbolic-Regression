#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui_MainWindow.h"

#include <SymbolicRegressionSolver.hpp>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private slots:

    void on_stepButton_clicked();

    void on_runButton_clicked();

    void on_resetButton_clicked();

    void on_solutionsList_currentRowChanged(int currentRow);

private:

    void setPointsToModel(PointList points);
    void initGraph();
    void step();
    void run();
    void updateInterface();
    void reset();
    void updateGraph(int graph, int index);

    Ui::MainWindow ui;

    // TODO: Refractor to it's own widget?
    SymbolicRegressionSolver m_solver;
};

#endif // MAINWINDOW_HPP
