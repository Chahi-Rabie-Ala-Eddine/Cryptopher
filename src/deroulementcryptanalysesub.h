#ifndef DEROULEMENTCRYPTANALYSESUB_H
#define DEROULEMENTCRYPTANALYSESUB_H

#include <QWidget>
#include <Polygramme.h>
#include <TextS.h>
#include <KeyS.h>
namespace Ui
{
    class deroulementcryptanalysesub;
}

class deroulementcryptanalysesub : public QWidget
{
    Q_OBJECT

    public:
    explicit deroulementcryptanalysesub(QWidget *parent = nullptr);
    ~deroulementcryptanalysesub();
    std::string generate_m_key();
    std::string new_text();
    void getQuadrigrams();
    double getCompatibility();
    bool getScore();

    private:
    Polygramme P;
    TextS* text;
    float score_max;
    float score;
    KeyS cle_mere;
    KeyS cle_fille;

    Ui::deroulementcryptanalysesub *ui;
};

#endif // DEROULEMENTCRYPTANALYSESUB_H
