#include "sensoranemometro.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QSizePolicy>

static const QString TITULO_STYLE =
    "background:#EEF2F7;"
    "color:#082B66;"
    "font-size:18px;"
    "font-weight:700;"
    "padding:14px;"
    "border-top-left-radius:16px;"
    "border-top-right-radius:16px;";

static const QString VALOR_STYLE =
    "font-size:28px;"
    "font-weight:700;"
    "color:#2D3748;"
    "padding:8px;";

static const QString CARD_STYLE =
    "background:white;"
    "border:1px solid #DDE3EA;"
    "border-radius:16px;";

sensoranemometro::sensoranemometro(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(CARD_STYLE);

    setMinimumSize(420, 240);

    setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding);

    auto *titulo = new QLabel("ANEMÔMETRO", this);
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet(TITULO_STYLE);

    labelVelocidade = new QLabel("-- km/h", this);
    labelDirecao = new QLabel("Direção: --", this);

    labelVelocidade->setAlignment(Qt::AlignCenter);
    labelDirecao->setAlignment(Qt::AlignCenter);

    labelVelocidade->setStyleSheet(VALOR_STYLE);
    labelDirecao->setStyleSheet(VALOR_STYLE);

    auto *layout = new QVBoxLayout(this);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    layout->addWidget(titulo);
    layout->addStretch();
    layout->addWidget(labelVelocidade);
    layout->addWidget(labelDirecao);
    layout->addStretch();
}

void sensoranemometro::setVelocidade(double velocidade)
{
    labelVelocidade->setText(
        QString("%1 km/h")
            .arg(velocidade, 0, 'f', 1));
}

void sensoranemometro::setDirecao(int graus)
{
    labelDirecao->setText(
        QString("Direção: %1")
            .arg(converterDirecaoTexto(graus)));
}

QString sensoranemometro::converterDirecaoTexto(int graus)
{
    static const QString direcoes[] =
        {
            "N", "NE", "L", "SE",
            "S", "SO", "O", "NO"
        };

    const int indice = ((graus + 22) % 360) / 45;

    return direcoes[indice];
}