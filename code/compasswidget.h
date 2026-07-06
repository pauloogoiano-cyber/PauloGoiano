#ifndef COMPASSWIDGET_H
#define COMPASSWIDGET_H

#include <QWidget>

class CompassWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompassWidget(QWidget *parent = nullptr);
    void setDirecao(int graus);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int direcaoAtual = 0;
    QString grausParaCardinal(int graus) const;
};

#endif