#ifndef LOGGERHANDLE_H
#define LOGGERHANDLE_H

void logger_init();
void logger_loop();
bool getLoggerSts();
void setLoggerSts(bool sts);
String getLogName();
void setLogName(String name);
int getJumlah();
void setJumlah(int val);
void setInterval(int val);
int getInterval();
void simpanLogger(String name,String msg,int count);
String getDataLog();
#endif