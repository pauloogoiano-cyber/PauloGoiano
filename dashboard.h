#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QVector>
#include <QDateTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QListWidget>

class QPushButton;
class QLabel;
class QTimer;
class QStackedLayout;
class QTextEdit;
class QWidget;
class CompassWidget;

struct Medicao
{
    int vento = 0;
    int chuva = 0;
    int direcao = 0;
    int temperatura = 0;
    QDateTime tempo;
};

class Dashboard : public QWidget
{
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

private slots:
    void abrirHome();
    void abrirAdmin();
    void abrirHistorico();

    void atualizarDados();
    void simularDados();
    void verificarSerial();

private:
    void setupUI();
    void setupStyle();
    void setupConnections();
    void mostrarPagina(QWidget *pagina);

private:
    QPushButton *btnHome = nullptr;
    QPushButton *btnAdmin = nullptr;
    QPushButton *btnHistorico = nullptr;
    QPushButton *btnAtualizar = nullptr;

    QWidget *paginaHome = nullptr;
    QWidget *paginaAdmin = nullptr;
    QWidget *paginaHistorico = nullptr;

    QStackedLayout *stack = nullptr;

    QLabel *lblVento = nullptr;
    QLabel *lblChuva = nullptr;
    QLabel *lblTemperatura = nullptr;
    QLabel *lblStatus = nullptr;

    QListWidget *listaUltimosDados = nullptr;
    QTextEdit *txtHistorico = nullptr;

    CompassWidget *compass = nullptr;

    QTimer *timer = nullptr;
    QTimer *timerSerial = nullptr;
    QSerialPort *serial = nullptr;

    Medicao ultimaMedicao;
    QVector<Medicao> historico;
};

#endif