#ifndef ENCRYPTWINDOW_H
#define ENCRYPTWINDOW_H

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
#include <QStackedWidget>

class encryptwindow : public QWidget
{
    Q_OBJECT

    private:
    QTextEdit *texedit;
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
    QStackedWidget* stack;
    QPushButton* btn_g;
    QPushButton* btn_six;

    public slots:
    void process_encrypt();
    void result_encrypt();
    void key_type();
    void load();
    void back_home();
    void key_creator();
    void treat(QString input);

    public:
    encryptwindow();
    ~encryptwindow();
    void setCurrent();
    void create_key(int i, QString nouveau);
    static std::string currentText;

};

#endif // ENCRYPTWINDOW_H
