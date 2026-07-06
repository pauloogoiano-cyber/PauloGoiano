#include "compasswidget.h"
#include <QPainter>
#include <QtMath>

CompassWidget::CompassWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(300, 300);
}

void CompassWidget::setDirecao(int graus)
{
    direcaoAtual = (graus % 360 + 360) % 360;
    update();
}

QString CompassWidget::grausParaCardinal(int graus) const
{
    const QStringList dir = { "NORTE", "NORDESTE", "LESTE", "SUDESTE", "SUL", "SUDOESTE", "OESTE", "NOROESTE" };
    int idx = (graus + 22) / 45 % 8;
    return dir[idx];
}

void CompassWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRect r = rect();
    QPoint c = r.center();
    int radius = qMin(r.width(), r.height()) / 2 - 25;

    // CÍRCULO DA BÚSSOLA
    p.setPen(QPen(QColor(51, 65, 85), 2));
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(c, radius, radius);

    const QStringList pontos = {"N", "NE", "L", "SE", "S", "SO", "O", "NO"};
    QFont font = p.font();
    font.setPointSize(12);
    font.setBold(true);
    p.setFont(font);
    p.setPen(QColor(51, 65, 85));

    for (int i = 0; i < 8; ++i) {
        double anguloGraus = (i * 45) - 90;
        double anguloRad = qDegreesToRadians(anguloGraus);

        int distanciaTexto = radius - 18;
        int x = c.x() + distanciaTexto * qCos(anguloRad);
        int y = c.y() + distanciaTexto * qSin(anguloRad);

        QRect textRect(x - 20, y - 12, 40, 24);
        p.drawText(textRect, Qt::AlignCenter, pontos[i]);
    }

    p.save();
    p.translate(c);
    p.rotate(direcaoAtual);

    QPolygon arrow;
    arrow << QPoint(0, -radius + 25)
          << QPoint(-8, 10)
          << QPoint(8, 10);

    p.setBrush(QColor(45, 108, 223));
    p.setPen(Qt::NoPen);
    p.drawPolygon(arrow);

    p.restore();
}