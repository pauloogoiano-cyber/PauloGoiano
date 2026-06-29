#include "dashboard.h"
#include "compasswidget.h"
#include "admin.h"
#include "sensormanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QStackedLayout>
#include <QTimer>
#include <QRandomGenerator>
#include <QDateTime>
#include <QGroupBox>
#include <QListWidget> // 🟢 Incluído para a nova lista

// ================= CONSTRUTOR =================
Dashboard::Dashboard(QWidget *parent)
    : QWidget(parent)
{
    resize(1700, 950);

    setupUI();
    setupStyle();
    setupConnections();

    // Instancia o objeto serial
    serial = new QSerialPort(this);

    // Timer de dados atualizando a cada 10 segundos (10000 ms)
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Dashboard::simularDados);
    timer->start(10000);

    // Timer dedicado para checar a USB a cada 3 segundos
    timerSerial = new QTimer(this);
    connect(timerSerial, &QTimer::timeout, this, &Dashboard::verificarSerial);
    timerSerial->start(3000);

    // Executa as primeiras checagens imediatamente ao abrir
    simularDados();
    verificarSerial();
}

// Destrutor
Dashboard::~Dashboard()
{
    if (serial && serial->isOpen()) {
        serial->close();
    }
}

// ================= UI =================
void Dashboard::setupUI()
{
    auto *mainLayout = new QHBoxLayout(this);

    // MENU ESQUERDO
    QWidget *menuWidget = new QWidget();
    menuWidget->setFixedWidth(180);

    auto *menuLayout = new QVBoxLayout(menuWidget);
    btnHome = new QPushButton("Home");
    btnAdmin = new QPushButton("Admin");
    btnHistorico = new QPushButton("Histórico");
    btnAtualizar = new QPushButton("Atualizar");

    menuLayout->addWidget(btnHome);
    menuLayout->addWidget(btnAdmin);
    menuLayout->addWidget(btnHistorico);
    menuLayout->addWidget(btnAtualizar);
    menuLayout->addStretch();

    // HOME
    QWidget *homeWidget = new QWidget();
    auto *homeLayout = new QVBoxLayout(homeWidget);

    // 1. MOLDURA DE SEPARAÇÃO: DADOS DOS SENSORES
    auto *groupSensores = new QGroupBox("Leituras dos Sensores");
    auto *layoutSensoresGroup = new QVBoxLayout(groupSensores);

    // Card da Bússola
    QWidget *cardCompass = new QWidget();
    cardCompass->setObjectName("card");
    auto *compassLayout = new QVBoxLayout(cardCompass);
    compass = new CompassWidget();
    compass->setMinimumSize(320, 320);
    compassLayout->addWidget(compass, 0, Qt::AlignCenter);

    // Card do Vento
    QWidget *cardVento = new QWidget();
    cardVento->setObjectName("card");
    auto *ventoLayout = new QVBoxLayout(cardVento);
    lblVento = new QLabel("Velocidade do vento: -- m/s");
    lblVento->setAlignment(Qt::AlignCenter);
    ventoLayout->addWidget(lblVento);

    // Card da Chuva
    QWidget *cardChuva = new QWidget();
    cardChuva->setObjectName("card");
    auto *chuvaLayout = new QVBoxLayout(cardChuva);
    lblChuva = new QLabel("Pluviosidade: -- mm");
    lblChuva->setAlignment(Qt::AlignCenter);
    chuvaLayout->addWidget(lblChuva);

    // 🟢 NOVO: Card com a mini lista dos 5 últimos valores
    QWidget *cardLista = new QWidget();
    cardLista->setObjectName("card");
    auto *listaLayout = new QVBoxLayout(cardLista);

    QLabel *lblTituloLista = new QLabel("Últimas 5 Medições (Tempo Real):");
    lblTituloLista->setStyleSheet("font-weight: bold; color: #475569; font-size: 13px;");

    listaUltimosDados = new QListWidget();
    listaUltimosDados->setFixedHeight(130); // Altura fixa para não esticar a tela

    listaLayout->addWidget(lblTituloLista);
    listaLayout->addWidget(listaUltimosDados);

    // Adiciona todos os componentes na moldura de sensores
    layoutSensoresGroup->addWidget(cardCompass);
    layoutSensoresGroup->addWidget(cardVento);
    layoutSensoresGroup->addWidget(cardChuva);
    layoutSensoresGroup->addWidget(cardLista); // 🟢 Colocando a lista na tela

    // 2. MOLDURA DE SEPARAÇÃO: INFRAESTRUTURA / CONEXÃO
    auto *groupStatus = new QGroupBox("Comunicação e Conectividade");
    auto *layoutStatusGroup = new QVBoxLayout(groupStatus);

    // Card de Status da Conexão USB
    QWidget *cardStatus = new QWidget();
    cardStatus->setObjectName("card");
    auto *statusLayout = new QVBoxLayout(cardStatus);
    lblStatus = new QLabel("Status: Desconectado");
    lblStatus->setAlignment(Qt::AlignCenter);
    statusLayout->addWidget(lblStatus);

    layoutStatusGroup->addWidget(cardStatus);

    // Insere os blocos criados na página principal da Home
    homeLayout->addWidget(groupSensores);
    homeLayout->addWidget(groupStatus);
    homeLayout->addStretch();

    // HISTÓRICO
    paginaHistorico = new QWidget();
    auto *histLayout = new QVBoxLayout(paginaHistorico);
    txtHistorico = new QTextEdit();
    txtHistorico->setReadOnly(true);
    histLayout->addWidget(txtHistorico);

    // ADMIN
    paginaAdmin = new Admin(new SensorManager(this), this);

    // STACK
    stack = new QStackedLayout();
    paginaHome = homeWidget;

    stack->addWidget(paginaHome);
    stack->addWidget(paginaAdmin);
    stack->addWidget(paginaHistorico);

    mainLayout->addWidget(menuWidget);
    mainLayout->addLayout(stack);

    mostrarPagina(paginaHome);
}

// ================= STYLE SHEET =================
void Dashboard::setupStyle()
{
    setStyleSheet(R"(
        Dashboard {
            background-color: #F8FAFC;
        }

        QGroupBox {
            font-size: 14px;
            font-weight: bold;
            color: #475569;
            border: 1px solid #CBD5E1;
            border-radius: 8px;
            margin-top: 12px;
            padding-top: 15px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 15px;
            padding: 0 5px;
        }

        QPushButton {
            background-color: #2D6CDF;
            color: white;
            padding: 8px;
            border-radius: 6px;
            font-weight: bold;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #1A52B8;
        }
        QLabel {
            color: #1E293B;
            font-size: 15px;
            font-weight: 500;
        }
        QTextEdit {
            background-color: #FFFFFF;
            border: 1px solid #CBD5E1;
            border-radius: 8px;
            color: #0F172A;
            padding: 8px;
        }

        /* Estilização da mini lista da Home */
        QListWidget {
            background-color: #F8FAFC;
            border: 1px solid #E2E8F0;
            border-radius: 6px;
            color: #334155;
            font-size: 13px;
        }

        QWidget#card {
            background-color: #FFFFFF;
            border: 1px solid #E2E8F0;
            border-radius: 14px;
            padding: 12px;
        }
    )");
}

// ================= CONEXÕES =================
void Dashboard::setupConnections()
{
    connect(btnHome, &QPushButton::clicked, this, &Dashboard::abrirHome);
    connect(btnAdmin, &QPushButton::clicked, this, &Dashboard::abrirAdmin);
    connect(btnHistorico, &QPushButton::clicked, this, &Dashboard::abrirHistorico);
    connect(btnAtualizar, &QPushButton::clicked, this, &Dashboard::atualizarDados);
}

void Dashboard::mostrarPagina(QWidget *pagina) { stack->setCurrentWidget(pagina); }
void Dashboard::abrirHome() { mostrarPagina(paginaHome); }
void Dashboard::abrirAdmin() { mostrarPagina(paginaAdmin); }
void Dashboard::abrirHistorico() { mostrarPagina(paginaHistorico); }

// ================= PROCESSAMENTO DE DADOS =================
void Dashboard::simularDados()
{
    ultimaMedicao.vento = QRandomGenerator::global()->bounded(0, 120);
    ultimaMedicao.chuva = QRandomGenerator::global()->bounded(0, 80);
    ultimaMedicao.direcao = QRandomGenerator::global()->bounded(360);
    ultimaMedicao.tempo = QDateTime::currentDateTime();

    historico.append(ultimaMedicao);
    atualizarDados();
}

void Dashboard::atualizarDados()
{
    static auto dirTexto = [](int g) {
        const QStringList d = {"NORTE","NORDESTE","LESTE","SUDESTE","SUL","SUDOESTE","OESTE","NOROESTE"};
        return d[(g + 22) / 45 % 8];
    };

    QString direcaoStr = dirTexto(ultimaMedicao.direcao);

    lblVento->setText("Velocidade do vento: " + QString::number(ultimaMedicao.vento) + " m/s");
    lblChuva->setText("Pluviosidade: " + QString::number(ultimaMedicao.chuva) + " mm");

    if (compass)
        compass->setDirecao(ultimaMedicao.direcao);

    // 🟢 LÓGICA DOS 5 ÚLTIMOS VALORES NA HOME
    if (listaUltimosDados) {
        // Monta o texto simplificado da linha
        QString novaLinha = ultimaMedicao.tempo.toString("hh:mm:ss") +
                            " : Vento: " + QString::number(ultimaMedicao.vento) + " m/s" +
                            " | Chuva: " + QString::number(ultimaMedicao.chuva) + " mm" +
                            " | Direção: " + direcaoStr;

        // Insere sempre no topo (índice 0)
        listaUltimosDados->insertItem(0, novaLinha);

        // Se a lista passar de 5 itens, joga fora o mais antigo (o último do índice)
        while (listaUltimosDados->count() > 5) {
            delete listaUltimosDados->takeItem(listaUltimosDados->count() - 1);
        }
    }

    // Mantém o histórico completo na outra aba intacto
    txtHistorico->append(
        ultimaMedicao.tempo.toString("hh:mm:ss") +
        " | Vento: " + QString::number(ultimaMedicao.vento) +
        " m/s | Chuva: " + QString::number(ultimaMedicao.chuva) +
        " mm | Direção: " + direcaoStr
        );
}

// ================= VERIFICAÇÃO DA PORTA USB =================
void Dashboard::verificarSerial()
{
    if (serial->isOpen()) {
        QList<QSerialPortInfo> portasDisponiveis = QSerialPortInfo::availablePorts();
        bool aindaExiste = false;

        for (const QSerialPortInfo &info : qAsConst(portasDisponiveis)) {
            if (info.portName() == serial->portName()) {
                aindaExiste = true;
                break;
            }
        }

        if (aindaExiste) {
            lblStatus->setText("Status: Conectado (" + serial->portName() + ")");
            lblStatus->setStyleSheet("color: #10B981; font-weight: bold;");
            return;
        } else {
            serial->close();
        }
    }

    QList<QSerialPortInfo> portas = QSerialPortInfo::availablePorts();

    if (portas.isEmpty()) {
        lblStatus->setText("Status: Desconectado (Nenhuma USB detectada)");
        lblStatus->setStyleSheet("color: #EF4444; font-weight: bold;");
        return;
    }

    QSerialPortInfo portaAlvo = portas.first();

    serial->setPort(portaAlvo);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        lblStatus->setText("Status: Conectado (" + portaAlvo.portName() + ")");
        lblStatus->setStyleSheet("color: #10B981; font-weight: bold;");
    } else {
        lblStatus->setText("Status: Erro ao abrir a porta " + portaAlvo.portName());
        lblStatus->setStyleSheet("color: #F59E0B; font-weight: bold;");
    }
}