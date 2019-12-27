#ifndef DEROULEMENTCRYPTANALYSESUBSTITUTION_H
#define DEROULEMENTCRYPTANALYSESUBSTITUTION_H

#include <QWidget>
#include <Polygramme.h>
#include <TextS.h>
#include <KeyS.h>
#include <QTextEdit>

class DeroulementCryptanalyseSubstitution : public QWidget
{
    Q_OBJECT

    public:
    DeroulementCryptanalyseSubstitution(string m_text);
    std::string generate_m_key();
    std::string new_text();
    void getQuadrigrams();
    double getCompatibility();
    bool getScore();
    void process();
    void end();
    string permute_key();

    private:
    Polygramme P;
    TextS* text;
    double m_score_max;
    double m_score;
    QTextEdit* texte;
    QTextEdit* txt;
    int step = 0;
    int turn = 1;
    string textSub;
    string key_mother;
    string key_daughter;
    string init_text;

    public slots:
    void process_steps();

};

#endif // DEROULEMENTCRYPTANALYSESUBSTITUTION_H
