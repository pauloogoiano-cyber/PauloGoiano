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

Dashboard::Dashboard(QWidget *parent)
    : QWidget(parent)
{
    resize(1700, 950);

    setupUI();
    setupStyle();
    setupConnections();

    serial = new QSerialPort(this);

    // Timer de dados (10 segundos)
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Dashboard::simularDados);
    timer->start(10000);

    // Timer da USB (3 segundos)
    timerSerial = new QTimer(this);
    connect(timerSerial, &QTimer::timeout, this, &Dashboard::verificarSerial);
    timerSerial->start(3000);

    simularDados();
    verificarSerial();
}

Dashboard::~Dashboard()
{
    if (serial && serial->isOpen()) {
        serial->close();
    }
}

void Dashboard::setupUI()
{
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // MENU LATERAL AZUL
    QWidget *menuWidget = new QWidget();
    menuWidget->setObjectName("menuLateral");
    menuWidget->setFixedWidth(200);

    auto *menuLayout = new QVBoxLayout(menuWidget);
    menuLayout->setContentsMargins(15, 30, 15, 30);
    menuLayout->setSpacing(15);

    QLabel *lblMenuLogo = new QLabel("☰  PAINEL");
    lblMenuLogo->setStyleSheet("color: white; font-size: 18px; font-weight: bold; margin-bottom: 20px;");
    lblMenuLogo->setAlignment(Qt::AlignCenter);
    menuLayout->addWidget(lblMenuLogo);

    btnHome = new QPushButton("⌂  Home");
    btnAdmin = new QPushButton("⚙  Admin");
    btnHistorico = new QPushButton("📋  Histórico");
    btnAtualizar = new QPushButton("🔄  Atualizar");

    menuLayout->addWidget(btnHome);
    menuLayout->addWidget(btnAdmin);
    menuLayout->addWidget(btnHistorico);
    menuLayout->addWidget(btnAtualizar);
    menuLayout->addStretch();

    // PAINEL CENTRAL
    QWidget *conteudoWidget = new QWidget();
    conteudoWidget->setObjectName("conteudoPrincipal");
    auto *conteudoLayout = new QVBoxLayout(conteudoWidget);
    conteudoLayout->setContentsMargins(30, 30, 30, 30);
    conteudoLayout->setSpacing(20);

    auto *headerLayout = new QHBoxLayout();
    auto *vHeaderTxt = new QVBoxLayout();
    QLabel *lblNomeUsuario = new QLabel("Olá, Operador");
    lblNomeUsuario->setStyleSheet("font-size: 28px; font-weight: bold; color: #0F172A; padding: 0; margin: 0;");
    QLabel *lblSubTitulo = new QLabel("Bem-vindo de volta ao seu centro de controle");
    lblSubTitulo->setStyleSheet("font-size: 14px; color: #64748B; padding: 0;");
    vHeaderTxt->addWidget(lblNomeUsuario);
    vHeaderTxt->addWidget(lblSubTitulo);
    headerLayout->addLayout(vHeaderTxt);
    headerLayout->addStretch();
    conteudoLayout->addLayout(headerLayout);

    paginaHome = new QWidget();
    auto *homeLayout = new QVBoxLayout(paginaHome);
    homeLayout->setContentsMargins(0, 0, 0, 0);
    homeLayout->setSpacing(20);

    // Destaque Temperatura e Cards Rápidos
    auto *linhaSuperiorLayout = new QHBoxLayout();
    linhaSuperiorLayout->setSpacing(20);

    QWidget *cardWeatherHighlight = new QWidget();
    cardWeatherHighlight->setObjectName("cardClimaDestaque");
    cardWeatherHighlight->setMinimumHeight(180);
    auto *weatherHighlightLayout = new QVBoxLayout(cardWeatherHighlight);

    QLabel *lblWeatherTitle = new QLabel("TEMPO REAL");
    lblWeatherTitle->setStyleSheet("color: rgba(255,255,255,0.8); font-size: 13px; font-weight: bold;");
    lblTemperatura = new QLabel("--°C");
    lblTemperatura->setStyleSheet("color: white; font-size: 48px; font-weight: bold;");
    QLabel *lblWeatherDesc = new QLabel("Monitoramento Estação Meteorológica");
    lblWeatherDesc->setStyleSheet("color: white; font-size: 14px;");

    weatherHighlightLayout->addWidget(lblWeatherTitle);
    weatherHighlightLayout->addWidget(lblTemperatura);
    weatherHighlightLayout->addWidget(lblWeatherDesc);
    linhaSuperiorLayout->addWidget(cardWeatherHighlight, 2);

    QWidget *cardQuickStats = new QWidget();
    cardQuickStats->setObjectName("cardBranco");
    auto *quickStatsLayout = new QVBoxLayout(cardQuickStats);

    QLabel *lblQuickTitle = new QLabel("Dados Rápidos do Sistema");
    lblQuickTitle->setStyleSheet("font-weight: bold; color: #475569; font-size: 13px; margin-bottom: 5px;");
    quickStatsLayout->addWidget(lblQuickTitle);

    auto *gridStats = new QHBoxLayout();
    lblVento = new QLabel("Vento\n-- m/s");
    lblChuva = new QLabel("Chuva\n-- mm");
    lblVento->setAlignment(Qt::AlignCenter);
    lblChuva->setAlignment(Qt::AlignCenter);
    lblVento->setStyleSheet("font-size: 16px; font-weight: bold; color: #2D6CDF; background: #F0F5FF; padding: 15px; border-radius: 10px;");
    lblChuva->setStyleSheet("font-size: 16px; font-weight: bold; color: #F59E0B; background: #FFFBEB; padding: 15px; border-radius: 10px;");
    gridStats->addWidget(lblVento);
    gridStats->addWidget(lblChuva);
    quickStatsLayout->addLayout(gridStats);
    linhaSuperiorLayout->addWidget(cardQuickStats, 3);
    homeLayout->addLayout(linhaSuperiorLayout);

    // Bússola e Mini Lista
    auto *linhaCentralLayout = new QHBoxLayout();
    linhaCentralLayout->setSpacing(20);

    QWidget *cardCompass = new QWidget();
    cardCompass->setObjectName("cardBranco");
    auto *compassLayout = new QVBoxLayout(cardCompass);
    compassLayout->setAlignment(Qt::AlignCenter);

    QLabel *lblTitleCompass = new QLabel("Direção Analógica do Vento");
    lblTitleCompass->setStyleSheet("font-weight: bold; color: #475569; font-size: 13px;");
    lblTitleCompass->setAlignment(Qt::AlignCenter);

    compass = new CompassWidget();
    compass->setMinimumSize(300, 300);

    compassLayout->addWidget(lblTitleCompass);
    compassLayout->addWidget(compass);
    linhaCentralLayout->addWidget(cardCompass, 2);

    QWidget *cardLista = new QWidget();
    cardLista->setObjectName("cardBranco");
    auto *listaLayout = new QVBoxLayout(cardLista);

    QLabel *lblTituloLista = new QLabel("Histórico Recente (Últimas 5 Linhas)");
    lblTituloLista->setStyleSheet("font-weight: bold; color: #475569; font-size: 13px;");

    listaUltimosDados = new QListWidget();
    listaUltimosDados->setStyleSheet("background: #F8FAFC; border: none; border-radius: 8px; color: #334155; padding: 5px;");

    listaLayout->addWidget(lblTituloLista);
    listaLayout->addWidget(listaUltimosDados);
    linhaCentralLayout->addWidget(cardLista, 3);
    homeLayout->addLayout(linhaCentralLayout);

    // Status Conexão
    QWidget *cardStatus = new QWidget();
    cardStatus->setObjectName("cardBranco");
    auto *statusLayout = new QVBoxLayout(cardStatus);
    lblStatus = new QLabel("Status: Verificando...");
    lblStatus->setAlignment(Qt::AlignCenter);
    statusLayout->addWidget(lblStatus);
    homeLayout->addWidget(cardStatus);

    paginaHistorico = new QWidget();
    auto *histLayout = new QVBoxLayout(paginaHistorico);
    txtHistorico = new QTextEdit();
    txtHistorico->setReadOnly(true);
    histLayout->addWidget(txtHistorico);

    paginaAdmin = new Admin(new SensorManager(this), this);

    stack = new QStackedLayout();
    stack->addWidget(paginaHome);
    stack->addWidget(paginaAdmin);
    stack->addWidget(paginaHistorico);
    conteudoLayout->addLayout(stack);

    mainLayout->addWidget(menuWidget);
    mainLayout->addWidget(conteudoWidget);
    mostrarPagina(paginaHome);
}

void Dashboard::setupStyle()
{
    setStyleSheet(R"(
        QWidget#conteudoPrincipal { background-color: #F3F7FA; }
        QWidget#menuLateral { background-color: #1E88E5; }
        QWidget#menuLateral QPushButton {
            background-color: transparent; color: rgba(255, 255, 255, 0.85);
            border: none; text-align: left; padding: 12px 20px; font-size: 15px; font-weight: 500; border-radius: 8px;
        }
        QWidget#menuLateral QPushButton:hover { background-color: rgba(255, 255, 255, 0.15); color: white; }
        QWidget#cardBranco { background-color: #FFFFFF; border: none; border-radius: 18px; padding: 18px; }
        QWidget#cardClimaDestaque {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1E88E5, stop:1 #0D47A1);
            border-radius: 18px; padding: 20px;
        }
        QTextEdit { background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 12px; color: #0F172A; padding: 10px; }
    )");
}

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

void Dashboard::simularDados()
{
    ultimaMedicao.temperatura = QRandomGenerator::global()->bounded(14, 39);
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
        const QStringList d = {
            "NORTE", "NORDESTE", "LESTE", "SUDESTE",
            "SUL", "SUDOESTE", "OESTE", "NOROESTE"
        };
        return d[(g + 22) / 45 % 8];
    };

    QString direcaoStr = dirTexto(ultimaMedicao.direcao);

    lblTemperatura->setText(QString::number(ultimaMedicao.temperatura) + "°C");
    lblVento->setText("Vento\n" + QString::number(ultimaMedicao.vento) + " m/s");
    lblChuva->setText("Chuva\n" + QString::number(ultimaMedicao.chuva) + " mm");

    if (compass)
        compass->setDirecao(ultimaMedicao.direcao);

    if (listaUltimosDados) {
        QString novaLinha = ultimaMedicao.tempo.toString("hh:mm:ss") +
                            " -> T: " + QString::number(ultimaMedicao.temperatura) + "°C" +
                            " | V: " + QString::number(ultimaMedicao.vento) + " m/s" +
                            " | Dir: " + direcaoStr;

        listaUltimosDados->insertItem(0, novaLinha);
        while (listaUltimosDados->count() > 5) {
            delete listaUltimosDados->takeItem(listaUltimosDados->count() - 1);
        }
    }

    txtHistorico->append(
        ultimaMedicao.tempo.toString("hh:mm:ss") +
        " | Temp: " + QString::number(ultimaMedicao.temperatura) + "°C" +
        " | Vento: " + QString::number(ultimaMedicao.vento) + " m/s" +
        " | Chuva: " + QString::number(ultimaMedicao.chuva) + " mm" +
        " | Direção: " + direcaoStr
        );
}

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
            lblStatus->setStyleSheet("color: #10B981; font-weight: bold; font-size: 14px;");
            return;
        } else {
            serial->close();
        }
    }

    QList<QSerialPortInfo> portas = QSerialPortInfo::availablePorts();

    if (portas.isEmpty()) {
        lblStatus->setText("Status: Desconectado (Nenhuma USB detectada)");
        lblStatus->setStyleSheet("color: #EF4444; font-weight: bold; font-size: 14px;");
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
        lblStatus->setStyleSheet("color: #10B981; font-weight: bold; font-size: 14px;");
    } else {
        lblStatus->setText("Status: Erro ao abrir a porta " + portaAlvo.portName());
        lblStatus->setStyleSheet("color: #F59E0B; font-weight: bold; font-size: 14px;");
    }
}