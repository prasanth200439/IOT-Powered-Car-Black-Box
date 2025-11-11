#include <SoftwareSerial.h>
#include <Wire.h>
#include <MPU6050.h>
#include <TinyGPS++.h>
#include <SD.h>
//gps and gsm
SoftwareSerial gpsSerial(4,3);
SoftwareSerial gsmSerial(8,7);
TinyGPSPlus gps;
//accelerometer
MPU6050 mpu;
//sd card
const int chipSelect = 10;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  gsmSerial.begin(9600);
  Wire.begin();
  mpu.initialize();
  //SD.begin(chipSelect);

  if(!mpu.testConnection()) Serial.println("MPU6050 not connected!");
  if(!SD.begin(chipSelect)) Serial.println("SD card failed");

  // put your setup code here, to run once:

}

void loop() {
  while (gpsSerial.available()){
    gps.encode(gpsSerial.read());
  
  // put your main code here, to run repeatedly:

}
   // <-- check GPS valid
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);
if (abs(ax) > 13000 || abs(ay) > 13000 || abs(az) > 13000){
  float lat = gps.location.lat();
  float lng = gps.location.lng();
//sd card save
  File dataFile = SD.open("log.txt", FILE_WRITE);
  if (dataFile){
    dataFile.print("Crash at: ");
    dataFile.print(lat, 6);
    dataFile.print(",");
    dataFile.println(lng, 6);
    dataFile.print("Date: ");
    dataFile.print(gps.date.day());
    dataFile.print("/");
    dataFile.print(gps.date.month());
    dataFile.print("/");
    dataFile.print(gps.date.year());
    dataFile.print(" Time: ");
    dataFile.print(gps.time.hour());
    dataFile.print(":");
    dataFile.print(gps.time.minute());
    dataFile.print(":");
    dataFile.print(gps.time.second());
    dataFile.close();
  }
// send sms
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  gsmSerial.println("AT+CMGS=\"+919360764839\"");
  delay(1000);
  gsmSerial.print("Accident detected at https://maps.google.com/?q=");
  gsmSerial.print(lat, 6);
  gsmSerial.print(",");
  gsmSerial.print(lng, 6);
  gsmSerial.write(26);

} 

  
  delay(1000);
  
  
  }