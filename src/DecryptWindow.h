#ifndef DECRYPTWINDOW_H
#define DECRYPTWINDOW_H

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
#include "gestfichier.h"
#include <TextV.h>
#include <TextS.h>

class decryptwindow : public QWidget
{
    Q_OBJECT

    private:
    QTabWidget *onglets;
    QWidget *page1;
    QWidget *page2;
    QWidget *page3;
    QLineEdit* linedit;
    QComboBox *liste;
    QCheckBox *checkbox;
    QPushButton *bouton1;
    QGridLayout *layout;
    QLineEdit* lines[26];
    char alphabet[26];
    QPushButton* btn_g;
    QLabel *processLabel;
    QCheckBox* check;
    QMovie * movie;

    public slots:
    void process_decrypt();
    void result_decrypt();
    void key_type();
    void load();
    void back_home();
    void verify_check();
    void treat(QString input);


    public:
    QTextEdit *texedit;
    QLabel* m_label;
    decryptwindow();
    ~decryptwindow();
    void create_key(int i, QString nouveau);
    void setCurrent();
    static std::string currentText;

};

#endif // DECRYPTWINDOW_H
