#include "RunThread.hpp"

void RunThread::run()
{
    qDebug() << "Running...";
    m_solver.setIsRunning(true);
    while(m_solver.isRunning() &&
          m_solver.currentGeneration() < m_solver.config().maxGenerations &&
          m_solver.currentSolutionSet().size() != 0 &&
          !m_solver.foundSolution())
    {
        qDebug() << "Stepping...\n";
        m_solver.step();

        emit stepOccured();

        if(m_sleepAmount)
        {
            qDebug() << "sleeping for amount: " << QString::number(m_sleepAmount);
            QThread::msleep(m_sleepAmount);
            qDebug() << "done sleeping...\n";
        }
    }

    m_solver.setIsRunning(false);
    emit stoppedRunning();
}
