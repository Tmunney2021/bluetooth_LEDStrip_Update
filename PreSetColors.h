class PreSetColors : public Adafruit_NeoPixel
{
  public:
    // Member Variables:
    int rVal, gVal, bVal; //these will hold and remember the rgb values for the colors

    // Constructor - calls base-class constructor to initialize strip
    PreSetColors(int r, int g, int b)
    {
      rVal = r;
      gVal = g;
      bVal = b;
    }

    int getR() //return the r value of the color
    {
      return rVal;
    }

    int getG() // return the g value of the color
    {
      return gVal;
    }

    int getB()  //return the b value of the color
    {
      return bVal;
    }

    uint32_t getColor()  // return the adafruit RGB color to diectly apply to an RGB strip
    {
      return Color(rVal, gVal, bVal);
    }
};
