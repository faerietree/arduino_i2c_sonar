#include <Wire.h>
#include <APM_I2CSonar.h>       // I2CSonar Library

APM_I2CSonar_Class APM_I2CSonar;

unsigned long timer;

void setup()
{	
  Serial.begin(115200);
  Serial.println("I2CSonar library test");
  timer = millis();

  APM_I2CSonar.Init();	 // I2C Sonar initialization
}

void loop()
{
  if((millis()- timer) > 100){
    timer = millis();
    
    APM_I2CSonar.Read();

    Serial.println(APM_I2CSonar.RawSonar);

//  Serial.print(APM_I2CSonar.RawSonar, BYTE);

//  Serial.print( 0xff, BYTE);
//  Serial.print( (APM_I2CSonar.RawSonar >> 8) & 0xff, BYTE);
//  Serial.print( APM_I2CSonar.RawSonar & 0xff, BYTE);

    }
}

