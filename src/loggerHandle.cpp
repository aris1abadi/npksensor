#include <Arduino.h>
#include "FS.h"
#include "SPIFFS.h"
#include <bluethootHandle.h>
#include <sensorHandle.h>

//#define TES_LOGGER

#define FORMAT_SPIFFS_IF_FAILED true

bool loggerOn = false;
int loggerInterval = 60; // default 1 menit
int loggerCount = 0;     // count logger
int loggerJumlah = 5;
int loggerJumlahCount = 0;
String loggerName = "log.txt";
String loggerVal = "";

#ifdef TES_LOGGER
// format npk_sensor,temp,lengas,ec,ph,N,P,K,airTemp,airHum,co2,airtemp1,airTemp2,airHum2,-,;\n
//           0        1    2     3  4  5 6 7  8       9     10    11      12       13
String getSplitValue(String data, char separator, int index);

String getSensor();
void kirimBT(String msg);

#endif

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir(fs, file.path(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

String readFile(fs::FS &fs, const char *path)
{
  String msg_out = "";
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory())
  {
    Serial.println("- failed to open file for reading");
    msg_out = "#File tidak bisa dibaca/tidak ada\n";
    return msg_out;
  }

  Serial.println("- read from file:");
  while (file.available())
  {
    char c = file.read();
    msg_out += c;
    Serial.write(c);
  }
  file.close();
  msg_out += "\n";
  return msg_out;
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    Serial.println("- file written");
  }
  else
  {
    Serial.println("- write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Appending to file: %s\r\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    Serial.println("- failed to open file for appending");
    return;
  }
  if (file.print(message))
  {
    Serial.println("- message appended");
  }
  else
  {
    Serial.println("- append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char *path1, const char *path2)
{
  Serial.printf("Renaming file %s to %s\r\n", path1, path2);
  if (fs.rename(path1, path2))
  {
    Serial.println("- file renamed");
  }
  else
  {
    Serial.println("- rename failed");
  }
}

void deleteFile(fs::FS &fs, const char *path)
{
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path))
  {
    Serial.println("- file deleted");
  }
  else
  {
    Serial.println("- delete failed");
  }
}

bool getLoggerSts()
{
  return loggerOn;
}

void setLoggerSts(bool sts)
{
  loggerOn = sts;
}

int getInterval()
{
  return loggerInterval;
}

void setInterval(int val)
{
  loggerInterval = val;
}

int getJumlah()
{
  return loggerJumlah;
}

void setJumlah(int val)
{
  loggerJumlah = val;
}

String getLogName()
{
  return loggerName;
}

void setLogName(String name)
{

  loggerName = name;
}
void simpanLogger(String name, String msg, int count)
{

  if (count == 0)
  {
    // delete last logger
    // writeFile(SPIFFS,"lastLogger.txt",name.c_str());
    String info = "#Mulai:";
    info += name;
    info += "\nInterval:";
    info += String(loggerInterval);
    info += " detik\nJumlah:";
    info += String(loggerJumlah);
    info += "x\n\n";
    info += String(count + 1);
    info += ".";
    info += msg;
    info += "\n";

    writeFile(SPIFFS, "/myLogger.txt", info.c_str());
  }
  else
  {
    String data = String(count + 1);
    data += ".";
    data += msg;
    data += "\n";
    appendFile(SPIFFS, "/myLogger.txt", data.c_str());
  }
}

String getDataLog()
{

  return readFile(SPIFFS, "/myLogger.txt");
}

void hapusLog()
{
}

void loggerStop()
{
  kirimBT("logger_resp,stop,ok,-,\n");
}

void logger_init()
{
  // Initialize SPIFFS
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
  {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
}

void logger_loop()
{
  static unsigned long logMillis = millis();

  if ((millis() - logMillis) > 1000)
  {
    // cek tiap detik
    logMillis = millis();
    if (loggerOn)
    {
      if (++loggerCount > loggerInterval)
      {
        loggerCount = 0;

        // simpan log disini
        simpanLogger(loggerName, getSensor(), loggerJumlahCount);

        if (++loggerJumlahCount > loggerJumlah - 1)
        {
          loggerOn = false; // stop logger
          loggerJumlahCount = 0;
          loggerStop();
          Serial.println("Logger Selesai");
        }
      }
    }
  }
}