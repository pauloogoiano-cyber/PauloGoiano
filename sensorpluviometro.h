#ifndef SENSORPLUVIOMETRO_H
#define SENSORPLUVIOMETRO_H

#include <QWidget>

class QLabel;

class sensorpluviometro : public QWidget
{
    Q_OBJECT

public:
    explicit sensorpluviometro(QWidget *parent = nullptr);

    void setChuva(double chuva);

private:
    QLabel *labelChuva{nullptr};
};

#endif