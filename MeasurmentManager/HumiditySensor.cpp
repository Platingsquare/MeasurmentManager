#include "HumiditySensor.h"
#include <random>

HumiditySensor::HumiditySensor(const SensorConfig& cfg)
    : Sensor(cfg),
    rng_(std::random_device{}()),
    dist_(cfg.minValue, cfg.maxValue)
{
}

double HumiditySensor::read()
{
    return dist_(rng_);
}
