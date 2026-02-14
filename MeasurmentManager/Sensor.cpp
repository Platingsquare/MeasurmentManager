#include "Sensor.h"
#include "TemperatureSensor.h"
#include "HumiditySensor.h"
#include "MeasurementStorage.h"
#include "FileStorage.h"
#include "Measurement.h"
#include "Timestamp.h"
#include <iostream>
#include <vector>

void simulateSensors(MeasurementStorage& storage, FileStorage& file)
{
    // Example default configs; in practice load from CSV via SensorConfig::loadFromCsv
    SensorConfig cfg1{ "TempSensor 1", "°C", -10.0, 40.0, 30.0 };
    SensorConfig cfg2{ "Humidity 1", "%", 0.0, 100.0, 80.0 };

    TemperatureSensor tempSensor(cfg1);
    HumiditySensor humSensor(cfg2);

    double tval = tempSensor.read();
    double hval = humSensor.read();

   // ....
}