#include "sensormanager.h"

#include "sensoranemometro.h"
#include "sensorpluviometro.h"

SensorManager::SensorManager(QObject *parent)
    : QObject(parent)
{
}

void SensorManager::adicionarSensor(const QString &tipo)
{
    QWidget *sensor = nullptr;

    if (tipo == "anemometro")
        sensor = new sensoranemometro;

    else if (tipo == "pluviometro")
        sensor = new sensorpluviometro;

    if (!sensor)
        return;

    listaSensores.append(sensor);

    emit sensoresAtualizados();
}

void SensorManager::removerSensor(int index)
{
    if (index < 0 || index >= listaSensores.size())
        return;

    QWidget *sensor = listaSensores.takeAt(index);

    if (sensor)
        sensor->deleteLater();

    emit sensoresAtualizados();
}

const QVector<QWidget*>& SensorManager::sensores() const
{
    return listaSensores;
}