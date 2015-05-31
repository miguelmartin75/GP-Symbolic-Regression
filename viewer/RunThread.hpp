#ifndef RUNTHREAD_HPP
#define RUNTHREAD_HPP

#include <QThread>
#include <QDebug>

#include <SymbolicRegressionSolver.hpp>

class RunThread : public QThread
{
    Q_OBJECT

public:

    RunThread(const long long& sleepAmount, SymbolicRegressionSolver& solver, QMutex& solverMutex) :
               m_sleepAmount(sleepAmount),
               m_solver(solver),
               m_solverMutex(solverMutex)
    {}

signals:

    void stepOccured();
    void stoppedRunning();

private:

    const long long& m_sleepAmount;
    SymbolicRegressionSolver& m_solver;
    QMutex& m_solverMutex;

    void run() override;
};

#endif // RUNTHREAD_HPP
