#ifndef SENSORANEMOMETRO_H
#define SENSORANEMOMETRO_H

#include <QWidget>
#include <QString>

class QLabel;

class sensoranemometro : public QWidget
{
    Q_OBJECT

public:
    explicit sensoranemometro(QWidget *parent = nullptr);

    void setVelocidade(double velocidade);
    void setDirecao(int graus);

    static QString converterDirecaoTexto(int graus);

private:
    QLabel *labelVelocidade{nullptr};
    QLabel *labelDirecao{nullptr};
};

#endif // SENSORANEMOMETRO_H