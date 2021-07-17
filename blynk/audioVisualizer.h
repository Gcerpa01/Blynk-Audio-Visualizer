#ifndef aV
#define aV

#include <stdint.h>
#include <FastLED.h>


#define NUM_LEDS 300
#define LED_TYPE WSB2812b
#define DATA_PIN 4 //TEST D7 //note application uses D4 but fastLED GPIO layout has it differently

// Define color structure for lights
class Color{
  private:
  /* Set initial values for pitch, color and volume. Volume is set to 50 to account for offset for the lights to be off when there is no audio */
  bool dull = false;
  bool pitchy = false;
  bool norm = false;
  int low;
  int med;
  int high;
  int repeat = 0;
  uint8_t pitch_hue = 255;
  uint8_t color_hue = 255;
  uint8_t volume = 50;
  int bpm;
  int _mode = 0;

  
  public:
  int getPitch_Hue();
  int getColor_Hue();
  int getVolume();
  void setHue(int vals[]);
  void setVolume(int vals[]);
  void animate(int vals[]);
  void freq(int vals[]);
  void spd(int vals[]);
  void ledSetup();

  void sparkleAudioI(int vals[]);
  void sparkleAudioII(int vals[]);
  /*Additional animating options that do not require any audio reads*/
  void staticLED(int r, int g, int b,int bright);
  void pridefx();

  
};


#endif
