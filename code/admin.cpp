#include "admin.h"
#include "sensormanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>

Admin::Admin(SensorManager *manager, QWidget *parent)
    : QWidget(parent), sensorManager(manager)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    bancoUsuarios["admin"] = "1234";

    QString estiloBtnAzul = "QPushButton { background-color: #2D6CDF; color: white; padding: 8px; border-radius: 6px; font-weight: bold; } QPushButton:hover { background-color: #1A52B8; }";
    QString estiloBtnVermelho = "QPushButton { background-color: #EF4444; color: white; padding: 8px; border-radius: 6px; font-weight: bold; } QPushButton:hover { background-color: #DC2626; }";
    QString estiloInputs = "padding: 8px; border: 1px solid #CBD5E1; border-radius: 6px; background: #F8FAFC; color: #0F172A;";
    QString estiloListas = "QListWidget { background-color: white; border: 1px solid #E2E8F0; border-radius: 8px; padding: 5px; color: #0F172A; }";

    // LOGIN TELA
    containerLogin = new QWidget(this);
    auto *loginLayout = new QVBoxLayout(containerLogin);
    loginLayout->setAlignment(Qt::AlignCenter);

    QWidget *cardLogin = new QWidget();
    cardLogin->setFixedWidth(400);
    cardLogin->setStyleSheet("background-color: white; border: 1px solid #E2E8F0; border-radius: 14px; padding: 25px;");
    auto *cardLayout = new QVBoxLayout(cardLogin);

    QLabel *lblTitulo = new QLabel("Área Restrita - Admin");
    lblTitulo->setStyleSheet("font-size: 20px; font-weight: bold; color: #0F172A; margin-bottom: 15px;");
    cardLayout->addWidget(lblTitulo);

    auto *formLayout = new QFormLayout();
    txtUsuario = new QLineEdit(); txtUsuario->setStyleSheet(estiloInputs);
    txtSenha = new QLineEdit(); txtSenha->setEchoMode(QLineEdit::Password); txtSenha->setStyleSheet(estiloInputs);
    formLayout->addRow("Usuário:", txtUsuario);
    formLayout->addRow("Senha:", txtSenha);
    cardLayout->addLayout(formLayout);

    lblMgsErro = new QLabel(""); lblMgsErro->setStyleSheet("color: #EF4444; font-weight: bold;");
    cardLayout->addWidget(lblMgsErro);

    btnEntrar = new QPushButton("Entrar"); btnEntrar->setStyleSheet(estiloBtnAzul);
    cardLayout->addWidget(btnEntrar);
    loginLayout->addWidget(cardLogin);
    mainLayout->addWidget(containerLogin);

    // PAINEL APÓS LOGIN
    containerPainel = new QWidget(this);
    auto *painelLayout = new QVBoxLayout(containerPainel);

    auto *barraSup = new QHBoxLayout();
    QLabel *lblPainel = new QLabel("Painel Administrativo");
    lblPainel->setStyleSheet("font-size: 18px; font-weight: bold; color: #0F172A;");
    btnLogout = new QPushButton("Logout"); btnLogout->setStyleSheet(estiloBtnVermelho);
    barraSup->addWidget(lblPainel); barraSup->addStretch(); barraSup->addWidget(btnLogout);
    painelLayout->addLayout(barraSup);

    auto *colunasLayout = new QHBoxLayout();

    // COLUNA SENSORES
    auto *boxSensores = new QGroupBox("Sensores");
    auto *layEsq = new QVBoxLayout(boxSensores);
    btnAddAnemo = new QPushButton("Adicionar Anemômetro"); btnAddAnemo->setStyleSheet(estiloBtnAzul);
    btnAddPluvio = new QPushButton("Adicionar Pluviômetro"); btnAddPluvio->setStyleSheet(estiloBtnAzul);
    listaSensores = new QListWidget(); listaSensores->setStyleSheet(estiloListas);
    btnRemoverSensor = new QPushButton("Remover Sensor"); btnRemoverSensor->setStyleSheet(estiloBtnVermelho);
    layEsq->addWidget(btnAddAnemo); layEsq->addWidget(btnAddPluvio); layEsq->addWidget(listaSensores); layEsq->addWidget(btnRemoverSensor);
    colunasLayout->addWidget(boxSensores);

    // COLUNA USUÁRIOS
    auto *boxUsers = new QGroupBox("Usuários");
    auto *layDir = new QVBoxLayout(boxUsers);
    auto *formN = new QFormLayout();
    txtNovoUsuario = new QLineEdit(); txtNovoUsuario->setStyleSheet(estiloInputs);
    txtNovaSenha = new QLineEdit(); txtNovaSenha->setEchoMode(QLineEdit::Password); txtNovaSenha->setStyleSheet(estiloInputs);
    formN->addRow("Usuário:", txtNovoUsuario);
    formN->addRow("Senha:", txtNovaSenha);
    layDir->addLayout(formN);

    btnCadastrarUsuario = new QPushButton("Incluir Usuário"); btnCadastrarUsuario->setStyleSheet(estiloBtnAzul);
    listaUsuarios = new QListWidget(); listaUsuarios->setStyleSheet(estiloListas); listaUsuarios->addItem("admin");
    btnRemoverUsuario = new QPushButton("Remover Usuário"); btnRemoverUsuario->setStyleSheet(estiloBtnVermelho);
    layDir->addWidget(btnCadastrarUsuario); layDir->addWidget(listaUsuarios); layDir->addWidget(btnRemoverUsuario);
    colunasLayout->addWidget(boxUsers);

    painelLayout->addLayout(colunasLayout);
    mainLayout->addWidget(containerPainel);

    containerLogin->setVisible(true);
    containerPainel->setVisible(false);

    connect(btnEntrar, &QPushButton::clicked, this, &Admin::realizarLogin);
    connect(txtSenha, &QLineEdit::returnPressed, this, &Admin::realizarLogin);
    connect(btnLogout, &QPushButton::clicked, this, &Admin::realizarLogout);
    connect(btnCadastrarUsuario, &QPushButton::clicked, this, &Admin::cadastrarUsuario);
    connect(btnRemoverUsuario, &QPushButton::clicked, this, &Admin::removerUsuario);

    connect(btnAddAnemo, &QPushButton::clicked, this, [this]() { sensorManager->adicionarSensor("anemometro"); listaSensores->addItem("Anemômetro"); });
    connect(btnAddPluvio, &QPushButton::clicked, this, [this]() { sensorManager->adicionarSensor("pluviometro"); listaSensores->addItem("Pluviômetro"); });
    connect(btnRemoverSensor, &QPushButton::clicked, this, [this]() {
        int r = listaSensores->currentRow(); if (r < 0) return;
        sensorManager->removerSensor(r); delete listaSensores->takeItem(r);
    });
}

void Admin::realizarLogin() {
    if (bancoUsuarios.contains(txtUsuario->text()) && bancoUsuarios[txtUsuario->text()] == txtSenha->text()) {
        lblMgsErro->setText(""); txtUsuario->clear(); txtSenha->clear();
        containerLogin->setVisible(false); containerPainel->setVisible(true);
    } else { lblMgsErro->setText("Dados Incorretos!"); txtSenha->clear(); }
}

void Admin::realizarLogout() { containerPainel->setVisible(false); containerLogin->setVisible(true); }

void Admin::cadastrarUsuario() {
    QString u = txtNovoUsuario->text().trimmed(); QString s = txtNovaSenha->text();
    if (u.isEmpty() || s.isEmpty() || bancoUsuarios.contains(u)) return;
    bancoUsuarios[u] = s; listaUsuarios->addItem(u);
    txtNovoUsuario->clear(); txtNovaSenha->clear();
}

void Admin::removerUsuario() {
    QListWidgetItem *it = listaUsuarios->currentItem();
    if (!it || it->text() == "admin") return;
    bancoUsuarios.remove(it->text()); delete listaUsuarios->takeItem(listaUsuarios->currentRow());
}