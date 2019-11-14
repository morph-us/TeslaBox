/************************************************************* Library Used ***************************************************************************************/
#include <ESP8266WiFi.h>;
 
#include <WiFiClient.h>;
 
#include <ThingSpeak.h>;

#include <MQ2.h>
#include <Wire.h> 

/********************************************************** Pin Configuration ************************************************************************************/
int c2w_tesla_mq2_pin = A0;
const int c2w_tesla_buzzer = D1;                                    // buzzer connected to D1
int c2w_tesla_trig=D2;                                              // trig connected to D2
int c2w_tesla_echo=D3;                                              // echo connected to D3
int c2w_tesla_LDRpin = D0;                                          // LDR Pin Connected at D0 Pin

/*********************************************************** Variable used **************************************************************************************/
long c2w_tesla_duration;
long c2w_tesla_distance;
int c2w_tesla_val;

/********************************************************** Instance of MQ2 class *******************************************************************************/

MQ2 mq2(c2w_tesla_mq2_pin);

/********************************************************* wifi name and password for nodemcu ********************************************************************/ 

const char* c2w_tesla_ssid = "sanjay";                                         // Network SSID
 
const char* c2w_tesla_password = "00000000";                                   // Network Password
 
 
WiFiClient client;                                                   // Reference of WifiClient class for wifi connection 
 
unsigned long c2w_tesla_channelNumber =840301;                               // Thinkspeak Channel Number
 
const char * c2w_tesla_writeAPIKey = "EQQ7OAJA2N2RJGB7";                     // ThingSpeak Write API Key to write into the thinkspeak channel

/******************************************************** setup set the pin of all the sensor on board ************************************************************/

void setup()
 
{
 
Serial.begin(9600);                                                  // broud rate set to 9600

mq2.begin();

pinMode(c2w_tesla_buzzer, OUTPUT);                                   // set the buzzer pin as output
pinMode(c2w_tesla_trig,OUTPUT);                                      // set the trigger pin as output
pinMode(c2w_tesla_echo,INPUT);                                       // set the echo pin as input 
 
delay(10);
 
// Connect to WiFi network
 
WiFi.begin(c2w_tesla_ssid, c2w_tesla_password);                                          // connect to wifi

Serial.print("wifi connection establish");
  
ThingSpeak.begin(client);                                            // connecting to thingspeak cloud
 
}

 
 
/************************************************************** Loop Execute forever ************************************************************************/ 
void loop() {
 
  int c2w_tesla_ldr_val = digitalRead(c2w_tesla_LDRpin);
  float* values= mq2.read(true);                                     // print the mq2 sensor values in the Serial

  Serial.print("LPG : ");
  Serial.println(mq2.lpg);                                           // print the lpg value on serial monitor
  Serial.print("CO : ");
  Serial.println(mq2.co);                                            // print the co value on serial monitor
  Serial.print("Smoke: ");
  Serial.println(mq2.smoke);                                         // print the smoke value on serial monitor
  

  long c2w_tesla_dist = c2w_tesla_ultrasonic();                      // call the ultrasonic method to calculate the distance
  Serial.print("Distance : ");
  Serial.println(c2w_tesla_dist);                                    // print the ultrasonic distance on serial monitor
  Serial.print("ldr value : ");
  Serial.println(c2w_tesla_ldr_val);                                 // print the LDR sensor value on serial monitor
  
  if(c2w_tesla_ldr_val==1){                                          // check the condition of LDR sensor if high then buzzer will off
    
    digitalWrite(c2w_tesla_buzzer, HIGH);                            // off the buzzer
    Serial.println(" lid close");  
    
  }
  
  else  {
    
      digitalWrite(c2w_tesla_buzzer, LOW);                           // on the buzzer
      Serial.println("lid open");
          
  }
  
 ThingSpeak.setField(1,(float)mq2.smoke);                            // set the field of smoke into the field 1 of thingspeak
 ThingSpeak.setField(2,(float)mq2.co);                               // set the field of co into the field 2 of thingspeak
 ThingSpeak.setField(3,(float)c2w_tesla_ldr_val);                    // set the field of LDR value into the field 3 of thingspeak
 ThingSpeak.setField(4,(float)c2w_tesla_distance);                   // set the field of distance into the field 4 of thingspeak
  
ThingSpeak.writeFields(c2w_tesla_channelNumber,c2w_tesla_writeAPIKey);               // Update in ThingSpeak 
 
}
/******************************************************** Function to calculate the distance from ultrasonic sensor ************************************************/

long c2w_tesla_ultrasonic(){

  digitalWrite(c2w_tesla_trig,LOW);                                  // CLEAR THE TRIGERPIN 
  delay(10);
  digitalWrite(c2w_tesla_trig,HIGH);                                 // SET THE TRIGER PIN ON HIGH STATE FOR 10 MICRO SECONDS
  delay(10);
  digitalWrite(c2w_tesla_trig,LOW);                                  // SET THE TRIGER PIN ON LOW STATE 
  c2w_tesla_duration=pulseIn(c2w_tesla_echo,HIGH);                   // READ THE ECHOPIN , RETURNS THE SOUND WAVE TRAVEL TIME IN MICROSECOND
  c2w_tesla_distance=((c2w_tesla_duration*0.034)/2);                 // DISTACE OF 1 WAY PATH IS  IS CALCULATED 
  return c2w_tesla_distance;                                         // RETURN THE DISTANCE 
  
}
