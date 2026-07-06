#include "sensorpluviometro.h"

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

sensorpluviometro::sensorpluviometro(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(CARD_STYLE);

    setMinimumSize(420, 240);

    setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding);

    auto *titulo = new QLabel("PLUVIÔMETRO", this);
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet(TITULO_STYLE);

    labelChuva = new QLabel("-- mm", this);

    labelChuva->setAlignment(Qt::AlignCenter);
    labelChuva->setStyleSheet(VALOR_STYLE);

    auto *layout = new QVBoxLayout(this);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    layout->addWidget(titulo);
    layout->addStretch();
    layout->addWidget(labelChuva);
    layout->addStretch();
}

void sensorpluviometro::setChuva(double chuva)
{
    labelChuva->setText(
        QString("%1 mm")
            .arg(chuva, 0, 'f', 1));
}