/* Bibliteca alterada para poder suportar vários sensores do mesmo tipo (possuem o mesmo ID)
  Para isso foi inserido e modificado a LIB SoftWire para permitir que se criem barramentos i2c por software para cada sensor
  Modificado por Julio Cesar Goldner Vendramini
  Bibliotecas modificadas: 
  - https://github.com/stevemarple/SoftWire/
  - https://github.com/pololu/vl53l0x-arduino
*/

/* This example shows how to get single-shot range
 measurements from the VL53L0X. The sensor can optionally be
 configured with different ranging profiles, as described in
 the VL53L0X API user manual, to get better performance for
 a certain application. This code is based on the four
 "SingleRanging" examples in the VL53L0X API.

 The range readings are in units of mm. */

#include <Wire.h>
#include "SoftWire.h"
#include "VL53L0X.h"


//agora o sensor deve ser iniciado passando os pinos SDA e SCL para funcionar o resto casicamente continua igual
VL53L0X sensor(5,6); //SDA,SCL
//se quiser por exemplo, utilizar mais um sensor é só declarar outro em outras portas:
//VL53L0X sensor2(7,8); //SDA,SCL

// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.
//#define LONG_RANGE

// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED
//#define HIGH_ACCURACY


void setup()
{

  Serial.begin(9600);
  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  sensor.setMeasurementTimingBudget(200000);
#endif
}

void loop()
{
  Serial.print(sensor.readRangeSingleMillimeters());
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.println();
}
