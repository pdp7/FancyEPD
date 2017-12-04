#include <stdint.h>
#include <Adafruit_GFX.h>
#include "FancyEPD.h"
#include "FancyEPD_Demo_images.h"

#define THRESHOLD 1500
#define NBUTTON 3
#define NULL_VALUE -1

#define DELAY_BETWEEN_IMAGES_MS       (6 * 1000)
#define DELAY_SHORT_MS       (500)

// Pins set for project: github.com/pdp7/kicad-teensy-epaper
FancyEPD epd(k_epd_model_E2215CS062, 17, 16, 14, 15);	// hardware SPI

// epaper buttons
// "*": Pin 0
// "+": Pin 1
// "#": Pin 23

int sample[NBUTTON];
String desc[NBUTTON] = { "*", "+", "#"  };
int pin[NBUTTON] =     {  0,   1,   23  };
int reading = 0;
int disp_sample = NULL_VALUE;
String label;
bool image1 = false;
bool image2 = false;

void setup() {
  bool success;

  Serial.begin(115200);
  Serial.println("FancyEPD Teensy Badge start");
  delay(100);

  success = epd.init();

  if (!success) {
    Serial.println("epd.init(): failed");
    // Panic and freak out
    return;
  } else {
    Serial.println("epd.init(): OK");
  }

  epd.updateScreen(k_update_quick_refresh);

  epd.setRotation(0);
  epd.setBorderColor(0x00); // black
  epd.updateScreenWithImage( drewfustini, k_image_4bit_monochrome );
  delay(DELAY_SHORT_MS);
  epd.updateScreenWithImage( drewfustini, k_image_4bit_monochrome );
  delay(DELAY_BETWEEN_IMAGES_MS);

  epd.setRotation(2);
  epd.setBorderColor(0x00); // black
  epd.updateScreenWithImage( drewlandscape, k_image_4bit_monochrome );
  delay(DELAY_SHORT_MS);  
  epd.updateScreenWithImage( drewlandscape, k_image_4bit_monochrome );
  delay(DELAY_BETWEEN_IMAGES_MS);


}

void loop() {

  drawCircles();
  //delay(DELAY_BETWEEN_IMAGES_MS);
  //epd.setBorderColor(0x00);  // black
  //epd.updateScreenWithImage(angel_4bit, k_image_4bit_monochrome, k_update_quick_refresh);
  //delay(DELAY_BETWEEN_IMAGES_MS);
  //epd.clearBuffer();
  //epd.updateScreen(k_update_builtin_refresh);

  //Serial.println("delay");
  //delay(100);
  /*
    drawCircles();
    epd.updateScreen(k_update_no_blink);
    delay(DELAY_BETWEEN_IMAGES_MS);

    drawLines();
    epd.updateScreen(k_update_no_blink);
    delay(DELAY_BETWEEN_IMAGES_MS);

    // Angel
    epd.setBorderColor(0xff);	// black
    epd.updateScreenWithImage(angel_4bit, k_image_4bit_monochrome, k_update_quick_refresh);
    delay(DELAY_BETWEEN_IMAGES_MS);

    // Angel
    epd.setBorderColor(0x00);	// white
    epd.updateScreenWithImage(angel2_8bit, k_image_8bit_monochrome, k_update_quick_refresh);
    delay(DELAY_BETWEEN_IMAGES_MS);

    // Doggy
    epd.setBorderColor(0x40);	// grey-ish
    epd.updateScreenWithImage(doggy_2bit, k_image_2bit_monochrome, k_update_quick_refresh);
    delay(DELAY_BETWEEN_IMAGES_MS);
  */
}

bool drawCircles()
{
  int w = random(epd.width());
  int h = random(epd.height());
  epd.clearBuffer();
  int max_thickness = random(3, 10);
  int radius_scale = random(10, 15);
  for (int thickness = max_thickness; thickness > 0; thickness--) {
    uint8_t radius = (max_thickness - thickness) * radius_scale;
    for (int i = 0; i < thickness; i++) {
      epd.drawCircle(w, h, radius++, 0xff);
    }
    epd.updateScreen(k_update_no_blink);
    // TODO: support simultaneous button presses
    for (int i = 0; i < NBUTTON; i++) {

      sample[i] = touchRead(pin[i]);

      /*
          NOTE: only one pressed button
          will "win" for each iteration
          of loop(). Thus "multi-touch"
          is not supported.
      */
      if (sample[i] > THRESHOLD) {
        disp_sample = i;
      }

      /*Serial.print(i);
        Serial.print("\t");
        Serial.print(sample[i]);
        Serial.print("\t");
        Serial.print(pin[i] );
        Serial.print("\t");
        Serial.print(desc[i] );
        Serial.print("\t");*/
      if (disp_sample >= 0) {
        Serial.print("drawCircle(): disp_sample=");
        Serial.print( disp_sample );
        if (disp_sample == 1) {
          image1 = true;
          epd.setBorderColor(0x00);  // black
          epd.setRotation(2);
          epd.updateScreenWithImage(tindie_logo, k_image_4bit_monochrome);
          delay(DELAY_SHORT_MS);
          epd.updateScreenWithImage(tindie_logo, k_image_4bit_monochrome);          
          delay(DELAY_BETWEEN_IMAGES_MS);
          image1 = false;
        } else  if (disp_sample == 2) {
          image2 = true;
          epd.setBorderColor(0x00);  // black
          epd.setRotation(2);
          epd.updateScreenWithImage(hackaday_logo, k_image_4bit_monochrome);
delay(DELAY_SHORT_MS);
          epd.updateScreenWithImage(hackaday_logo, k_image_4bit_monochrome);
          delay(DELAY_BETWEEN_IMAGES_MS);
          image2 = false;
        }

        Serial.println(". return false");
        disp_sample = NULL_VALUE; // reset disp_sample
        return false; //interrupted by button press
      }

    }
  }
  Serial.println("drawCircle(): completed");
  return true;
}



