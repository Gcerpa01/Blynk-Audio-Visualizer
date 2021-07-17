/* Gerardo Cerpa
 *
 * Utilized David Wang's code to read audio input from a 3.5 mm cable
 * so huge amount of gratitude to him.
 * The audio input is then used to animate an LED stip based
 * on the pitch and volume of the music.
 *
 *Also utilized Mark Wang's Pride 2015 animation, all credit goes to him
 *for that section of code and gratitude for an introduction
 *to LEDs
 *
 */
 
#define BLYNK_PRINT Serial
#include <FastLED.h>
#include "audioVisualizer.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "OTABlynkCredentials.h"

/*Variables and isntantiation of object to hold WiFi credentials */

credentials Credentials;
char auth[33]; //holder for Blynk authentication 
char* ssid= "LEDLights"; //enter WiFi host name here
char* pass = "PrototypeV1"; //enter WiFi password here

/*  ------------------------------ */
int analogPin = A0; // MSGEQ7 OUT
int strobePin = D1; // MSGEQ7 STROBE //D1 Application // D5 Test
int resetPin = D0; // MSGEQ7 RESET //D0 Application  // D6 Test
int spectrumValue[7];
// MSGEQ7 OUT pin produces values around 50-80
// when there is no input, so use this value to
// filter out a lot of the chaff.
int filterValue = 80;
int brightness = 50;
/* --------------------------------- */

//Control variables to help dictate the mode running
int lightStatus = 0;
int effect = false;
int R,G,B;
int userB;

//control variable to check for internet conection
bool connected_to_internet = 0;

//Create led object
Color led;

void setup() {
  Serial.begin(9600);
  
  //make a string using Config information
  String auth_string = Credentials.EEPROM_Config();
  auth_string.toCharArray(auth,33);

  //If credentials were received,connect to network
  if(Credentials.credentials_get()){
    Blynk.config(auth);
    connected_to_internet = 1;
  }
  else{
    //Credentials were received and no connection, create Access Point
    Credentials.setupAP(ssid,pass);
    connected_to_internet = 0;
  }

  if(connected_to_internet){
   led.ledSetup();
  /*---------------*/
  //Setup the pins for reading from MSGEQ7
  randomSeed(analogRead(A0));
  // Read from MSGEQ7 OUT
  pinMode(analogPin, INPUT);
  // Write to MSGEQ7 STROBE and RESET
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  // Set analogPin's reference voltage
  //analogReference(DEFAULT); // 5V
 
  // Set startup values for pins
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
  /*-------------------*/
  }
 
  
  
  
  //Blynk.begin(auth,ssid,pass);
}

//Create an array of different methods to dictate the mode that will be
//running with the program
typedef void(*Spectremodes[])();
Spectremodes pos = {off,audio,pick,sparkle_AudioI,sparkle_AudioII,pride};

//Read the mode the user has selected in the mode menu
//the mode selected will change the status integer dictating
//the position of the array element
BLYNK_WRITE(V0){
  switch (param.asInt()){
  case 1:
    lightStatus = 0;
    break;
  case 2:
    lightStatus = 1;
    break;
  case 3:
    lightStatus = 3;
    break;
  case 4:
    lightStatus = 4;
    break;
  case 5: 
    lightStatus = 5;
    break;
  }
}

//Read user input from ZeRGBa and pass the values
//to a function to display user selected colors
BLYNK_WRITE(V7){
  R = param[0].asInt();
  G = param[1].asInt();
  B = param[2].asInt();
  if(effect == false && lightStatus != 0){
    lightStatus = 2;
    Serial.println("Static mode");
  }
}

//Read user selected Brightness
BLYNK_WRITE(V8){
  userB = param.asInt();
}

//Determine on whether effects are on or off
//having effects off will allow the user to be able
//to select their color for display
BLYNK_WRITE(V1){
  if (param.asInt() == 0){
    effect = false;
    Serial.println("Effects are off");
  }
  else{
    effect = true;
    Serial.println("Effects are on");
  }
      
}

void loop() {
    Credentials.server_loops();
    if(connected_to_internet){
      Blynk.run();
      pos[lightStatus](); //run function based on user input
    }
}

//Runs the audio visualizer function to display
//an audio spectrum across the lights
void audio(){
  if(effect == true){
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
  // Get all 7 spectrum values from the MSGEQ7
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); // Allow output to settle
 
    spectrumValue[i] = analogRead(analogPin);
 
    // Constrain any value above 1023 or below filterValue
    spectrumValue[i] = constrain(spectrumValue[i], filterValue, 1023);
 
 
    // Remap the value to a number between 0 and 255
    spectrumValue[i] = map(spectrumValue[i], filterValue, 1023, 0, 255);
 
    // Remove serial stuff after debugging
    Serial.print(spectrumValue[i]);
    Serial.print(" ");
    digitalWrite(strobePin, HIGH);
   }
  Serial.println("");
  Color audioLights;
  audioLights.animate(spectrumValue);
  }
  else{
  Serial.println("Animate is not fully on");
  }
}

//Turns all leds off
void off() {
  Serial.println("Off");
  do{                             //Start of a loop
    brightness = brightness - 10;           //Subtract 10 from the bright variable and save it
    FastLED.setBrightness(brightness);  //Set LED strip Brightness to the value storged in "Bright"
    FastLED.show();                 //Update LED strip
    delay(10);                      //Wait for 20ms
    }while(brightness > 0);             //Go back to start of the loop if Bight is bigger then 0
    FastLED.clear();
    brightness = 50;                    //turn of LED strip
    FastLED.setBrightness(brightness); 
    FastLED.show();
}

//Changes the color to the User selected color from ZeRGBa
void pick(){
  Color staticColor;
  staticColor.staticLED(G,R,B,userB);
  Serial.println("Static mode is on");
  FastLED.show();
}


//Changes animation to sample provided in FastLED library
void pride(){
  if(effect == true){
    Color pride;
    pride.pridefx();
    Serial.println("Pride is on");
    FastLED.setBrightness(brightness); 
    FastLED.show();
  }
}

void sparkle_AudioI(){
  if(effect == true){
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
  // Get all 7 spectrum values from the MSGEQ7
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); // Allow output to settle
 
    spectrumValue[i] = analogRead(analogPin);
 
    // Constrain any value above 1023 or below filterValue
    spectrumValue[i] = constrain(spectrumValue[i], filterValue, 1023);
 
 
    // Remap the value to a number between 0 and 255
    spectrumValue[i] = map(spectrumValue[i], filterValue, 1023, 0, 255);
 
    // Remove serial stuff after debugging
    Serial.print(spectrumValue[i]);
    Serial.print(" ");
    digitalWrite(strobePin, HIGH);
   }
  Serial.println("");
  Color audioLights;
  audioLights.sparkleAudioI(spectrumValue);
  }
}

void sparkle_AudioII(){
  if(effect == true){
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
  // Get all 7 spectrum values from the MSGEQ7
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); // Allow output to settle
 
    spectrumValue[i] = analogRead(analogPin);
 
    // Constrain any value above 1023 or below filterValue
    spectrumValue[i] = constrain(spectrumValue[i], filterValue, 1023);
 
 
    // Remap the value to a number between 0 and 255
    spectrumValue[i] = map(spectrumValue[i], filterValue, 1023, 0, 255);
 
    // Remove serial stuff after debugging
    Serial.print(spectrumValue[i]);
    Serial.print(" ");
    digitalWrite(strobePin, HIGH);
   }
  Serial.println("");
  Color audioLights;
  audioLights.sparkleAudioII(spectrumValue);
  }
}
