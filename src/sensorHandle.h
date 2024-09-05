#ifndef SENSORHANDLE_H
#define SENSORHANDLE_H

void sensor_init();
void sensor_loop();
String infoTemp();
String infoLengas();
String infoEC();
String infoN();
String infoP();
String infoK();
String infoPH();
String infoAirTemp();
String infoAirHum();
String getSensor();
String getSplitValue(String data, char separator, int index);
#endif