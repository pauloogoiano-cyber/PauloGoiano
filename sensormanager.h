#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include <QObject>
#include <QVector>
#include <QString>

class QWidget;

class SensorManager : public QObject
{
    Q_OBJECT

public:
    explicit SensorManager(QObject *parent = nullptr);
    ~SensorManager() override = default;

    void adicionarSensor(const QString &tipo);
    void removerSensor(int index);

    const QVector<QWidget*>& sensores() const;

signals:
    void sensoresAtualizados();

private:
    QVector<QWidget*> listaSensores;
};

#endif // SENSORMANAGER_H