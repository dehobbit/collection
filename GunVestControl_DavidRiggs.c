//ESP8266 E12 NodeMCU microcontroller

#include <SoftwareSerial.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRtimer.h>
#include <lasertag.h>
#include <IRsend.h>

#define DEBUG_MODE

#define TEAM 192
#define EOL 0x0A

#define rxPin D1
#define txPin D2
#define kRecvPin D5
#define trigger D6
#define speaker D7
#define kIrLed D8

int shoot = 0;
uint8_t userId;   //8 bit user id
uint16_t hitcode; //16 bit hit code, with an 8 bit shooter id on left and 8 bit casualty id on right

IRrecv irrecv(kRecvPin);
IRsend irsend(kIrLed);
decode_results results;
SoftwareSerial xBee(rxPin, txPin); // RX, TX

void setup()
{
  userId = 0x11;
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  irrecv.enableIRIn();
  while (!Serial) // Wait for the serial connection to be establised.
    delay(50);

  // set the data rate for the SoftwareSerial port
  xBee.begin(9600);

  //setup for gun
  Serial.begin(9600);
  pinMode(trigger, INPUT); // declare pushbutton as input

  //setup ir led and set to off
  pinMode(kIrLed, OUTPUT);
  laserStart();

  //tell player he's ready to go
  startupSound();
}

void loop() // run over and over
{
  //shoot the gun when signal received from trigger
  shoot = digitalRead(trigger); // read input value
  if (shoot == LOW)
  { // check if the input is HIGH (button released)
    laserSound();
    shootLaser(userId);
    #ifdef DEBUG_MODE
    Serial.println("Shooting.");
    #endif
  }

  //decode and transmit info if player is hit
  if (irrecv.decode(&results))
  {
    hitSound();                           //play hit sound
    hitcode = results.value;              //receive hit code from IR receiver
    xBee.write(hitcode);
    xBee.write(userId);
    xBee.write(EOL);                      //write shooter code, victim code, endline for master

    #ifdef DEBUG_MODE                     //if in debug mode, print out received signal
    serialPrintUint64(hitcode, HEX);
    Serial.println("");
    #endif

    irrecv.resume();                      // Receive the next value
    delay(1);
  }
}

/***************************************************
 * Sound effects. The following uses the arduino 
 * standard library functions tone and noTone to 
 * put a square wave of specified frequency on the 
 * speaker pin.
 ****************************************************/

//Startup sound is just reverse of shooting sound.
void startupSound()
{
  for (int i = boomLength - 1; i >= 0; i--)
  {
    tone(speaker, boom[i], 10);
    delay(13);
    noTone(speaker);
  }
}

//Shooting sound
//Consists of playing tones from 1kHz down to zero over 1 second
void laserSound()
{
  for (int i = 0; i < boomLength; i++)
  {
    tone(speaker, boom[i], 10);
    delay(13);
    noTone(speaker);
  }
}

//Hit sound when shot by other player
void hitSound()
{
  for (int i = 0; i < hitLength; i++)
  {
    tone(speaker, hit[i], 25);
    delay(13);
    noTone(speaker);
  }
}

//check if shooter on same team as user
bool friendlyFire(int shooter)
{
  int team = userId & TEAM;
  int enTeam = shooter & team;
  return (team == enTeam);
}
