#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QMap>

class SensorManager;

class Admin : public QWidget
{
    Q_OBJECT

public:
    explicit Admin(SensorManager *manager, QWidget *parent = nullptr);

private slots:
    void realizarLogin();
    void realizarLogout();
    void cadastrarUsuario();
    void removerUsuario();

private:
    SensorManager *sensorManager;
    QMap<QString, QString> bancoUsuarios;

    QWidget *containerLogin;
    QLineEdit *txtUsuario;
    QLineEdit *txtSenha;
    QPushButton *btnEntrar;
    QLabel *lblMgsErro;

    QWidget *containerPainel;
    QPushButton *btnAddAnemo;
    QPushButton *btnAddPluvio;
    QPushButton *btnRemoverSensor;
    QListWidget *listaSensores;

    QLineEdit *txtNovoUsuario;
    QLineEdit *txtNovaSenha;
    QPushButton *btnCadastrarUsuario;
    QPushButton *btnRemoverUsuario;
    QListWidget *listaUsuarios;
    QPushButton *btnLogout;
};

#endif