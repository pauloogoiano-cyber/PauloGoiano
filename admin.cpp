#include "admin.h"
#include "sensormanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>

Admin::Admin(SensorManager *manager, QWidget *parent)
    : QWidget(parent),
    sensorManager(manager)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Inicializa o banco com o usuário padrão
    bancoUsuarios["admin"] = "1234";

    // Stylesheets reutilizáveis
    QString estiloBotaoAzul = "QPushButton { background-color: #2D6CDF; color: white; padding: 8px; border-radius: 6px; font-weight: bold; } QPushButton:hover { background-color: #1A52B8; }";
    QString estiloBotaoVermelho = "QPushButton { background-color: #EF4444; color: white; padding: 8px; border-radius: 6px; font-weight: bold; } QPushButton:hover { background-color: #DC2626; }";
    QString estiloInputs = "padding: 8px; border: 1px solid #CBD5E1; border-radius: 6px; background: #F8FAFC; color: #0F172A;";
    QString estiloListas = "QListWidget { background-color: white; border: 1px solid #E2E8F0; border-radius: 8px; padding: 5px; color: #0F172A; }";

    // =========================================================
    // 1. TELA DE LOGIN
    // =========================================================
    containerLogin = new QWidget(this);
    auto *loginLayout = new QVBoxLayout(containerLogin);
    loginLayout->setAlignment(Qt::AlignCenter);

    QWidget *cardLogin = new QWidget();
    cardLogin->setFixedWidth(400);
    cardLogin->setStyleSheet("background-color: white; border: 1px solid #E2E8F0; border-radius: 14px; padding: 25px;");
    auto *cardLayout = new QVBoxLayout(cardLogin);

    QLabel *lblTitulo = new QLabel("Área Restrita - Admin");
    lblTitulo->setStyleSheet("font-size: 20px; font-weight: bold; color: #0F172A; margin-bottom: 15px;");
    lblTitulo->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(lblTitulo);

    auto *formLayout = new QFormLayout();
    formLayout->setSpacing(10);
    txtUsuario = new QLineEdit();
    txtUsuario->setPlaceholderText("Digite o usuário");
    txtUsuario->setStyleSheet(estiloInputs);
    txtSenha = new QLineEdit();
    txtSenha->setPlaceholderText("Digite a senha");
    txtSenha->setEchoMode(QLineEdit::Password);
    txtSenha->setStyleSheet(estiloInputs);
    formLayout->addRow("Usuário:", txtUsuario);
    formLayout->addRow("Senha:", txtSenha);
    cardLayout->addLayout(formLayout);

    lblMensagemErro = new QLabel("");
    lblMensagemErro->setStyleSheet("color: #EF4444; font-size: 13px; font-weight: bold; margin-top: 5px;");
    lblMensagemErro->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(lblMensagemErro);

    btnEntrar = new QPushButton("Entrar");
    btnEntrar->setStyleSheet(estiloBotaoAzul + "QPushButton { padding: 10px; margin-top: 15px; }");
    cardLayout->addWidget(btnEntrar);

    loginLayout->addWidget(cardLogin);
    mainLayout->addWidget(containerLogin);

    // =========================================================
    // 2. PAINEL DE CONTROLE APÓS LOGIN (SENSORES + USUÁRIOS)
    // =========================================================
    containerPainel = new QWidget(this);
    auto *painelLayout = new QVBoxLayout(containerPainel);

    // Barra Superior do Painel com o Botão de Logout
    auto *barraSuperiorLayout = new QHBoxLayout();
    QLabel *lblBoasVindas = new QLabel("Painel Administrativo do Sistema");
    lblBoasVindas->setStyleSheet("font-size: 18px; font-weight: bold; color: #0F172A;");
    btnLogout = new QPushButton("Fazer Logout");
    btnLogout->setStyleSheet(estiloBotaoVermelho + "QPushButton { padding: 6px 15px; }");
    barraSuperiorLayout->addWidget(lblBoasVindas);
    barraSuperiorLayout->addStretch();
    barraSuperiorLayout->addWidget(btnLogout);
    painelLayout->addLayout(barraSuperiorLayout);

    // Layout de duas colunas (Esquerda: Sensores | Direita: Usuários)
    auto *colunasLayout = new QHBoxLayout();

    // --- COLUNA ESQUERDA: GERENCIAR SENSORES ---
    auto *boxSensores = new QGroupBox("Gerenciamento de Sensores");
    boxSensores->setStyleSheet("QGroupBox { font-weight: bold; color: #475569; }");
    auto *layoutEsquerda = new QVBoxLayout(boxSensores);

    btnAddAnemo = new QPushButton("Adicionar Anemômetro");
    btnAddPluvio = new QPushButton("Adicionar Pluviômetro");
    listaSensores = new QListWidget();
    btnRemoverSensor = new QPushButton("Remover Sensor Selecionado");

    btnAddAnemo->setStyleSheet(estiloBotaoAzul);
    btnAddPluvio->setStyleSheet(estiloBotaoAzul);
    listaSensores->setStyleSheet(estiloListas);
    btnRemoverSensor->setStyleSheet(estiloBotaoVermelho);

    layoutEsquerda->addWidget(btnAddAnemo);
    layoutEsquerda->addWidget(btnAddPluvio);
    layoutEsquerda->addWidget(listaSensores);
    layoutEsquerda->addWidget(btnRemoverSensor);
    colunasLayout->addWidget(boxSensores);

    // --- COLUNA DIREITA: GERENCIAR USUÁRIOS ---
    auto *boxUsuarios = new QGroupBox("Gerenciamento de Usuários (Acesso)");
    boxUsuarios->setStyleSheet("QGroupBox { font-weight: bold; color: #475569; }");
    auto *layoutDireita = new QVBoxLayout(boxUsuarios);

    auto *formNovoUser = new QFormLayout();
    txtNovoUsuario = new QLineEdit();
    txtNovoUsuario->setPlaceholderText("Novo Usuário");
    txtNovoUsuario->setStyleSheet(estiloInputs);
    txtNovaSenha = new QLineEdit();
    txtNovaSenha->setPlaceholderText("Nova Senha");
    txtNovaSenha->setEchoMode(QLineEdit::Password);
    txtNovaSenha->setStyleSheet(estiloInputs);
    formNovoUser->addRow("Usuário:", txtNovoUsuario);
    formNovoUser->addRow("Senha:", txtNovaSenha);
    layoutDireita->addLayout(formNovoUser);

    btnCadastrarUsuario = new QPushButton("Incluir Novo Usuário");
    btnCadastrarUsuario->setStyleSheet(estiloBotaoAzul);
    layoutDireita->addWidget(btnCadastrarUsuario);

    listaUsuarios = new QListWidget();
    listaUsuarios->setStyleSheet(estiloListas);
    // Lista inicializada com o admin padrão
    listaUsuarios->addItem("admin");
    layoutDireita->addWidget(listaUsuarios);

    btnRemoverUsuario = new QPushButton("Remover Usuário Selecionado");
    btnRemoverUsuario->setStyleSheet(estiloBotaoVermelho);
    layoutDireita->addWidget(btnRemoverUsuario);

    colunasLayout->addWidget(boxUsuarios);
    painelLayout->addLayout(colunasLayout);
    mainLayout->addWidget(containerPainel);

    // =========================================================
    // 3. CONFIGURAÇÕES INICIAIS E CONEXÕES
    // =========================================================
    containerLogin->setVisible(true);
    containerPainel->setVisible(false);

    // Conexões de Sessão (Login/Logout)
    connect(btnEntrar, &QPushButton::clicked, this, &Admin::realizarLogin);
    connect(txtSenha, &QLineEdit::returnPressed, this, &Admin::realizarLogin);
    connect(btnLogout, &QPushButton::clicked, this, &Admin::realizarLogout);

    // Conexões de Usuários
    connect(btnCadastrarUsuario, &QPushButton::clicked, this, &Admin::cadastrarUsuario);
    connect(btnRemoverUsuario, &QPushButton::clicked, this, &Admin::removerUsuario);

    // Conexões de Sensores (Suas originais)
    connect(btnAddAnemo, &QPushButton::clicked, this, [this]() {
        sensorManager->adicionarSensor("anemometro");
        listaSensores->addItem("Anemômetro");
    });
    connect(btnAddPluvio, &QPushButton::clicked, this, [this]() {
        sensorManager->adicionarSensor("pluviometro");
        listaSensores->addItem("Pluviômetro");
    });
    connect(btnRemoverSensor, &QPushButton::clicked, this, [this]() {
        const int row = listaSensores->currentRow();
        if (row < 0) return;
        sensorManager->removerSensor(row);
        delete listaSensores->takeItem(row);
    });
}

// Lógica de Validação Dinâmica contra o QMap
void Admin::realizarLogin()
{
    QString usuario = txtUsuario->text();
    QString senha = txtSenha->text();

    if (bancoUsuarios.contains(usuario) && bancoUsuarios[usuario] == senha) {
        lblMensagemErro->setText("");
        txtUsuario->clear();
        txtSenha->clear();
        containerLogin->setVisible(false);
        containerPainel->setVisible(true);
    } else {
        lblMensagemErro->setText("Usuário ou senha incorretos!");
        txtSenha->clear();
        txtSenha->setFocus();
    }
}

// Lógica de Logout
void Admin::realizarLogout()
{
    containerPainel->setVisible(false);
    containerLogin->setVisible(true);
    txtUsuario->setFocus();
}

// Lógica para Adicionar Usuário
void Admin::cadastrarUsuario()
{
    QString novoUser = txtNovoUsuario->text().trimmed();
    QString novaSenha = txtNovaSenha->text();

    if (novoUser.isEmpty() || novaSenha.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Preencha o usuário e a senha para cadastrar.");
        return;
    }

    if (bancoUsuarios.contains(novoUser)) {
        QMessageBox::warning(this, "Aviso", "Este usuário já está cadastrado.");
        return;
    }

    // Salva no mapa e coloca na interface gráfica
    bancoUsuarios[novoUser] = novaSenha;
    listaUsuarios->addItem(novoUser);

    txtNovoUsuario->clear();
    txtNovaSenha->clear();
    QMessageBox::information(this, "Sucesso", "Usuário cadastrado com sucesso!");
}

// Lógica para Remover Usuário
void Admin::removerUsuario()
{
    QListWidgetItem *item = listaUsuarios->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Aviso", "Selecione um usuário na lista para remover.");
        return;
    }

    QString userAlvo = item->text();

    if (userAlvo == "admin") {
        QMessageBox::critical(this, "Erro", "Por segurança, o usuário master 'admin' não pode ser removido.");
        return;
    }

    // Remove do mapa de dados e deleta o item visual da lista
    bancoUsuarios.remove(userAlvo);
    delete listaUsuarios->takeItem(listaUsuarios->currentRow());
    QMessageBox::information(this, "Sucesso", "Usuário removido com sucesso!");
}