#include <Arduino.h>
#include <oledHandle.h>
#include <sensorHandle.h>
#include <bluethootHandle.h>
#include <otaHandle.h>
#include <loggerHandle.h>

#define PIN_POWER 4
long powerMillis ;
int powerDelay = 10000;
bool powerSts = false;

void setup()
{
  Serial.begin(115200);
  oled_init();
  pinMode(PIN_POWER,OUTPUT);
    
  sensor_init();
  Serial.println();
  Serial.println();
  Serial.println("Sensor NPK & CO2 Handle");  
  bt_init();
  ota_init();
  logger_init();

}

void loop()
{
  
  sensor_loop();
  oled_loop();
  bt_loop();
  ota_loop();
  logger_loop();
  
}
