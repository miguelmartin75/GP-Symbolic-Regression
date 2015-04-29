#ifndef POINTLISTEDITDIALOG_HPP
#define POINTLISTEDITDIALOG_HPP

#include <memory>
#include <functional>

#include <Types.hpp>

#include "PointEditDialog.hpp"
#include "GeneratePointDialog.hpp"

#include "ui_PointListEditDialog.h"

class PointListEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PointListEditDialog(QWidget *parent, PointList& points);

    void onUpdate(std::function<void()> fn) { m_onUpdate = fn; }

private slots:
    void on_addButton_clicked();

    void on_editButton_clicked();

    void on_removeButton_clicked();

    void on_generateButton_clicked();

    void on_okButton_clicked();

    void on_generatePointDialog_addedPoints();

private:

    void refreshListView();
    void error(const QString& str);
    bool pointExists(const Point& p) const
    {
        auto it = std::find_if(m_points.begin(), m_points.end(),
                              [&](const Point& point) { return point.x == p.x; });
        return it != m_points.end();
    }

    void update() { refreshListView(); m_onUpdate(); }

    template <class F>
    void editPoints(F f)
    {
        f();
        std::sort(m_points.begin(), m_points.end());
        update();
    }

    Ui::PointListEditDialog ui;

    PointEditDialog* m_pointEditDialog = nullptr;
    GeneratePointDialog* m_generatePointDialog = nullptr;
    std::function<void()> m_onUpdate;
    PointList& m_points;
};

#endif // POINTLISTEDITDIALOG_HPP
