#include "MainWindow.hpp"

#include <fstream>
#include <cctype>

#include <boost/lexical_cast.hpp>

#include "PointListEditDialog.hpp"
#include "ErrorDialog.hpp"
#include "RunThread.hpp"

void showDialog(QDialog& dialog)
{
    dialog.show();
    dialog.activateWindow();
    dialog.raise();
    dialog.setFocus();
}

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

    ui.plotWidget->graph(GRAPH_DATA_TO_PLOT)->setPen(QPen(QColor(200, 50, 0)));

    Function fn{parse("(+ x 1)").statement};

    const int start = -10;
    const int end = 10;
    PointList temp;
    for(int i = start; i <= end; i += 2)
    {
        temp.emplace_back(i, fn(m_variableMap, i));
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
    m_solver.setPoints(std::move(points));
    updatePointsToModel();
}

void MainWindow::updatePointsToModel()
{
    auto& points = m_solver.points();
    auto graph = ui.plotWidget->graph(0);
    graph->clearData();
    for(auto& point : points)
    {
        qDebug() << "adding point: (" << point.x << ", " << point.y << ")";
        graph->addData(point.x, point.y);
    }
    ui.plotWidget->replot();
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
        g->addData(point.x, fn(m_variableMap, point.x));
    }
    ui.plotWidget->replot();
    std::cout << "Updated graph " << fn << std::endl;
}

void MainWindow::step()
{
    m_solver.step();
    updateInterface();
}

void MainWindow::safeStep()
{
    if(m_solver.isRunning())
    {
        errorDialog("Cannot step whilst the solver is running...");
        return;
    }
    step();
}

void MainWindow::run()
{
    qDebug() << "Run notification";
    if(!m_solver.isRunning())
    {
        qDebug() << "Attempting to run";
        RunThread* thread = new RunThread(m_sleepAmount, m_solver);
        QObject::connect(thread, SIGNAL(stepOccured()), this, SLOT(on_runnerThread_stepOccured()));
        QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        QObject::connect(thread, SIGNAL(stoppedRunning()), this, SLOT(on_runnerThread_stoppedRunnnig()));
        thread->start();

        ui.runButton->setText("Stop");
        ui.actionRun->setText("Stop");
    }
    else
    {
        stop();
    }
}

void MainWindow::stop()
{
    ui.runButton->setText("Run");
    ui.actionRun->setText("Run");
    m_solver.stop();
}

void MainWindow::updateInterface()
{
    qDebug() << "Updating the interface";
    ui.solutionsList->clear();
    for(auto& solution : m_solver.currentSolutionSet())
    {
        try
        {
            std::string str = "[" + boost::lexical_cast<std::string>(solution.fitnessLevel) + "] - " +
                              boost::lexical_cast<std::string>(solution.function) ;

            ui.solutionsList->addItem(QString{str.c_str()});
        }
        catch(...)
        {
            qDebug() << "Exception caught!";
        }
    }

    ui.currentGenerationLabel->setText("Current Generation: " + QString::number(m_solver.currentGeneration()));
}

void MainWindow::on_stepButton_clicked()
{
    safeStep();
}

void MainWindow::on_runButton_clicked()
{
    run();
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

void MainWindow::on_actionPoints_triggered()
{
    if(!m_pointListEditDialog)
    {
        m_pointListEditDialog = new PointListEditDialog(this, m_solver.points());
        m_pointListEditDialog->onUpdate([&]
        {
            updatePointsToModel();
        });
        m_pointListEditDialog->setWindowTitle("Edit Points");
    }
    showDialog(*m_pointListEditDialog);
}

void MainWindow::on_actionStep_triggered()
{
    safeStep();
}

void MainWindow::on_actionReset_triggered()
{
    reset();
}

void MainWindow::on_actionRun_triggered()
{
    run();
}

void MainWindow::on_runnerThread_stepOccured()
{
    qDebug() << "step event triggered";
    updateInterface();
}

void MainWindow::on_runnerThread_stoppedRunnnig()
{
    qDebug() << "Finished";
    ui.runButton->setText("Run");
    ui.actionRun->setText("Run");
}

void MainWindow::on_actionPreferences_triggered()
{
    if(!m_config)
    {
        m_config = new ConfigDialog(this, m_solver.config());
    }

    showDialog(*m_config);
}

void MainWindow::on_actionImport_triggered()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Open a CSV file", tr("CSV (*.csv)"));
    if(filepath.isEmpty()) return;

    std::ifstream file{filepath.toStdString()};
    if(!file)
    {
        errorDialog("No such file");
    }

    PointList points;
    std::string buffer;
    try
    {
        while(std::getline(file, buffer))
        {
            int index = -1;
            for(auto ch : buffer)
            {
                ++index;
                if(std::isspace(ch) || ch == ',')
                {
                    break;
                }
            }

            std::string xAsStr{buffer.begin(), buffer.begin() + index};
            std::string yAsStr{buffer.begin() + index + 1, buffer.end()};
            auto x = boost::lexical_cast<int>(xAsStr);
            auto y = boost::lexical_cast<int>(yAsStr);
            points.emplace_back(x, y);
        }
    }
    catch(boost::bad_lexical_cast& e)
    {
        std::string reason = e.what();
        errorDialog(QString::fromStdString("Failed to read file. Reason " + reason));
    }

    if(points.size() != 0)
    {
        setPointsToModel(std::move(points));
    }
}

void MainWindow::on_sleepSlider_valueChanged(int value)
{
    m_sleepAmount = value;
    ui.sleepLabel->setText(QString::fromStdString("Sleep Amount " + boost::lexical_cast<std::string>(value) + " ms"));
}
