


#include <Arduino.h>
#include <HardwareSerial.h>
#include <math.h>

#define NPK_DEBUG

unsigned long resptime = millis();

#define sensorWaitingTime 1000
#define sensorID 0x01
#define sensorFunction 0x03
#define sensorByteResponse 0x0E

#define TEMP_MODE 1
#define PH_MODE 2
#define EC_MODE 3
#define LENGAS_MODE 4
#define NPK_MODE 5
#define XY_MD02_MODE 6

// HardwareSerial npkSensor(2); // use UART2

// unsigned char byteRequest[8] = {0X01, 0X03, 0X00, 0X00, 0X00, 0X07, 0X04, 0X08};
unsigned char byteRequest[8] = {0x01, 0x03, 0x00, 0x12, 0x00, 0x02, 0x64, 0x0E};
unsigned char byteResponse[19] = {};
uint8_t sensorFrameSize = 8;

uint8_t sensorCount = 0;
uint8_t serMode = TEMP_MODE;
uint8_t serCount = 0;

String co2Buff = "";
uint8_t co2Count = 0;



float temperature =0;
float  ph = 0;
float airTemp = 0;
int nitrogen = 0;
int phosphorus = 0;
int potassium = 0;
int ec = 0;
int lengas = 0;
int airHum = 0;

int co2 = 0;
int airTemp1 = 0;
float airTemp2 = 0;
float airHum2 = 0;

int sensorValue(int x, int y)
{
    int t = 0;
    t = x * 256;
    t = t + y;

    return t;
}

String getSensor(){
    //format npk_sensor,temp,lengas,ec,ph,N,P,K,airTemp,airHum,co2,airtemp1,airTemp2,airHum2,-,;\n
    //          0        1    2     3  4  5 6 7  8       9     10    11      12       13
    String out="npk_sensor,";

    out += String(temperature,1);
    out += ",";
    out += String(lengas);
    out += ",";
    out += String(ec);
    out += ",";
    out += String(ph,1);
    out += ",";
    out += String(nitrogen);
    out += ",";
    out += String(phosphorus);
    out += ",";
    out += String(potassium);
    out += ",";
    out += String(airTemp,1);
    out += ",";
    out += String(airHum);
    out += ",";
    out += String(co2);
    out += ",";
    out += String(airTemp1);
    out += ",";
    out += String(airTemp2);
    out += ",";
    out += String(airHum2);
    out += ",-,;\n"; 
     
    return out;
}

void sensor_init()
{
    Serial2.begin(9600, SERIAL_8N1, 16, 17);
    Serial1.begin(9600, SERIAL_8N1, 9, 10);
}

void waitDelay()
{
    resptime = millis();
    while ((Serial2.available() < sensorFrameSize) && ((millis() - resptime) < sensorWaitingTime))
    {
        delay(1);
    }
}

String infoTemp(){
    return String(temperature,1);
}
String infoLengas(){
    return String(lengas);
}

String infoEC(){
    return String(ec);

}
String infoN(){
    return String(nitrogen);
}
String infoP(){
    return String(phosphorus);

}
String infoK(){
    return String(potassium);
}

String infoPH(){
    return String(ph);
}
String infoAirTemp(){
    return String(airTemp2,1);
}
String infoAirHum(){
    return String(airHum2);
}

void printResponse()
{
    
    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print("°C");
    Serial.print("\t");

    Serial.print("Lengas: ");
    Serial.print(lengas);
    Serial.print("%");
    Serial.print("\t");

    Serial.print("EC: ");
    Serial.print(ec);
    Serial.print("uS/m");
    Serial.print("\t");

    Serial.print("pH: ");
    Serial.println(ph);
    // Serial.print("");
    // Serial.print("\n");

    Serial.print("N: ");
    Serial.print(nitrogen);
    Serial.print("mg/kg");
    Serial.print("\t");

    Serial.print("P: ");
    Serial.print(phosphorus);
    Serial.print("mg/kg");
    Serial.print("\t");

    Serial.print("K: ");
    Serial.print(potassium);
    Serial.println("mg/kg");

    Serial.print("Suhu Udara: ");
    Serial.print(airTemp);
    Serial.print("°C");
    Serial.print("\t");

    Serial.print("Humidity: ");
    Serial.print(airHum);
    Serial.println("%");

    // Serial.print("\t");
    Serial.println();
}
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


void cekSer1(String msg){
    //format pesan co2_module,co2,temp1,hum,temp2,-,;\n
    if(msg.indexOf("co2_module") >= 0){
        co2 = getSplitValue(msg,',',1).toInt();
        airTemp1 = getSplitValue(msg,',',2).toInt();
        airTemp2 = getSplitValue(msg,',',3).toFloat();
        airHum2 = getSplitValue(msg,',',4).toFloat();        
    }
}

void cekResponse(uint8_t mode)
{

    if (mode == TEMP_MODE)
    {
        temperature = sensorValue(byteResponse[5], byteResponse[6]) * 0.1;
#ifdef NPK_DEBUG
        Serial.print("Byte Temperature Response: ");

        String responseString;
        for (int j = 0; j < sensorFrameSize; j++)
        {
            responseString += byteResponse[j] < 0x10 ? " 0" : " ";
            responseString += String(byteResponse[j], HEX);
            responseString.toUpperCase();
        }
        Serial.println(responseString);

#endif
    }
    else if (mode == LENGAS_MODE)
    {
        lengas = round(sensorValue(byteResponse[3], byteResponse[4]) / 3.95);
        if (lengas > 100)
        {
            lengas = 100;
        }
#ifdef NPK_DEBUG
        Serial.print("Byte Lengas Response: ");

        String responseString;
        for (int j = 0; j < sensorFrameSize; j++)
        {
            responseString += byteResponse[j] < 0x10 ? " 0" : " ";
            responseString += String(byteResponse[j], HEX);
            responseString.toUpperCase();
        }
        Serial.println(responseString);

#endif
    }
    else if (mode == EC_MODE)
    {
        ec = sensorValue(byteResponse[3], byteResponse[4]);
#ifdef NPK_DEBUG
        Serial.print("Byte EC Response: ");

        String responseString;
        for (int j = 0; j < sensorFrameSize; j++)
        {
            responseString += byteResponse[j] < 0x10 ? " 0" : " ";
            responseString += String(byteResponse[j], HEX);
            responseString.toUpperCase();
        }
        Serial.println(responseString);

#endif
    }
    else if (mode == PH_MODE)
    {
        ph = sensorValue(byteResponse[3], byteResponse[4]) * 0.01;
#ifdef NPK_DEBUG
        Serial.print("Byte PH Response: ");

        String responseString;
        for (int j = 0; j < sensorFrameSize; j++)
        {
            responseString += byteResponse[j] < 0x10 ? " 0" : " ";
            responseString += String(byteResponse[j], HEX);
            responseString.toUpperCase();
        }
        Serial.println(responseString);

#endif
    }
    else if (mode == NPK_MODE)
    {
        nitrogen = sensorValue(byteResponse[3], byteResponse[4]);
        phosphorus = sensorValue(byteResponse[5], byteResponse[6]);
        potassium = sensorValue(byteResponse[7], byteResponse[8]);

#ifdef NPK_DEBUG
        Serial.print("Byte npk Response: ");

        String responseString;
        for (int j = 0; j < sensorFrameSize; j++)
        {
            responseString += byteResponse[j] < 0x10 ? " 0" : " ";
            responseString += String(byteResponse[j], HEX);
            responseString.toUpperCase();
        }
        Serial.println(responseString);

#endif
    }
    else if (mode == XY_MD02_MODE)
    {
        airTemp = sensorValue(byteResponse[3], byteResponse[4]) * 0.1;
        airHum = round(sensorValue(byteResponse[5], byteResponse[6])* 0.1);
        //potassium = sensorValue(byteResponse[7], byteResponse[8]);

#ifdef NPK_DEBUG
        Serial.print("Byte XY-MD02 Response: ");

        String responseString;
        for (int j = 0; j < sensorFrameSize; j++)
        {
            responseString += byteResponse[j] < 0x10 ? " 0" : " ";
            responseString += String(byteResponse[j], HEX);
            responseString.toUpperCase();
        }
        Serial.println(responseString);

#endif
    }
    printResponse();

}

void waitResponse()
{
    if(Serial1.available()){
        char c= Serial1.read();
        if(c == '\n'){
            co2Buff += c;
            cekSer1(co2Buff);
            co2Buff ="";

        }else{
            co2Buff += c;
        }
    }
    if (Serial2.available())
    {
        byteResponse[serCount] = Serial2.read();
        serCount++;
        if (serCount == (sensorFrameSize - 1))
        {
            cekResponse(serMode);
            serCount = 0;
        }
    }
}

void getTempHum()
{
    // for(int i = 0;i < 8;i++){
    //    npkSensor.write(byteRequest[i]);
    // delay(10);
    // }
    Serial2.write(byteRequest, 8);
    sensorFrameSize = 9;
    serMode = TEMP_MODE;
    serCount = 0;

    Serial.println("Baca Temp & Lengas Tanah");
    /*
     waitDelay();

    while (Serial2.available())
    {
        for (int n = 0; n < sensorFrameSize; n++)
        {
            byteResponse[n] = Serial2.read();
        }

        //if (byteResponse[0] != sensorID && byteResponse[1] != sensorFunction)
       // {
       //     return;
       // }
    }

    Serial.println();
    Serial.println("----- Humidity & Temp ------");
    Serial.print("Byte Response: ");

    String responseString;
    for (int j = 0; j < sensorFrameSize; j++)
    {
        responseString += byteResponse[j] < 0x10 ? " 0" : " ";
        responseString += String(byteResponse[j], HEX);
        responseString.toUpperCase();
    }
    Serial.println(responseString);

    Serial.print("Kelembaban: ");
    Serial.println(sensorValue(byteResponse[3], byteResponse[4]) * 0.1);
    temperature = sensorValue(byteResponse[5], byteResponse[6]) * 0.1;
    Serial.print("Suhu: ");
    Serial.println(temperature);
    */
}

void getLengas()
{
    unsigned char lengasRequest[8] = {0x01, 0x03, 0x00, 0x12, 0x00, 0x01, 0x24, 0x0F};

    Serial2.write(lengasRequest, 8);
    sensorFrameSize = 7;
    serMode = LENGAS_MODE;
    serCount = 0;

    Serial.println("Baca Lengas");
    /*
     waitDelay();

    while (Serial2.available())
    {
        for (int n = 0; n < sensorFrameSize; n++)
        {
            byteResponse[n] = Serial2.read();
        }

        //if (byteResponse[0] != sensorID && byteResponse[1] != sensorFunction)
       // {
       //     return;
        //}
    }

    Serial.println();
    Serial.println("----- Lengas ------");
    Serial.print("Byte Response: ");

    String responseString;
    for (int j = 0; j < sensorFrameSize; j++)
    {
        responseString += byteResponse[j] < 0x10 ? " 0" : " ";
        responseString += String(byteResponse[j], HEX);
        responseString.toUpperCase();
    }
    Serial.println(responseString);

    lengas = sensorValue(byteResponse[3], byteResponse[4]) * 0.1;
    Serial.print("Lengas: ");
    Serial.println(lengas);
    */
}

void getEC()
{
    unsigned char ECRequest[8] = {0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE};
    Serial2.write(ECRequest, 8);
    sensorFrameSize = 7;
    serMode = EC_MODE;
    serCount = 0;

    Serial.println("Baca EC");
    /*
     waitDelay();

    while (Serial2.available())
    {
        for (int n = 0; n < sensorFrameSize; n++)
        {
            byteResponse[n] = Serial2.read();
        }

        if (byteResponse[0] != sensorID && byteResponse[1] != sensorFunction)
        {
            return;
        }
    }

    Serial.println();
    Serial.println("----- EC ------");
    Serial.print("Byte Response: ");

    String responseString;
    for (int j = 0; j < sensorFrameSize; j++)
    {
        responseString += byteResponse[j] < 0x10 ? " 0" : " ";
        responseString += String(byteResponse[j], HEX);
        responseString.toUpperCase();
    }
    Serial.println(responseString);
    ec = sensorValue(byteResponse[3], byteResponse[4]);
    Serial.print("EC: ");
    Serial.println(ec);
    */
}

void getPH()
{
    unsigned char PHRequest[8] = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};
    Serial2.write(PHRequest, 8);
    sensorFrameSize = 7;
    serMode = PH_MODE;
    serCount = 0;

    Serial.println("Baca PH");
    /*
     waitDelay();

    while (Serial2.available())
    {
        for (int n = 0; n < sensorFrameSize; n++)
        {
            byteResponse[n] = Serial2.read();
        }

        if (byteResponse[0] != sensorID && byteResponse[1] != sensorFunction)
        {
            return;
        }
    }

    Serial.println();
    Serial.println("----- PH ------");
    Serial.print("Byte Response: ");

    String responseString;
    for (int j = 0; j < sensorFrameSize; j++)
    {
        responseString += byteResponse[j] < 0x10 ? " 0" : " ";
        responseString += String(byteResponse[j], HEX);
        responseString.toUpperCase();
    }
    Serial.println(responseString);
    ph = sensorValue(byteResponse[3], byteResponse[4]) * 0.01;
    Serial.print("PH: ");
    Serial.println(ph);
    */
}

void getNPK()
{
    unsigned char NPKRequest[8] = {0x01, 0x03, 0x00, 0x1E, 0x00, 0x03, 0x65, 0xCD};
    Serial2.write(NPKRequest, 8);
    sensorFrameSize = 11;
    serMode = NPK_MODE;
    serCount = 0;

    Serial.println("Baca NPK");
    /*
     waitDelay();

    while (Serial2.available())
    {
        for (int n = 0; n < sensorFrameSize; n++)
        {
            byteResponse[n] = Serial2.read();
        }

        if (byteResponse[0] != sensorID && byteResponse[1] != sensorFunction)
        {
            return;
        }
    }

    Serial.println();
    Serial.println("----- NPK ------");
    Serial.print("Byte Response: ");

    String responseString;
    for (int j = 0; j < sensorFrameSize; j++)
    {
        responseString += byteResponse[j] < 0x10 ? " 0" : " ";
        responseString += String(byteResponse[j], HEX);
        responseString.toUpperCase();
    }
    Serial.println(responseString);

    nitrogen = sensorValue(byteResponse[3], byteResponse[4]);
    phosphorus = sensorValue(byteResponse[5], byteResponse[6]);
    potassium = sensorValue(byteResponse[7], byteResponse[8]);

    Serial.print("N: ");
    Serial.println(nitrogen);
    Serial.print("P: ");
    Serial.println(phosphorus);
    Serial.print("K: ");
    Serial.println(potassium);
    */
}


void getTemp_udara(){
    //XY-MD02
    unsigned char XY_MD02Request[8] = {0x01, 0x04, 0x00, 0x01, 0x00, 0x02, 0x20, 0x0B};
    Serial2.write(XY_MD02Request, 8);
    sensorFrameSize = 9;
    serMode = XY_MD02_MODE;
    serCount = 0;

    Serial.println("Baca Temp & Humidity udara");

}

void getHum_CO2(){
    Serial1.println("getHum_CO2");
}
void sensor_loop()
{
    static unsigned long sensorDelay = millis();

    waitResponse();
    if ((millis() - sensorDelay) > 1000)
    {
        sensorDelay = millis();
        
        getHum_CO2();
        if (++sensorCount > 5)
        {
            sensorCount = 0;
            
        }

        switch (sensorCount)
        {
        case 0:

            getTempHum();
            break;
        case 1:
            getLengas();
            break;

        case 2:
            getEC();
            break;
        case 3:
            getPH();
            break;

        case 4:
            getNPK();
            break;
        case 5:
            getTemp_udara();
        break;
        }
    }
}
