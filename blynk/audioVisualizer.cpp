

#include "audioVisualizer.h"
#include <FastLED.h>

CRGB leds[NUM_LEDS];

//Setup LEDs to start being turned off
void Color :: ledSetup(){  
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(84);
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(0,0,0);
  }
}

void Color :: setVolume(int vals[]){
  for (int i = 0; i < 4; i++){
    /*Get volume to determine brightness*/
    volume += int(vals[i]/4);
  }
  
}


/* Seperate the 7 bands bands into three different frequencies */

void Color:: freq(int vals[]){
  low = vals[0] + vals[1];
  med = vals[2] + vals[3] + vals[4];
  high = vals[5] + vals[6];

  if(low > high && low > med){
    dull = true;
  }
  else if(high > med && med > low){
    high = true;
  }
  else{
    norm = true;
  }
}

void Color :: setHue(int vals[]){
  freq(vals);

  /* Utilize the switches in order to calculate the different hues and pitches the leds will show in accordance to the audio in */

  switch(dull){
    case true:
      pitch_hue = 0.30*low + high;
      break;
    case false:
      color_hue -= 0.25*low;
      break;
  }

  switch(norm){
    case true:
      pitch_hue = 0.15*med + low;
      break;
    case false:
      color_hue -= 0.25*med;
  }
  switch(pitchy){
    case true:
      pitch_hue = 0.15*high + med;
      break;
    case false:
      color_hue -= 0.25*high;
  }
}

int Color:: getPitch_Hue(){
  return pitch_hue;
}

int Color:: getColor_Hue(){
  return color_hue;
}

int Color:: getVolume(){
  return volume;
}


void Color:: spd (int vals[]){
  int _mode = 0;
  
  /* Calculate which frequency is the highest across the 7 bands of the graphic equalizer */

  for(int i = 0; i < sizeof(vals)/sizeof(vals[0]) - 1 ;i++){
      if( vals[i] < vals[i +1]){
          _mode = i;
      }
  }


 /* Determine the amount of leds that will be updated based on which frequency has the highest value. The lower the frequency, the less LEDS update */

  switch(_mode){
  case 0:
      bpm = 3;
      break;
  case 1:
      bpm = 5;
      break;
  case 2:
      bpm = 7;
      break;
  case 3:
      bpm = 13;
      break;
  case 4: 
      bpm = 15;
      break;
  case 5:
      bpm = 17;
      break;
  case 6:
      bpm = 19 ;
      break;
  }
}
void Color::animate(int vals[]){
  LEDS.setBrightness(84);
  spd(vals);

 /* Offset the LEDS to the right by the amount that will be updated */
  for(int i = NUM_LEDS - 1; i >= bpm; i--){
    leds[i] = leds[i - bpm];
  }
  
  /* Calculate for a repeat of pitch in succession */
  if(getPitch_Hue() > (getVolume()-50)){
    repeat += 1;
  }
  
  setHue(vals);
  setVolume(vals);
    for(int i = 0; i < bpm; i++){
    leds[i] = CHSV(pitch_hue,color_hue,volume - 50);
  }


  /* A succession of the same frequncy 4 times will result in a change of color at a random pace */

  if(repeat >= 4){
    for(int i = 0; i < repeat*2; i++){
    leds[random(repeat,i)] = CHSV(pitch_hue,color_hue,volume -50);
    }
        repeat = 0;
  }
    FastLED.show();
    pitch_hue= 0;
    color_hue = 255;
    
}


void Color:: staticLED(int g, int r, int b, int bright){
   for(int i = 0; i < NUM_LEDS; i++){
      leds[i] = CRGB(g,r,b);
    }
   LEDS.setBrightness(bright);
}

void Color:: pridefx(){
  //Taken from FastLED example, Pride 2015, by Mark Kriegsman
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS-1) - pixelnumber;
    
    nblend( leds[pixelnumber], newcolor, 64);
  }
}

void Color :: sparkleAudioI(int vals[]){
  spd(vals);
  setHue(vals);
  setVolume(vals);
  int pixel = random(NUM_LEDS); //determine a random position on the array
  for (int i = 0; i < 3 ; i++){ //change a segment of the leds to the same color based on frequency
    if(pixel == NUM_LEDS || pixel == NUM_LEDS - 1 || pixel == NUM_LEDS - 2){
      leds[pixel - i] = CHSV(pitch_hue,color_hue,volume - 50);
    }
      leds[pixel + i] = CHSV(pitch_hue,color_hue,volume - 50);
  }
  FastLED.show();
  EVERY_N_MILLISECONDS( 20 ); //20 millisecond delay
  for (int i = 0; i < 3 ; i++){ //turn off the leds turned on before
    if(pixel == NUM_LEDS || pixel == NUM_LEDS - 1 || pixel == NUM_LEDS - 2){
      leds[pixel - i] = CHSV(0,0,0);
    }
      leds[pixel + i] = CHSV(0,0,0);
  }
  
  pitch_hue= 0;
  color_hue = 255;
}

void Color :: sparkleAudioII(int vals[]){
  spd(vals);
  setHue(vals);
  setVolume(vals);
  int pixel[3] = {random(NUM_LEDS),random(NUM_LEDS),random(NUM_LEDS)}; //change 3 random leds to the same color based on frequency input
  for (int i = 0; i < 3 ; i++){
      leds[pixel[i]] = CHSV(pitch_hue,color_hue,volume - 50);
  }
  FastLED.show();
  EVERY_N_MILLISECONDS( 20 );
  for (int i = 0; i < 3 ; i++){ //turn off the leds 
      leds[pixel[i]] = CHSV(0,0,0);
  }
  pitch_hue= 0; 
  color_hue = 255;
}
