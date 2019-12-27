#ifndef DEROULEMENTCRYPTSUB_H
#define DEROULEMENTCRYPTSUB_H

#include <QWidget>

namespace Ui {
class DeroulementCryptSub;
}

class DeroulementCryptSub : public QWidget
{
    Q_OBJECT

public:
    explicit DeroulementCryptSub(QWidget *parent = nullptr);
    ~DeroulementCryptSub();

    void  EtapeSelectChar(int asciiCode);
    void EtapeChangeChar(int originalChar, int replacementChar);
    void remplirTabText(std::string text);
    void remplirKeyEncrypt(std::string key);
    void remplirKeyDecrypt(std::string key);
    void setType(int a);
    void changeProgress();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::DeroulementCryptSub *ui;
    int CaractereEnCours=0;
    int EtapeActuelle=0;
    int type;
};

#endif // DEROULEMENTCRYPTSUB_H
