#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QObject>
#include <QLabel>
#include <qtabwidget.h>

class mainwindow : public QWidget
{
    Q_OBJECT

    public:
    mainwindow();

    private:
    QPushButton* Encrypt;
    QPushButton* Decrypt;
    QLabel *fond;
    QTabWidget *onglets;

    public slots:
    void cypher();
    void encypher();
};


#endif // MAINWINDOW_H
