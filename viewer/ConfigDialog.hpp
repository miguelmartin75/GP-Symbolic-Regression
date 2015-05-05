#ifndef CONFIGDIALOG_HPP
#define CONFIGDIALOG_HPP

#include "ui_ConfigDialog.h"

#include <SymbolicRegressionSolver.hpp>

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    using Config = SymbolicRegressionSolver::Config;

    explicit ConfigDialog(QWidget *parent, Config& config);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:

    Ui::ConfigDialog ui;
    Config& m_config;
};

#endif // CONFIGDIALOG_HPP
