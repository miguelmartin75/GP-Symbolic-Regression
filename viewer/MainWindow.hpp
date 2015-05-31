#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui_MainWindow.h"
#include "PointListEditDialog.hpp"
#include "ConfigDialog.hpp"

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

    void on_actionPoints_triggered();

    void on_actionStep_triggered();

    void on_actionReset_triggered();

    void on_actionRun_triggered();

    void on_runnerThread_stepOccured();

    void on_runnerThread_stoppedRunnnig();

    void on_actionPreferences_triggered();

    void on_actionImport_triggered();

    void on_sleepSlider_valueChanged(int value);

private:

    void setPointsToModel(PointList points);
    void initGraph();
    void step();
    void safeStep();
    void stop();
    void run();
    void updatePointsToModel();
    void updateInterface();
    void reset();
    void updateGraph(int graph, int index);
    void updateCurrentGen();

    Ui::MainWindow ui;

    // TODO: Refractor to it's own widget?
    SymbolicRegressionSolver m_solver;

    PointListEditDialog* m_pointListEditDialog = nullptr;
    ConfigDialog* m_config = nullptr;
    VariableMap m_variableMap;

    long long m_sleepAmount = 0;
    QMutex m_solverMutex;
};

#endif // MAINWINDOW_HPP
