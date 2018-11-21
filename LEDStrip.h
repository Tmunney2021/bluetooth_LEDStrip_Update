#define PIN 6 // pin led strip is conected to

#include <EEPROM.h>

boolean PRCS = false;    boolean PRCSDefault = false;     int PRCSAddress  =  1;   //play Random Color Setting

int CWTSDefault  = 50;             int CWTSAddress  =  2;    // Color wipe timming setting
int TCTSDefault  = 70;             int TCTSAddress  =  3;    // Theater Chase timming setting
int RTSDefault   = 50;             int RTSAddress   =  4;    // Rainbow timming setting
int RChTSDefault = 47;             int RChTSAddress =  5;    // Rainbow Chase timming setting
int RCyTSDefault = 60;             int RCyTSAddress =  6;    // Rainbow Cycle timming setting
int BBTSDefault  = 45;             int BBTSAddress  =  7;    // Block Build timming setting
int CFTSDefault  = 45;             int CFTSAddress  =  8;    // Cross Fire timming setting
int HITSDefault  = 45;             int HITSAddress  =  9;    // Half In timming setting
int HOTSDefault  = 45;             int HOTSAddress  =  10;   // Half Out timming setting
int VTSDefault   = 45;             int VTSAddress   =  11;   // Volly timming setting
int FTSDefault   = 50;            int FTSAddress   =  12;   // Volly timming setting
int STSDefault   = 50;            int STSAddress   =  13;   // Volly timming setting


#include "PreSetColors.h" //personal color library

PreSetColors Red(255, 0, 0);
PreSetColors Green(0, 255, 0);
PreSetColors Blue(0, 0, 255);
PreSetColors Pink(255, 0, 255);
PreSetColors Orange(255, 128, 0);
PreSetColors Black(32, 32, 32);

PreSetColors Colors[6] = {Red, Green, Blue, Pink, Orange, Black};

// Pattern types supported:
enum  pattern { NONE, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE, RAINBOW, HALF_OUT, HALF_IN, VOLLY, BLOCK_BUILD, THEATER_CHASE_RAINBOW, CROSS_FIRE};
enum  BTChecking { EFFECTS, TIMMING, PLAYRANDOMCOLOR};
// Patern directions supported:
enum  direction { FORWARD, REVERSE};

// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : public Adafruit_NeoPixel
{
  public:

    // Member Variables:
    pattern  ActivePattern;  // which pattern is running
    direction Direction;     // direction to run the pattern

    unsigned long Interval;   // milliseconds between updates
    unsigned long lastUpdate; // last update of position

    uint32_t Color1, Color2;  // What colors are in use
    uint16_t TotalSteps;  // total number of steps in the pattern
    uint16_t Index;  // current step within the pattern
    uint16_t Build;  // keep track of how far the build has gotten for block build

    void (*OnComplete)();  // Callback on completion of pattern

    // Constructor - calls base-class constructor to initialize strip
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type/*, void (*callback)()*/)
      : Adafruit_NeoPixel(pixels, pin, type)
    {
      //OnComplete = callback;
    }

    // Update the pattern
    void Update()
    {
      if ((millis() - lastUpdate) > Interval) // time to update
      {
        lastUpdate = millis();
        switch (ActivePattern)
        {
          case RAINBOW_CYCLE:
            RainbowCycleUpdate();
            break;
          case THEATER_CHASE:
            TheaterChaseUpdate();
            break;
          case COLOR_WIPE:
            ColorWipeUpdate();
            break;
          case SCANNER:
            ScannerUpdate();
            break;
          case FADE:
            FadeUpdate();
            break;
          case RAINBOW:
            RainbowUpdate();
            break;
          case HALF_OUT:
            HalfOutUpdate();
            break;
          case HALF_IN:
            HalfInUpdate();
            break;
          case VOLLY:
            VollyUpdate();
            break;
          case CROSS_FIRE:
            CrossFireUpdate();
            break;
          case BLOCK_BUILD:
            BlockBuildUpdate();
            break;
          default:
            break;
        }
      }
    }

    // Increment the Index and reset at the end
    void Increment()
    {
      if (Direction == FORWARD)
      {
        Index++;
        if (Index >= TotalSteps)
        {
          Index = 0;
          StickComplete();
        }
      }
      else // Direction == REVERSE
      {
        --Index;
        if (Index <= 0)
        {
          Index = TotalSteps - 1;
          StickComplete();
        }
      }
    }

    // Reverse pattern direction
    void Reverse()
    {
      if (Direction == FORWARD)
      {
        Direction = REVERSE;
        Index = TotalSteps - 1;
      }
      else
      {
        Direction = FORWARD;
        Index = 0;
      }
    }

    // Initialize for a RainbowCycle
    void RainbowCycle(uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = RAINBOW_CYCLE;
      Interval = interval;
      TotalSteps = 255;
      Index = 0;
      Direction = dir;
      ColorSet(Color(0, 0, 0));
    }

    // Update the Rainbow Cycle Pattern
    void RainbowCycleUpdate()
    {
      for (int i = 0; i < numPixels(); i++)
      {
        setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
      }
      show();
      Increment();
    }

    // Initialize for a Theater Chase
    void TheaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = THEATER_CHASE;
      Interval = interval;
      TotalSteps = numPixels();
      Color1 = color1;
      Color2 = color2;
      Index = 0;
      Direction = dir;
      ColorSet(Color(0, 0, 0));
    }

    // Update the Theater Chase Pattern
    void TheaterChaseUpdate()
    {
      for (int i = 0; i < numPixels(); i++)
      {
        if ((i + Index) % 3 == 0)
        {
          setPixelColor(i, DimColor(DimColor(Color1)));
        }
        else
        {
          setPixelColor(i, DimColor(DimColor(Color2)));
        }
      }
      show();
      Increment();
    }

    // Initialize for a ColorWipe
    void ColorWipe(uint32_t color, uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = COLOR_WIPE;
      Interval = interval;
      TotalSteps = numPixels();
      Color1 = color;
      Index = 0;
      Direction = dir;
      ColorSet(Color(0, 0, 0));
    }

    // Update the Color Wipe Pattern
    void ColorWipeUpdate()
    {
      setPixelColor(Index, Color1);
      show();
      Increment();
    }

    // Initialize for a SCANNNER
    void Scanner(uint32_t color1, uint8_t interval)
    {
      ActivePattern = SCANNER;
      Interval = interval;
      TotalSteps = (numPixels() - 1) * 2;
      Color1 = color1;
      Index = 0;
      ColorSet(Color(0, 0, 0));
    }

    // Update the Scanner Pattern
    void ScannerUpdate()
    {
      for (int i = 0; i < numPixels(); i++)
      {
        if (i == Index)  // Scan Pixel to the right
        {
          setPixelColor(i, Color1);
        }
        else if (i == TotalSteps - Index) // Scan Pixel to the left
        {
          setPixelColor(i, Color1);
        }
        else // Fading tail
        {
          setPixelColor(i, DimColor(getPixelColor(i)));
        }
      }
      show();
      Increment();
    }

    // Initialize for a Fade
    void Fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = FADE;
      Interval = interval;
      TotalSteps = steps;
      Color1 = color1;
      Color2 = color2;
      Index = 0;
      // if(dir==REVERSE)
      // {
      //       ColorSet(Color(0, 0, 0));
      //   }
      Direction = dir;

    }

    // Update the Fade Pattern
    void FadeUpdate()
    {
      // Calculate linear interpolation between Color1 and Color2
      // Optimise order of operations to minimize truncation error
      uint8_t red = ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) / TotalSteps;
      uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
      uint8_t blue = ((Blue(Color1) * (TotalSteps - Index)) + (Blue(Color2) * Index)) / TotalSteps;

      ColorSet(Color(red, green, blue));
      show();
      Increment();
    }


    // Initialize for a RainbowCycle
    void Rainbow(uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = RAINBOW;
      Interval = interval;
      TotalSteps = 255;
      Index = 0;
      Direction = dir;
      ColorSet(Color(0, 0, 0));
    }

    // Update the Rainbow Cycle Pattern
    void RainbowUpdate()
    { for (int i = 0; i < numPixels(); i++)
      {
        setPixelColor(i, Wheel((i + Index) & 255));
      }
      show();
      Increment();
    }

    // Initialize for a Half Out
    void HalfOut(uint32_t color, uint8_t interval)
    {
      ActivePattern = HALF_OUT;
      Interval = interval;
      TotalSteps = numPixels();
      Color1 = color;
      Index = 0;
      ColorSet(Color(0, 0, 0));
    }

    void HalfOutUpdate()
    {
      if (Index == 0)
        setPixelColor(numPixels() / 2, Color1);
      setPixelColor((numPixels() / 2) + Index, Color1);
      setPixelColor((numPixels() / 2) - Index, Color1);
      show();
      Increment();
    }

    // Initialize for a Half In
    void HalfIn(uint32_t color, uint8_t interval)
    {
      ActivePattern = HALF_IN;
      Interval = interval;
      TotalSteps = numPixels() / 2;
      Color1 = color;
      Index = 0;
      ColorSet(Color(0, 0, 0));
    }

    void HalfInUpdate()
    {
      setPixelColor(Index, Color1);
      setPixelColor(numPixels() - Index, Color1);
      show();
      Increment();
    }

    // Initialize for a Volly
    void Volly(uint32_t color, uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = VOLLY;
      Interval = interval;
      TotalSteps = numPixels() - 2;
      Color1 = color;
      Direction = dir;
      if (Direction == FORWARD)
      {
        Index = 0;
      }
      else
      {
        Index = numPixels();
      }
      ColorSet(Color(0, 0, 0));
    }

    void VollyUpdate()
    {
      if (Direction == FORWARD)
      {
        setPixelColor(Index - 1, Color(0, 0, 0));
        setPixelColor(Index - 2, Color(0, 0, 0));

        setPixelColor(Index, Color1);
        setPixelColor(Index + 1, Color1);
        setPixelColor(Index + 2, Color1);
      }
      else if (Direction == REVERSE)
      {
        setPixelColor(Index + 1, Color(0, 0, 0));

        setPixelColor(Index, Color1);
        setPixelColor(Index - 1, Color1);
        setPixelColor(Index - 2, Color1);
      }
      show();
      Increment();
    }

    // Initialize for a Cross Fire
    void CrossFire(uint32_t color1, uint32_t color2, uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = CROSS_FIRE;
      Interval = interval;
      TotalSteps = numPixels() / 2;
      Color1 = color1;
      Color2 = color2;
      Direction = dir;
      Index = 0;
      ColorSet(Color(0, 0, 0));
    }

    void CrossFireUpdate()
    {
      if (Direction == FORWARD)
      {
        setPixelColor(Index, Color1);
        setPixelColor(numPixels() - Index, Color2);
      }
      else if (Direction == REVERSE)
      {
        setPixelColor(numPixels() / 2 + Index, Color2);
        setPixelColor(numPixels() / 2 - Index, Color1);
      }
      show();
      Increment();
    }

    // Initialize for a Block Build
    void BlockBuild(uint32_t color, uint8_t interval, int build = 0)
    {
      ActivePattern = BLOCK_BUILD;
      Interval = interval;
      TotalSteps = (numPixels() - build) - 2;
      Color1 = color;
      Build = build;
      Index = 0;
      if (Build == 0)
      {
        ColorSet(Color(0, 0, 0));
      }
    }

    void BlockBuildUpdate()
    {
      setPixelColor(Index - 1, Color(0, 0, 0));
      setPixelColor(Index - 2, Color(0, 0, 0));

      setPixelColor(Index, Color1);
      setPixelColor(Index + 1, Color1);
      setPixelColor(Index + 2, Color1);

      show();
      Increment();
    }

    // Calculate 50% dimmed version of a color (used by ScannerUpdate)
    uint32_t DimColor(uint32_t color)
    {
      // Shift R, G and B components one bit to the right
      uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
      return dimColor;
    }

    // Set all pixels to a color (synchronously)
    void ColorSet(uint32_t color)
    {
      for (int i = 0; i < numPixels(); i++)
      {
        setPixelColor(i, color);
      }
      show();
    }


    // Returns the Red component of a 32-bit color
    uint8_t Red(uint32_t color)
    {
      return (color >> 16) & 0xFF;
    }

    // Returns the Green component of a 32-bit color
    uint8_t Green(uint32_t color)
    {
      return (color >> 8) & 0xFF;
    }

    // Returns the Blue component of a 32-bit color
    uint8_t Blue(uint32_t color)
    {
      return color & 0xFF;
    }

    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t Wheel(byte WheelPos)
    {
      WheelPos = 255 - WheelPos;
      if (WheelPos < 85)
      {
        return Color(255 - WheelPos * 3, 0, WheelPos * 3);
      }
      else if (WheelPos < 170)
      {
        WheelPos -= 85;
        return Color(0, WheelPos * 3, 255 - WheelPos * 3);
      }
      else
      {
        WheelPos -= 170;
        return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
      }
    }

    uint32_t colorPicker()
    {
      if (EEPROM.read(PRCSAddress) == 0)
      {
        return  RandomPSColorReturn();
      }
      return RandomColorReturn();
    }

    uint32_t RandomColorReturn()
    {
      return Color(random(255), random(255), random(255));
    }

    uint32_t RandomPSColorReturn()
    {
      return Colors[random(5)].getColor();
    }


    void StickComplete()
    {
      switch (ActivePattern)
      {
        case RAINBOW_CYCLE:
          {
            RainbowCycle(EEPROM.read(RChTSAddress));
            break;
          }
        case THEATER_CHASE:
          {
            TheaterChase(RandomColorReturn(), RandomColorReturn(), EEPROM.read(TCTSAddress)) ;
            break;
          }
        case COLOR_WIPE:
          {
            ColorWipe(RandomColorReturn(), EEPROM.read(CWTSAddress));
            break;
          }
        case SCANNER:
          {
            Scanner(RandomColorReturn(), EEPROM.read(STSAddress));
            break;
          }
        case FADE: //restart effect where it fades from the end color to a new color
          {
            Fade(Color2, RandomColorReturn(), 100, EEPROM.read(FTSAddress));
            break;
          }
        case RAINBOW:
          {
            Rainbow(EEPROM.read(RTSAddress));
            break;
          }
        case HALF_OUT:
          {
            HalfOut(RandomColorReturn(), EEPROM.read(HOTSAddress));
            break;
          }
        case HALF_IN:
          {
            setPixelColor(numPixels() / 2, Color1); //set middle pixel to color because im to lazy in actully implement it withen where it should be
            HalfIn(RandomColorReturn(), EEPROM.read(HITSAddress));
            break;
          }
        case VOLLY:; //Either run reversed run or restart effect
          {
            if (Direction == FORWARD) //if the effect is finishing its foward part then set to reverse then run again
            {
              Volly(Color1, Interval, REVERSE);
            }
            else //if the reversed part is done then start forward part again with different color
            {
              Volly(RandomColorReturn(), EEPROM.read(VTSAddress));
            }
            break;
          }
        case CROSS_FIRE:; //Either run reversed run or restart effect
          {
            if (Direction == FORWARD) //if the effect is finishing its foward part then set to reverse then run again
            {
              CrossFire(Color1, Color2, Interval, REVERSE);
            }
            else //if the reversed part is done then start forward part again with different color
            {
              CrossFire(RandomColorReturn(), RandomColorReturn(), EEPROM.read(CFTSAddress), REVERSE);
            }
            break;
          }
        case BLOCK_BUILD:
          {
            /*    cool rainbowish block build idea
              if (Build > numPixels())
              {
               BlockBuild(RandomColorReturn(), 45, 0);
              }
              BlockBuild(RandomColorReturn(), 45, Build += 3);
            */
            if (Build > numPixels())
            {
              BlockBuild(RandomColorReturn(), EEPROM.read(BBTSAddress));
            }
            BlockBuild(Color1, EEPROM.read(BBTSAddress), Build += 3);
            break;
          }

        default:
          break;
      }
    }
};
