#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define NEO_PIXEL_PIN PD7
#define BUTTON_PIN PD6

#define INCREMENT 30

class Bar
{
private:
  int intensities[3];
  bool direction;
  int gap;
  int steps;

public:
  Bar() {}

  void init(int starting_intensity, int g)
  {
    gap = g;
    steps = 0;
    for (int i = 0; i <= 2; i++)
    {
      intensities[i] = 0;
    }
  }

  void update()
  {
    if (intensities[0] >= 255)
    {
      // The sweep has reached full intensity for all LEDs, time to count down.
      direction = false;
      steps = 0;
    }

    if (intensities[2] <= 0)
    {
      // The sweep has reached zero intensity for all LEDs, time to count up.
      direction = true;
      steps = 0;
    }
    steps++;
    if (direction)
    {
      int last_intensity = intensities[0];
      for (int i = 0; i <= 2; i++)
      {
        last_intensity = last_intensity + gap;
        intensities[i] = (last_intensity > 255) ? 255 : last_intensity;
      }
    }
    else
    {
      int last_intensity = intensities[2];
      for (int i = 2; i >= 0; i--)
      {
        last_intensity = last_intensity - gap;
        intensities[i] = (last_intensity < 0) ? 0 : last_intensity;
      }
    }
  }

  int getPixel(int i)
  {
    return intensities[i];
  }
};

Adafruit_NeoPixel strip;
Bar bar;
bool run = false;

void setup()
{
  strip = Adafruit_NeoPixel(3, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);
  strip.begin();
  strip.clear();
  strip.show();
  bar.init(0, INCREMENT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
  if (run)
  {
    strip.clear();
    bar.update();
    for (int i = 0; i <= 2; i++)
    {
      // For red
      strip.setPixelColor(i, strip.Color(bar.getPixel(i), 0, 0));
      // For blue
      // strip.setPixelColor(i, strip.Color(0, 0, bar.getPixel(i)));
    }
    strip.show();
  }
  if (digitalRead(BUTTON_PIN) == LOW)
  {
    run = !run;
    strip.clear();
    strip.show();
    delay(500);
  }
  delay(100);
}