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

    // Banco de dados em memória para os usuários (Chave: Usuário, Valor: Senha)
    QMap<QString, QString> bancoUsuarios;

    // Componentes da Tela de Login
    QWidget *containerLogin;
    QLineEdit *txtUsuario;
    QLineEdit *txtSenha;
    QPushButton *btnEntrar;
    QLabel *lblMensagemErro;

    // Componentes do Painel de Controle Expandido
    QWidget *containerPainel;
    QPushButton *btnAddAnemo;
    QPushButton *btnAddPluvio;
    QPushButton *btnRemoverSensor;
    QListWidget *listaSensores;

    // Novos Componentes de Gerenciamento de Usuários e Logout
    QLineEdit *txtNovoUsuario;
    QLineEdit *txtNovaSenha;
    QPushButton *btnCadastrarUsuario;
    QPushButton *btnRemoverUsuario;
    QListWidget *listaUsuarios;
    QPushButton *btnLogout;
};

#endif // ADMIN_H