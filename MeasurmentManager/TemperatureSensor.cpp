#include "TemperatureSensor.h"
#include <random>

TemperatureSensor::TemperatureSensor(const SensorConfig & cfg)
    : Sensor(cfg),
    rng_(std::random_device{}()),
    dist_(cfg.minValue, cfg.maxValue)
{
    // --
}

double TemperatureSensor::read()
{
    return dist_(rng_);
}