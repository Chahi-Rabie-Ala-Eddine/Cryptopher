#include <QLabel>
#include <QMovie>
#include "EncryptWindow.h"
#include "DecryptWindow.h"
#include "MainWindow.h"

mainwindow::mainwindow() : QWidget()
{
    //taille de la fenetre principal
    setFixedSize(900, 500);
    //texte de la fenetre
    this->setWindowTitle("Outil automatique d'aide au décryptage");
    //un Objet Qlabel qui contient l'image de fond
    //fond = new QLabel(this);
    //fond->setPixmap(QPixmap("/home/aladin/outil/home"));
    onglets = new QTabWidget(this);
    onglets->setGeometry(10, 10, 880, 480);

    QMovie *movie = new QMovie("/home/aladin/Documents/Projet Final Outil automatique de décryptage/logo.gif");
    QLabel *processLabel = new QLabel(this);
    processLabel->setMovie(movie);
    movie->start();
    processLabel->move(250,12);

    QPixmap monImage("/home/aladin/Documents/Projet Final Outil automatique de décryptage/img/logo_univ.png");
    QLabel* m_label = new QLabel(this);
    m_label->setPixmap(monImage);
    m_label->move(740,390);

    //feuille de style css pour modifier les styles des widgets
    setStyleSheet("QTabWidget { background-color: white} QPushButton { background-color: yellow } QTabWidget::hover { background-color: red } QPushButton:hover { background-color: red } ");

    //gestion du boutton Encrypt
    Encrypt = new QPushButton("Encrypt",this);
    Encrypt->setToolTip("Crypter un text clair !");
    Encrypt->setFont(QFont("Comic Sans MS", 15));
    Encrypt->setCursor(Qt::PointingHandCursor);
    Encrypt->setGeometry(200,330,150, 35);
    QObject::connect(Encrypt, SIGNAL(clicked()), this, SLOT(cypher()));

    //gestion du boutton Decrypt
    Decrypt = new QPushButton("Decrypt",this);
    Decrypt->setToolTip("Décrypter un text chiffré !");
    Decrypt->setFont(QFont("Comic Sans MS", 15));
    Decrypt->setCursor(Qt::PointingHandCursor);
    Decrypt->setGeometry(550,330,150, 35);
    QObject::connect(Decrypt, SIGNAL(clicked()), this, SLOT(encypher()));

}
//les slot de la fenetre
void mainwindow::cypher()
{
    QWidget::hide();
    encryptwindow* my_window = new encryptwindow;
    my_window-> show();
}
void mainwindow::encypher()
{
    QWidget::hide();
    decryptwindow* my_window = new decryptwindow;
    my_window-> show();
}
