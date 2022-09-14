#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3; 
static const uint32_t GPSBaud = 9600;

SoftwareSerial mySerial(9, 10);// GSM 900A TX=pin 9, RX=pin 10 of Arduino


TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin); // Here we make pin 3 as RX of arduino & pin 4 as TX of arduino

void setup()
{
  mySerial.begin(9600); 
  Serial.begin(9600);
  ss.begin(GPSBaud);
  delay(100);
 
}

void loop()
{
  while (ss.available() > 0)
  if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
  
  if (Serial.available()>0)
  switch(Serial.read())
  {
    case 's':
      mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
     delay(1000);  // Delay of 1 second
     mySerial.println("AT+CMGS=\"+91\"\r"); // Replace x with mobile number
     delay(1000);
      mySerial.println("Location Identified");
     mySerial.println(gps.location.lat(), 6);// The SMS text you want to send
     mySerial.println(gps.location.lng(), 6);
     delay(100);
     mySerial.println((char)26);// ASCII code of CTRL+Z for saying the end of sms to  the module 
      delay(1000);
      break;
     
    case 'r':
      mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
      delay(1000);
      break;
  }

 if (mySerial.available()>0)
   Serial.write(mySerial.read());

}

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }
 
  Serial.println();
}
