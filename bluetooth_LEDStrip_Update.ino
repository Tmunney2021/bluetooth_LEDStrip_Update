#include <Adafruit_NeoPixel.h> //led strip library
#include "LEDStrip.h" //personal LED library


#define PIN 6
NeoPatterns Stick(50,PIN, NEO_GRB + NEO_KHZ800); // creting led strip

// blueTooth setup
#include <SoftwareSerial.h>
SoftwareSerial mySerial(11, 12); // RX, TX   Digital pin 11 -> TX of BlueTooth
//                                           Digital pin 12 -> RX of BlueTooth
char receivedChar;


//RGB values maybe in use? check if variables are neccesary
int rVal;
int gVal;
int bVal;

#include <EEPROM.h>

void getChar() //recieve a char
{
  receivedChar = mySerial.read(); // get the char to test
  if (checkChar(receivedChar))    //if inbetwwen a and z
  {
    //if the recievedChar is withen what i want then procede with the switch statment
    //DB  Serial.println(receivedChar);
    switch (receivedChar)
    {
      case '0':// off
        {
          //DB Serial.println("OFF");
          //Stick.Scanner(Stick.RandomColorReturn(), EEPROM.read(CWTSAddress));
          Stick.ColorWipe(Stick.Color(0, 0, 0), EEPROM.read(CWTSAddress));
          break;
        }
      case '1':// Color wipe
        {
          //DB Serial.println("Color Wipe");
          
          Stick.ColorWipe(Stick.colorPicker(), EEPROM.read(CWTSAddress));
          break;
        }
      case '2': // theterChase
        {
          //DB Serial.println("Theter Chase");
          Stick.TheaterChase(Stick.colorPicker(), Stick.RandomColorReturn(), EEPROM.read(TCTSAddress)) ;
          break;
        }

      case '3': //rainbow
        {
          //DB Serial.println("rainbow");
          Stick.Rainbow(EEPROM.read(RTSAddress));
          break;
        }

      case '4': //rainbow Cycle
        {
          //DB Serial.println("rainbow cycle");
          Stick.RainbowCycle(EEPROM.read(RChTSAddress));
          break;
        }

      case '5': // theater chase rainbow
        {
          //DB Serial.println("rainbow theter chase");
          //Implement later, maybe. prob not.
          // theaterChaseRainbow(EEPROM.read(TCRTSAddress));
          break;
        }

      case '6': //block build
        {
          //DB  uint32_t color = selectColor();
          Stick.BlockBuild(Stick.colorPicker(), EEPROM.read(BBTSAddress));
          break;
        }

      case '7': //Half In
        {
          Stick.HalfIn(Stick.colorPicker(), EEPROM.read(HITSAddress));
          break;
        }
      case '8': //Half Out
        {
          Stick.HalfOut(Stick.colorPicker(), EEPROM.read(HOTSAddress));
          break;
        }

      case '9': //Volly
        {
          Stick.Volly(Stick.colorPicker(), EEPROM.read(VTSAddress));
          break;
        }
      case 'a': //Fade
        {
          Stick.Fade(Stick.Color(0, 0, 0), Stick.colorPicker(), 50, EEPROM.read(FTSAddress));
          break;
        }

      case 'o':// scanner
        {
          //DB Serial.println("SCANNER");
          Stick.Scanner(Stick.colorPicker(), EEPROM.read(STSAddress));
          break;
        }
      case 'z':// crossfire
        {
          //DB   Serial.println("Cross Fire");
          Stick.CrossFire(Stick.colorPicker(), Stick.colorPicker(), EEPROM.read(CFTSAddress));
          break;
        }

      //Start of the settings

      case 'b': // set setting to true
        {
          //Serial.println(" true");
          PRCS = true;
          EEPROM.write(PRCSAddress, PRCS);
          break;
        }
      case'c': // set setting to false
        {
          //Serial.println(" false");
          PRCS = false;
          EEPROM.write(PRCSAddress, PRCS);
          break;
        }

      case 'd': // effect colorwipe
        {
          //DB Serial.println(" Color Wipe");
          EEPROM.write(CWTSAddress, selectNum());
          break;
        }
      case 'e': // theater chase
        {
          //Serial.println(" Theater Chase");
          EEPROM.write(TCTSAddress, selectNum());
          break;
        }
      case 'f':
        {
          //Serial.println(" Rainbow"); // rainbow
          EEPROM.write(RTSAddress, selectNum());
          break;
        }
      case 'g': // Rainbow chase
        {
          //Serial.println(" Rainbow Theater Chase");
          EEPROM.write(RChTSAddress, selectNum());
          break;
        }
      case 'h': // rainbow cycle
        {
          //Serial.println(" Rainbow Cycle");
          //    RCyTS = selectNum();
          EEPROM.write(RCyTSAddress, selectNum());
          break;
        }
      case 'i':// Block Build
        {
          //Serial.println(" Block Build");
          EEPROM.write(BBTSAddress, selectNum());
          break;
        }
      case 'j': //Cross Fire
        {
          //Serial.println(" Cross Fire");
          EEPROM.write(CFTSAddress, selectNum());
          break;
        }
      case 'k': // Half In
        {
          //Serial.println(" Half In");
          EEPROM.write(HITSAddress, selectNum());
          break;
        }
      case 'l': // Half Out
        {
          //Serial.println(" Half Out");
          EEPROM.write(HOTSAddress, selectNum());
          break;
        }
      case 'm': // Volly
        {
          //Serial.println(" Volly");
          EEPROM.write(VTSAddress, selectNum());
          break;
        }
      case 'n': // default
        {
          //DB  Serial.println(" all effects timmings will be reverted to default");
          EEPROM.write(CWTSAddress, CWTSDefault);
          EEPROM.write(TCTSAddress, TCTSDefault);
          EEPROM.write(RTSAddress, RTSDefault);
          EEPROM.write(RChTSAddress, RChTSDefault);
          EEPROM.write(RCyTSAddress, RCyTSDefault);
          EEPROM.write(BBTSAddress, BBTSDefault);
          EEPROM.write(CFTSAddress, CFTSDefault);
          EEPROM.write(HITSAddress, HITSDefault);
          EEPROM.write(HOTSAddress, HOTSDefault);
          EEPROM.write(VTSAddress, VTSDefault);
          EEPROM.write(VTSAddress, VTSDefault);
          EEPROM.write(STSAddress, STSDefault);
          break;
        }
    }
  }
}

//end get char


boolean checkChar(char testChar)
{
  if (receivedChar == '<' || receivedChar == '!' ||
      (receivedChar >= '0' && receivedChar <= '9') ||
      (receivedChar >= 'A' && receivedChar <= 'Z') || //if inbetwwen A and Z       //order goes numbers, lowercase letters, uppercase letters
      (receivedChar >= 'a' && receivedChar <= 'z'))    //if inbetwwen a and z
    return true;
  else
    return false;
}
void setup()
{
  Serial.begin(9600); //begin serial

  Stick.begin();  //begin led strip from class

  mySerial.begin(9600); //begin bluetooth serial

  randomSeed(analogRead(0)); //set up RNG
}

void loop()
{
  getChar(); // check if there is a new BT char recieved
  Stick.Update(); //update the stick
}





/* all code under is LED effects that have not been implemented into the class


  void theaterChaseRainbow(uint8_t wait)
  {
  for (int j = 0; j < 256; j++)    // cycle all 256 colors in the wheel
  {
    for (int q = 0; q < 3; q++)
    {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
      {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();
      delay(wait);
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
      {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
  }
*/


int selectNum()  //dear god please for the sake of the world clean this god damn method up, like what the hell is half this, damn.

{
  String num = "";
  while (true)
  { 
    while (!mySerial.available());
    {
      receivedChar = mySerial.read();
      Stick.Update();
    }
    if (checkChar(receivedChar))
    {
      if (receivedChar == '<')
      {
        if (num.length() > 0)
        {
          num = num.substring(0, num.length() - 1);
        }
      }
      else if (receivedChar == '!')
      {
        if (num == "")
        {
          return 50;
        }
        else if (num == "1")
        {
          return 1;
        }
        else if (num == "2")
        {
          return 2;
        }
        else if (num == "3")
        {
          return 3;
        }
        else if (num == "4")
        {
          return 4;
        }
        else if (num == "5")
        {
          return 5;
        }
        else if (num == "6")
        {
          return 6;
        }
        else if (num == "7")
        {
          return 7;
        }
        else if (num == "8")
        {
          return 8;
        }
        else if (num == "9")
        {
          return 9;
        }
        else if (num == "10")
        {
          return 10;
        }
        else if (num == "11")
        {
          return 11;
        }
        else if (num == "12")
        {
          return 12;
        }
        else if (num == "13")
        {
          return 13;
        }
        else if (num == "14")
        {
          return 14;
        }
        else if (num == "15")
        {
          return 15;
        }
        else if (num == "16")
        {
          return 16;
        }
        else if (num == "17")
        {
          return 17;
        }
        else if (num == "18")
        {
          return 18;
        }
        else if (num == "19")
        {
          return 19;
        }
        else if (num == "20")
        {
          return 20;
        }
        else if (num == "21")
        {
          return 21;
        }
        else if (num == "22")
        {
          return 22;
        }
        else if (num == "23")
        {
          return 23;
        }
        else if (num == "24")
        {
          return 24;
        }
        else if (num == "25")
        {
          return 25;
        }
        else if (num == "26")
        {
          return 26;
        }
        else if (num == "27")
        {
          return 27;
        }
        else if (num == "28")
        {
          return 28;
        }
        else if (num == "29")
        {
          return 29;
        }
        else if (num == "30")
        {
          return 30;
        }
        else if (num == "31")
        {
          return 31;
        }
        else if (num == "32")
        {
          return 32;
        }
        else if (num == "33")
        {
          return 33;
        }
        else if (num == "34")
        {
          return 34;
        }
        else if (num == "35")
        {
          return 35;
        }
        else if (num == "36")
        {
          return 36;
        }
        else if (num == "37")
        {
          return 37;
        }
        else if (num == "38")
        {
          return 38;
        }
        else if (num == "39")
        {
          return 39;
        }
        else if (num == "40")
        {
          return 40;
        }
        else if (num == "41")
        {
          return 41;
        }
        else if (num == "42")
        {
          return 42;
        }
        else if (num == "43")
        {
          return 43;
        }
        else if (num == "44")
        {
          return 44;
        }
        else if (num == "45")
        {
          return 45;
        }
        else if (num == "46")
        {
          return 46;
        }
        else if (num == "47")
        {
          return 47;
        }
        else if (num == "48")
        {
          return 48;
        }
        else if (num == "49")
        {
          return 49;
        }
        else if (num == "50")
        {
          return 50;
        }
        else if (num == "51")
        {
          return 51;
        }
        else if (num == "52")
        {
          return 52;
        }
        else if (num == "53")
        {
          return 53;
        }
        else if (num == "54")
        {
          return 54;
        }
        else if (num == "55")
        {
          return 55;
        }
        else if (num == "56")
        {
          return 56;
        }
        else if (num == "57")
        {
          return 57;
        }
        else if (num == "58")
        {
          return 58;
        }
        else if (num == "59")
        {
          return 59;
        }
        else if (num == "60")
        {
          return 60;
        }
        else if (num == "61")
        {
          return 61;
        }
        else if (num == "62")
        {
          return 62;
        }
        else if (num == "63")
        {
          return 63;
        }
        else if (num == "64")
        {
          return 64;
        }
        else if (num == "65")
        {
          return 65;
        }
        else if (num == "66")
        {
          return 66;
        }
        else if (num == "67")
        {
          return 67;
        }
        else if (num == "68")
        {
          return 68;
        }
        else if (num == "69")
        {
          return 69;
        }
        else if (num == "70")
        {
          return 70;
        }
        else if (num == "71")
        {
          return 71;
        }
        else if (num == "72")
        {
          return 72;
        }
        else if (num == "73")
        {
          return 73;
        }
        else if (num == "74")
        {
          return 74;
        }
        else if (num == "75")
        {
          return 75;
        }
        else if (num == "76")
        {
          return 76;
        }
        else if (num == "77")
        {
          return 77;
        }
        else if (num == "78")
        {
          return 78;
        }
        else if (num == "79")
        {
          return 79;
        }
        else if (num == "80")
        {
          return 80;
        }
        else if (num == "81")
        {
          return 81;
        }
        else if (num == "82")
        {
          return 82;
        }
        else if (num == "83")
        {
          return 83;
        }
        else if (num == "84")
        {
          return 84;
        }
        else if (num == "85")
        {
          return 85;
        }
        else if (num == "86")
        {
          return 86;
        }
        else if (num == "87")
        {
          return 87;
        }
        else if (num == "88")
        {
          return 88;
        }
        else if (num == "89")
        {
          return 89;
        }
        else if (num == "90")
        {
          return 90;
        }
        else if (num == "91")
        {
          return 91;
        }
        else if (num == "92")
        {
          return 92;
        }
        else if (num == "93")
        {
          return 93;
        }
        else if (num == "94")
        {
          return 94;
        }
        else if (num == "95")
        {
          return 95;
        }
        else if (num == "96")
        {
          return 96;
        }
        else if (num == "97")
        {
          return 97;
        }
        else if (num == "98")
        {
          return 98;
        }
        else if (num == "99")
        {
          return 99;
        }
        else if (num == "100")
        {
          return 100;
        }
        else if (num == "101")
        {
          return 101;
        }
        else if (num == "102")
        {
          return 102;
        }
        else if (num == "103")
        {
          return 103;
        }
        else if (num == "104")
        {
          return 104;
        }
        else if (num == "105")
        {
          return 105;
        }
        else if (num == "106")
        {
          return 106;
        }
        else if (num == "107")
        {
          return 107;
        }
        else if (num == "108")
        {
          return 108;
        }
        else if (num == "109")
        {
          return 109;
        }
        else if (num == "110")
        {
          return 110;
        }
        else if (num == "111")
        {
          return 111;
        }
        else if (num == "112")
        {
          return 112;
        }
        else if (num == "113")
        {
          return 113;
        }
        else if (num == "114")
        {
          return 114;
        }
        else if (num == "115")
        {
          return 115;
        }
        else if (num == "116")
        {
          return 116;
        }
        else if (num == "117")
        {
          return 117;
        }
        else if (num == "118")
        {
          return 118;
        }
        else if (num == "119")
        {
          return 119;
        }
        else if (num == "120")
        {
          return 120;
        }
        else if (num == "121")
        {
          return 121;
        }
        else if (num == "122")
        {
          return 122;
        }
        else if (num == "123")
        {
          return 123;
        }
        else if (num == "124")
        {
          return 124;
        }
        else if (num == "125")
        {
          return 125;
        }
        else if (num == "126")
        {
          return 126;
        }
        else if (num == "127")
        {
          return 127;
        }
        else if (num == "128")
        {
          return 128;
        }
        else if (num == "129")
        {
          return 129;
        }
        else if (num == "130")
        {
          return 130;
        }
        else if (num == "131")
        {
          return 131;
        }
        else if (num == "132")
        {
          return 132;
        }
        else if (num == "133")
        {
          return 133;
        }
        else if (num == "134")
        {
          return 134;
        }
        else if (num == "135")
        {
          return 135;
        }
        else if (num == "136")
        {
          return 136;
        }
        else if (num == "137")
        {
          return 137;
        }
        else if (num == "138")
        {
          return 138;
        }
        else if (num == "139")
        {
          return 139;
        }
        else if (num == "140")
        {
          return 140;
        }
        else if (num == "141")
        {
          return 141;
        }
        else if (num == "142")
        {
          return 142;
        }
        else if (num == "143")
        {
          return 143;
        }
        else if (num == "144")
        {
          return 144;
        }
        else if (num == "145")
        {
          return 145;
        }
        else if (num == "146")
        {
          return 146;
        }
        else if (num == "147")
        {
          return 147;
        }
        else if (num == "148")
        {
          return 148;
        }
        else if (num == "149")
        {
          return 149;
        }
        else if (num == "150")
        {
          return 150;
        }
        else if (num == "151")
        {
          return 151;
        }
        else if (num == "152")
        {
          return 152;
        }
        else if (num == "153")
        {
          return 153;
        }
        else if (num == "154")
        {
          return 154;
        }
        else if (num == "155")
        {
          return 155;
        }
        else if (num == "156")
        {
          return 156;
        }
        else if (num == "157")
        {
          return 157;
        }
        else if (num == "158")
        {
          return 158;
        }
        else if (num == "159")
        {
          return 159;
        }
        else if (num == "160")
        {
          return 160;
        }
        else if (num == "161")
        {
          return 161;
        }
        else if (num == "162")
        {
          return 162;
        }
        else if (num == "163")
        {
          return 163;
        }
        else if (num == "164")
        {
          return 164;
        }
        else if (num == "165")
        {
          return 165;
        }
        else if (num == "166")
        {
          return 166;
        }
        else if (num == "167")
        {
          return 167;
        }
        else if (num == "168")
        {
          return 168;
        }
        else if (num == "169")
        {
          return 169;
        }
        else if (num == "170")
        {
          return 170;
        }
        else if (num == "171")
        {
          return 171;
        }
        else if (num == "172")
        {
          return 172;
        }
        else if (num == "173")
        {
          return 173;
        }
        else if (num == "174")
        {
          return 174;
        }
        else if (num == "175")
        {
          return 175;
        }
        else if (num == "176")
        {
          return 176;
        }
        else if (num == "177")
        {
          return 177;
        }
        else if (num == "178")
        {
          return 178;
        }
        else if (num == "179")
        {
          return 179;
        }
        else if (num == "180")
        {
          return 180;
        }
        else if (num == "181")
        {
          return 181;
        }
        else if (num == "182")
        {
          return 182;
        }
        else if (num == "183")
        {
          return 183;
        }
        else if (num == "184")
        {
          return 184;
        }
        else if (num == "185")
        {
          return 185;
        }
        else if (num == "186")
        {
          return 186;
        }
        else if (num == "187")
        {
          return 187;
        }
        else if (num == "188")
        {
          return 188;
        }
        else if (num == "189")
        {
          return 189;
        }
        else if (num == "190")
        {
          return 190;
        }
        else if (num == "191")
        {
          return 191;
        }
        else if (num == "192")
        {
          return 192;
        }
        else if (num == "193")
        {
          return 193;
        }
        else if (num == "194")
        {
          return 194;
        }
        else if (num == "195")
        {
          return 195;
        }
        else if (num == "196")
        {
          return 196;
        }
        else if (num == "197")
        {
          return 197;
        }
        else if (num == "198")
        {
          return 198;
        }
        else if (num == "199")
        {
          return 199;
        }
        else if (num == "200")
        {
          return 200;
        }
        else if (num == "201")
        {
          return 201;
        }
        else if (num == "202")
        {
          return 202;
        }
        else if (num == "203")
        {
          return 203;
        }
        else if (num == "204")
        {
          return 204;
        }
        else if (num == "205")
        {
          return 205;
        }
        else if (num == "206")
        {
          return 206;
        }
        else if (num == "207")
        {
          return 207;
        }
        else if (num == "208")
        {
          return 208;
        }
        else if (num == "209")
        {
          return 209;
        }
        else if (num == "210")
        {
          return 210;
        }
        else if (num == "211")
        {
          return 211;
        }
        else if (num == "212")
        {
          return 212;
        }
        else if (num == "213")
        {
          return 213;
        }
        else if (num == "214")
        {
          return 214;
        }
        else if (num == "215")
        {
          return 215;
        }
        else if (num == "216")
        {
          return 216;
        }
        else if (num == "217")
        {
          return 217;
        }
        else if (num == "218")
        {
          return 218;
        }
        else if (num == "219")
        {
          return 219;
        }
        else if (num == "220")
        {
          return 220;
        }
        else if (num == "221")
        {
          return 221;
        }
        else if (num == "222")
        {
          return 222;
        }
        else if (num == "223")
        {
          return 223;
        }
        else if (num == "224")
        {
          return 224;
        }
        else  if (num == "225")
        {
          return 225;
        }
        else  if (num == "226")
        {
          return 226;
        }
        else  if (num == "227")
        {
          return 227;
        }
        else  if (num == "228")
        {
          return 228;
        }
        else  if (num == "229")
        {
          return 229;
        }
        else  if (num == "230")
        {
          return 230;
        }
        else  if (num == "231")
        {
          return 231;
        }
        else if (num == "232")
        {
          return 232;
        }
        else   if (num == "233")
        {
          return 233;
        }
        else    if (num == "234")
        {
          return 234;
        }
        else  if (num == "235")
        {
          return 235;
        }
        else   if (num == "236")
        {
          return 236;
        }
        else    if (num == "237")
        {
          return 237;
        }
        else   if (num == "238")
        {
          return 238;
        }
        else    if (num == "239")
        {
          return 239;
        }
        else   if (num == "240")
        {
          return 240;
        }
        else  if (num == "241")
        {
          return 241;
        }
        else if (num == "242")
        {
          return 242;
        }
        else  if (num == "243")
        {
          return 243;
        }
        else  if (num == "244")
        {
          return 244;
        }
        else  if (num == "245")
        {
          return 245;
        }
        else  if (num == "246")
        {
          return 246;
        }
        else   if (num == "247")
        {
          return 247;
        }
        else   if (num == "248")
        {
          return 248;
        }
        else  if (num == "249")
        {
          return 249;
        }
        else    if (num == "250")
        {
          return 250;
        }
        else  if (num == "251")
        {
          return 251;
        }
        else if (num == "252")
        {
          return 252;
        }
        else    if (num == "253")
        {
          return 253;
        }
        else  if (num == "254")
        {
          return 254;
        }
        else  if (num == "255")
        {
          return 255;
        }
      }
      else
      {
        num += receivedChar;
      }
    }
  }
}
