#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QObject>
#include <QTableWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QGridLayout>
#include "QCheckBox"

class resultwindow : public QWidget
{
    Q_OBJECT

    private:
    QString texte_crypte;
    QLineEdit* lines[26];
    char alphabet[26];
    QTextEdit *edit;
    QTabWidget *onglets;
    QWidget *page1;
    QWidget *page2;
    QWidget *page3;
    int type;

    public:
    resultwindow();
    resultwindow(QString txt,int type);
    ~resultwindow();
    void setTextCrypte(QString txt);
    void setTextDecrypte(std::string txt);
    void modify_char(int i, QString nouveau);
    void modify_text(std::string texte, char ancien, char nouveau);
    void setType(int);

    public slots:
    void reset();
    void save();
    void retour();
    void back_home();
    void addSpaces();
};

#endif // RESULTWINDOW_H
