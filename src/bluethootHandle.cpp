#include <Arduino.h>
#include "BluetoothSerial.h"
#include <loggerHandle.h>

//#define TES_LOGGER

//String MAC_Address;

 #include <sensorHandle.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial btSensor;
String bluethootBuff = "";

#ifdef TES_LOGGER
// format npk_sensor,temp,lengas,ec,ph,N,P,K,airTemp,airHum,co2,airtemp1,airTemp2,airHum2,-,;\n
//           0        1    2     3  4  5 6 7  8       9     10    11      12       13
String getSplitValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String getSensor()
{
    String data = "npk_sensor,29.0,95,4343,7.2,234,317,741,0.0,0,931,28,31.30,77.10,-,;";
    return data;
}

#endif

void kirimBT(String msg){
    if (btSensor.connected())
        {
            btSensor.print(msg);
        }
}

void cekBtMsg(String msg)
{
    if (msg.indexOf("getSensor") >= 0)
    {
        // response
        kirimBT(getSensor());
        
    }
    else if (msg.indexOf("logger_start") >= 0)
    {
            
            kirimBT("logger_resp,run,ok,-,\n");
            setLoggerSts(true);            
            setLogName(getSplitValue(msg,',',1));
            setInterval((getSplitValue(msg,',',2).toInt() * 60));
            setJumlah(getSplitValue(msg,',',3).toInt());
            //simpanLogger(getLogName(),getSensor(),0);
       
        Serial.print(msg);
    }else if(msg.indexOf("logger_stop") >= 0){
        kirimBT("logger_resp,stop,ok,-,\n");
        setLoggerSts(false);
    }
    else if (msg.indexOf("logger_cek") >= 0)
    {
        if (getLoggerSts()){
            kirimBT("logger_resp,run,ok,-,\n");
        }else{
            kirimBT("logger_resp,stop,ok,-,\n");
        }
    }else if(msg.indexOf("logger_data") >= 0){
        String dt = "logger_resp,data,";
        dt += getDataLog();
        kirimBT(dt);
    }
}


void bt_init()
{
    
    btSensor.begin("NPK sensor"); // Bluetooth device name
    Serial.println("NPK sensor bluethooth init OK");
}

void bt_loop()
{
    if (btSensor.available())
    {
        char c = btSensor.read();
        if (c == '\n')
        {
            bluethootBuff += '\n';
            cekBtMsg(bluethootBuff);
            bluethootBuff = "";
        }
        else
        {
            bluethootBuff += c;
        }
    }
}