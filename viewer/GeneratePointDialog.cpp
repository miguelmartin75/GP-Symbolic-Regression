#include "GeneratePointDialog.hpp"

#include "ErrorDialog.hpp"

GeneratePointDialog::GeneratePointDialog(QWidget *parent, PointList& points) :
    QDialog(parent),
    m_points(points)
{
    ui.setupUi(this);

    ui.functionLineEdit->setText("(+ 1 x)");
    ui.startLineEdit->setText("0");
    ui.endLineEdit->setText("10");
    ui.stepSizeLineEdit->setText("2");

    QValidator* v = new QIntValidator();
    ui.startLineEdit->setValidator(v);
    ui.endLineEdit->setValidator(v);
    ui.stepSizeLineEdit->setValidator(v);
}


void GeneratePointDialog::on_addPointsButton_clicked()
{
    Function function{ui.functionLineEdit->text().toStdString()};
    updateVars();

    bool failed = false;
    PointList pointsToAdd;
    VariableMap map;
    for(int i = m_start; i < m_end; i += m_stepSize)
    {
        auto y = function(map, i);
        auto it = std::find_if(m_points.begin(), m_points.end(),
                               [&](const Point& p) { return p.x == i; });

        if(it == m_points.end())
        {
            pointsToAdd.emplace_back(i, y);
        }
        else
        {
            failed = true;
            break;
        }
    }

    if(failed)
    {
        errorDialog("Failed to add function as intercepts with current points");
        return;
    }

    for(auto& p : pointsToAdd)
    {
        m_points.emplace_back(p);
    }

    emit addedPoints();
}

void GeneratePointDialog::on_eraseAndAddButton_clicked()
{
    Function function{ui.functionLineEdit->text().toStdString()};
    updateVars();

    m_points.clear();
    VariableMap map;
    for(int i = m_start; i < m_end; i += m_stepSize)
    {
        m_points.emplace_back(i, function(map,i));
    }

    emit addedPoints();
}

void GeneratePointDialog::on_cancelButton_clicked()
{
    this->close();
}

void GeneratePointDialog::updateVars()
{
    m_start = ui.startLineEdit->text().toInt();
    m_end = ui.endLineEdit->text().toInt();
    m_stepSize = ui.stepSizeLineEdit->text().toInt();
}
