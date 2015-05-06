#ifndef RUNTHREAD_HPP
#define RUNTHREAD_HPP

#include <QThread>
#include <QDebug>

#include <SymbolicRegressionSolver.hpp>

class RunThread : public QThread
{
    Q_OBJECT

public:

    RunThread(const long long& sleepAmount, SymbolicRegressionSolver& solver) :
               m_sleepAmount(sleepAmount),
               m_solver(solver)
    {}

signals:

    void stepOccured();
    void stoppedRunning();

private:

    const long long& m_sleepAmount;
    SymbolicRegressionSolver& m_solver;

    void run()
    {
        qDebug() << "Running...";
        m_solver.setIsRunning(true);
        while(m_solver.isRunning() &&
              m_solver.currentGeneration() < m_solver.config().maxGenerations &&
              m_solver.currentSolutionSet().size() != 0 &&
              !m_solver.foundSolution())
        {
            m_solver.step();

            emit stepOccured();

            if(m_sleepAmount)
            {
                qDebug() << "sleeping for amount: " << QString::number(m_sleepAmount);
                QThread::currentThread()->sleep(m_sleepAmount);
            }
        }

        m_solver.setIsRunning(false);
        emit stoppedRunning();
    }
};

#endif // RUNTHREAD_HPP
