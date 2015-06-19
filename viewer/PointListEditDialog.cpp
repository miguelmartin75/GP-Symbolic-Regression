#include "PointListEditDialog.hpp"

#include "ErrorDialog.hpp"

PointListEditDialog::PointListEditDialog(QWidget *parent, PointList& points) :
    QDialog(parent),
    m_points(points)
{
    ui.setupUi(this);

    m_pointEditDialog = new PointEditDialog(this);
    m_pointEditDialog->setWindowTitle("Point Edit");

    m_generatePointDialog = new GeneratePointDialog(this, m_points);
    QObject::connect(m_generatePointDialog, SIGNAL(addedPoints()), this, SLOT(on_generatePointDialog_addedPoints()));
    m_generatePointDialog->setWindowTitle("Generate Points");

    refreshListView();
}

void PointListEditDialog::on_addButton_clicked()
{
    m_pointEditDialog->point(Point{0, 0});
    m_pointEditDialog->show();
    m_pointEditDialog->onOk([&](const Point& p)
    {
        std::cout << "adding point " << p.x << ", " << p.y << std::endl;
        if(pointExists(p))
        {
            errorDialog("You cannot add a point with an existing x value");
        }
        else
        {
            editPoints([&]
            {
                m_points.emplace_back(p);
            });
        }
    });
}

void PointListEditDialog::on_editButton_clicked()
{
    auto& pointToChange = m_points[ui.listWidget->currentIndex().row()];
    m_pointEditDialog->point(pointToChange);
    m_pointEditDialog->show();
    m_pointEditDialog->onOk([&](const Point& p) {
        if(p.x != pointToChange.x && pointExists(p))
        {
            errorDialog("You cannot change a point's x value to another existing x value");
        }
        else
        {
            editPoints([&]
            {
                pointToChange = p;
            });
        }
    });
}

void PointListEditDialog::on_removeButton_clicked()
{
    if(ui.listWidget->count() == 0) return;
    auto indexToRemove = ui.listWidget->currentIndex().row();
    editPoints([&]
    {
        m_points.erase(m_points.begin() + indexToRemove);
    });
}

void PointListEditDialog::on_generateButton_clicked()
{
    m_generatePointDialog->show();
    m_generatePointDialog->activateWindow();
    m_generatePointDialog->raise();
    m_generatePointDialog->setFocus();
}

void PointListEditDialog::refreshListView()
{
    std::cout << "refreshing list view" << std::endl;
    ui.listWidget->clear();
    for(auto& point : m_points)
    {
        QString string = "(" + QString::number(point.x) + ", " + QString::number(point.y) + ")";
        ui.listWidget->addItem(string);
    }
}

void PointListEditDialog::on_okButton_clicked()
{
    this->close();
}

void PointListEditDialog::on_generatePointDialog_addedPoints()
{
    update();
}
