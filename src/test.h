#ifndef TEST_H
#define TEST_H

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
#include <QString>

class test : public QWidget
{
    Q_OBJECT

    private:
    QLineEdit* lines[26];
    char alphabet[26];
    QTextEdit *edit;

    public:
    test();
    void f(int i, QString nouveau);
    void modify_text(std::string texte, char ancien, char nouveau);

    public slots:
    void reset();
};

#endif // TEST_H
