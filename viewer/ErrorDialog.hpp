#ifndef ERRORDIALOG_HPP
#define ERRORDIALOG_HPP

#include <QMessageBox>

static void errorDialog(const QString &str, int width = 500, int height = 200)
{
    QMessageBox messageBox;
    messageBox.critical(0, "Error", str);
    messageBox.setFixedSize(width, height);
}

#endif // ERRORDIALOG_HPP
