#ifndef CRYPTANALYSEVIG_H
#define CRYPTANALYSEVIG_H

#include <QWidget>
#include"Polygramme.h"
#include"TextV.h"

namespace Ui {
class CryptanalyseVig;
}

class CryptanalyseVig : public QWidget
{
    Q_OBJECT

public:
    explicit CryptanalyseVig(QWidget *parent = nullptr);
    ~CryptanalyseVig();
    void remplirText(TextV txt);
    void polygramsDetails();
    void pgcdFind();
    void indiceCalc();
    void colorText(int size);
    void sortPgcd();
    void divideText();
    void mostFrequent(int carAct);
    void initSteps();
     TextV myText;

private slots:
    void on_pushButton_clicked();

private:
    Ui::CryptanalyseVig *ui;
    int etape=1;
    int carActuel=0;
    int keySize;
    string un;
    string deux;
    string trois;
    string quatre;

};

#endif // CRYPTANALYSEVIG_H
