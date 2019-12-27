#ifndef DEROULEMENTVIGENERE_H
#define DEROULEMENTVIGENERE_H

#include <QWidget>

namespace Ui {
class DeroulementVigenere;
}

class DeroulementVigenere : public QWidget
{
    Q_OBJECT

public:
    explicit DeroulementVigenere(QWidget *parent = nullptr);
    void remplirCle(std::string key);
    void colorKey();
    void calculAdditionMod();
    void replaceClearWithCipher();
    void remplirText(std::string text,std::string key,int tratement);
    void colorText();
    ~DeroulementVigenere();

    void changeProgress();
private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::DeroulementVigenere *ui;
    int caractereEnCours=0;
    int etape=0;
    int typeOfTreatement;
    std::string keyCipher;
};

#endif // DEROULEMENTVIGENERE_H
