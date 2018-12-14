/*
  NAME:
  Basic usage of gbjBH1750 library for ambient light measurement at typical
  measurement accuracy.

  DESCRIPTION:
  The sketch measures ambient light intensity with one determined BH1750 sensor.
  - Connect sensor's pins to microcontroller's I2C bus or I2C default pins
    as described in README.md for used platform accordingly.
  - Leave ADDR pin floating or connected to GND in order to use default address.
  - Change measurement mode for various measurement sensitivity.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#define SKETCH "GBJ_BH1750_MEASUREMENT_TYPICAL 1.0.0"

#include "gbj_bh1750.h"

const unsigned int PERIOD_MEASURE = 3000;  // Time in miliseconds between measurements

gbj_bh1750 Sensor = gbj_bh1750();
// gbj_bh1750 Sensor = gbj_bh1750(gbj_bh1750::CLOCK_100KHZ, true, D2, D1);
// gbj_bh1750 Sensor = gbj_bh1750(gbj_bh1750::CLOCK_400KHZ);


void errorHandler(String location)
{
  if (Sensor.isSuccess()) return;
  Serial.print(location);
  Serial.print(" - Error: ");
  Serial.print(Sensor.getLastResult());
  Serial.print(" - ");
  switch (Sensor.getLastResult())
  {
    // General
    case gbj_bh1750::ERROR_ADDRESS:
      Serial.println("ERROR_ADDRESS");
      break;

    case gbj_bh1750::ERROR_PINS:
      Serial.println("ERROR_PINS");
      break;

    // Arduino, Esspressif specific
#if defined(__AVR__) || defined(ESP8266) || defined(ESP32)
    case gbj_bh1750::ERROR_BUFFER:
      Serial.println("ERROR_BUFFER");
      break;

    case gbj_bh1750::ERROR_NACK_DATA:
      Serial.println("ERROR_NACK_DATA");
      break;

    case gbj_bh1750::ERROR_NACK_OTHER:
      Serial.println("ERROR_NACK_OTHER");
      break;

    // Particle specific
#elif defined(PARTICLE)
    case gbj_bh1750::ERROR_BUSY:
      Serial.println("ERROR_BUSY");
      break;

    case gbj_bh1750::ERROR_END:
      Serial.println("ERROR_END");
      break;

    case gbj_bh1750::ERROR_TRANSFER:
      Serial.println("ERROR_TRANSFER");
      break;

    case gbj_bh1750::ERROR_TIMEOUT:
      Serial.println("ERROR_TIMEOUT");
      break;
#endif

    default:
      Serial.println("Uknown error");
      break;
  }
}


String getModeName()
{
  String measurementType;
  switch (Sensor.getMode())
  {
    case gbj_bh1750::MODE_CONTINUOUS_HIGH:
      measurementType = "Continuous High";
      break;
    case gbj_bh1750::MODE_CONTINUOUS_HIGH2:
      measurementType = "Continuous High Double";
      break;
    case gbj_bh1750::MODE_CONTINUOUS_LOW:
      measurementType = "Continuous Low";
      break;
    case gbj_bh1750::MODE_ONETIME_HIGH:
      measurementType = "Onetime High";
      break;
    case gbj_bh1750::MODE_ONETIME_HIGH2:
      measurementType = "Onetime High Double";
      break;
    case gbj_bh1750::MODE_ONETIME_LOW:
      measurementType = "Onetime Low";
      break;
  }
  return measurementType;
}


void setup()
{
  Serial.begin(9600);
  Serial.println(SKETCH);
  Serial.println("Libraries:");
  Serial.println(gbj_twowire::VERSION);
  Serial.println(gbj_bh1750::VERSION);
  Serial.println("---");

  if (Sensor.begin(gbj_bh1750::ADDRESS_FLOAT, gbj_bh1750::MODE_CONTINUOUS_HIGH))
  {
    errorHandler("Begin");
    return;
  }
  Serial.println("Accuracy: " + String(Sensor.getAccuracyTyp()) + " bitCount/lux");
  Serial.println("Mode: " + getModeName());
  Serial.println("Sensitivity: " +String(Sensor.getSensitivityTyp()) + " lux/bitCount");
  Serial.println("Resolution: " +String(Sensor.getResolutionTyp()) + " bitCount/lux");
  Serial.println("Measurement time: " + String(Sensor.getMeasurementTime()) + " ms");
  Serial.println("---");
  Serial.println("Light in lux");
}


void loop()
{
  if (Sensor.isError()) return;
  Serial.println(Sensor.measureLightTyp());
  delay(PERIOD_MEASURE);
}
