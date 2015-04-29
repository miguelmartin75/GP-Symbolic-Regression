#include "MainWindow.hpp"

#include <thread>
#include <boost/lexical_cast.hpp>

enum
{
    GRAPH_DATA_TO_PLOT = 0,
    CURRENT_PLOT = 1
};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);

    initGraph();
}


void MainWindow::initGraph()
{
    ui.plotWidget->xAxis->setLabel("x");
    ui.plotWidget->yAxis->setLabel("y");
    ui.plotWidget->setInteraction(QCP::iRangeDrag);
    ui.plotWidget->setInteraction(QCP::iRangeZoom);
    // add two graphs
    ui.plotWidget->addGraph();
    ui.plotWidget->addGraph();

    Function fn{"(+ x 1)"};

    const int start = -10;
    const int end = 10;
    PointList temp;
    for(int i = start; i <= end; i += 2)
    {
        temp.emplace_back(i, fn(i));
    }

    setPointsToModel(std::move(temp));

    reset();
}

void MainWindow::reset()
{
    m_solver.reset();
    updateInterface();
}

void MainWindow::setPointsToModel(PointList points)
{
    auto graph = ui.plotWidget->graph(0);
    graph->clearData();
    for(auto& point : points)
    {
        qDebug() << "adding point: (" << point.x << ", " << point.y << ")";
        graph->addData(point.x, point.y);
    }

    m_solver.setPoints(std::move(points));
}

void MainWindow::updateGraph(int graph, int index)
{
    if((size_t)index >= m_solver.currentSolutionSet().size())
        index = 0;

    auto& fn = m_solver.getFunction(index);

    auto g = ui.plotWidget->graph(graph);
    g->clearData();

    for(auto& point : m_solver.points())
    {
        g->addData(point.x, fn(point.x));
    }
    repaint();
    std::cout << "Updated graph " << fn << std::endl;
}

void MainWindow::step()
{
    m_solver.step();
    updateInterface();
}

void MainWindow::run()
{
    m_solver.solve();
    updateInterface();
    ui.runButton->setText("Run");
}

void MainWindow::updateInterface()
{
    qDebug() << "Updating the interface";
    ui.solutionsList->clear();
    for(auto& solution : m_solver.currentSolutionSet())
    {
        std::string str = "[" + boost::lexical_cast<std::string>(solution.fitnessLevel) + "] - " +
                          boost::lexical_cast<std::string>(solution.function) ;

        ui.solutionsList->addItem(QString{str.c_str()});
    }
}

void MainWindow::on_stepButton_clicked()
{
    step();
    /*
    std::thread thread{[&] { step() }};
    thread.detach();
    */
}

void MainWindow::on_runButton_clicked()
{
    if(!m_solver.isRunning())
    {
        std::thread runnerThread([&] {
            run();
        });

        runnerThread.detach();
        ui.runButton->setText("Stop");
    }
    else
    {
        ui.runButton->setText("Run");
        m_solver.stop();
    }
}

void MainWindow::on_resetButton_clicked()
{
    reset();
    updateInterface();
}

void MainWindow::on_solutionsList_currentRowChanged(int currentRow)
{
    updateGraph(CURRENT_PLOT, currentRow);
}
