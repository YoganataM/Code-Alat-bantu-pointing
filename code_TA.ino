#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <MPU6050_light.h>
#include <QMC5883LCompass.h>

static const int RXPin = 9, TXPin = 10;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps; // The TinyGPS++ object
SoftwareSerial ss(RXPin, TXPin); // The serial connection to the GPS device
 
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'-', '0', '.', 'D'}
};
byte rowPins[ROWS] = {0,2,3,4};
byte colPins[COLS] = {5,6,7,8};
 
Keypad costumkeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
 
LiquidCrystal_I2C lcd(0x27, 20, 4);

byte derajat[8] = {
  0b00110,
  0b01001,
  0b01001,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte sudut[8] = {
  B00000,
  B00000,
  B00001,
  B00010,
  B00100,
  B01100,
  B10100,
  B11111
};

byte antena[8] = {
  B00000,
  B10000,
  B10010,
  B11100,
  B01100,
  B10111,
  B10000,
  B10000
};

MPU6050 mpu(Wire);
QMC5883LCompass compass;
int x_min, x_max, y_min, y_max, z_min, z_max;
int calibrationData[3][2];
bool changed = false;
bool done = false;
int t = 0;
int c = 0;
String sat, stroffsetpol,strlatant, strlongant,strdeclin,strlongsat;
char stp;
enum {ke1,ke2,ke3,ke4,ke5,ke6,ke7,ke8,ke9,ke10,ke11,ke12,ke13,ke14,ke15,ke16,ke17,ke18,ke185,ke19,ke20,ke21};
int i;
float phi = 3.1418, r = 180, azi, az, ax, axx, bx, bxx, cx, cxx, byy, by, cyyy, cyy, cy, ayy, ay, e, el;
float longsat, longant, latant, longsat1,longant1, latant1, offsetpol, declin;
 
void setup()
{
  stroffsetpol.reserve(5);
  strlatant.reserve(5); 
  strlongant.reserve(5);
  strdeclin.reserve(5);
  strlongsat.reserve(5);

  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, derajat);
  lcd.createChar(1, sudut);
  lcd.createChar(2, antena);

  lcd.setCursor(1, 0); lcd.print(F("VSAT Pointing Tool")); 
  lcd.setCursor(0,1); lcd.print(F("====================")); 
  lcd.setCursor(0,2); lcd.print(F("Yoganata Mundi D"));
  lcd.setCursor(0,3); lcd.print(F("1102170227"));  
  delay(1500);
  lcd.clear();

  ss.begin(GPSBaud);

  byte status = mpu.begin();
  while(status!=0){ }

  compass.init();
  
}
 
void loop()
{
  char key = costumkeypad.getKey();
  if(key){
  }

  while (ss.available() > 0){
    gps.encode(ss.read());
    }

  mpu.update();

  float a;
  compass.read(); // Read compass values
  
  switch(stp){
      case ke1:
        if (key=='A'){
             i--; //i>0; i<6;
             if(i<1){
                i=6;
              }
             delay(300);
             lcd.clear();
          }
         if (key=='B'){
             i++; //i>0; i<6;
             if(i>6){
                i=1;
              }
             delay(300);
             lcd.clear();
          }

         switch(i){
            case 0:
              lcd.setCursor(0, 0);lcd.print(F("TARGET SATELIT"));
              lcd.setCursor(0, 1);lcd.print(F("  BRISAT"));
              lcd.setCursor(0, 2);lcd.print(F("  TELKOM 3S"));
              lcd.setCursor(0, 3);lcd.print(F("  TELKOM 2"));
            break;
            case 1:
              lcd.setCursor(0, 0);lcd.print(F("TARGET SATELIT"));
              lcd.setCursor(0, 1);lcd.print(F("> BRISAT"));
              lcd.setCursor(0, 2);lcd.print(F("  TELKOM 3S"));
              lcd.setCursor(0, 3);lcd.print(F("  TELKOM 2"));
              if(key=='D'){
                  stp = ke2;
                  lcd.clear();
                  sat="BRISAT";
                  longsat=150.4;
                }
            break;
            case 2:
              lcd.setCursor(0, 0);lcd.print(F("TARGET SATELIT"));
              lcd.setCursor(0, 1);lcd.print(F("  BRISAT"));
              lcd.setCursor(0, 2);lcd.print(F("> TELKOM 3S"));
              lcd.setCursor(0, 3);lcd.print(F("  TELKOM 2"));     
              if(key=='D'){
                  stp = ke2;
                  lcd.clear();
                  sat="TELKOM 3S";
                  longsat=118;
                }
            break;
            case 3:
              lcd.setCursor(0, 0);lcd.print(F("TARGET SATELIT"));
              lcd.setCursor(0, 1);lcd.print(F("  TELKOM 3S"));
              lcd.setCursor(0, 2);lcd.print(F("> TELKOM 2"));
              lcd.setCursor(0, 3);lcd.print(F("  SAT X"));
              if(key=='D'){
                  stp = ke2;
                  lcd.clear();
                  sat="TELKOM 2";
                  longsat=157;
                }
            break;
            case 4:
              lcd.setCursor(0, 0);lcd.print(F("  TELKOM 3S"));
              lcd.setCursor(0, 1);lcd.print(F("  TELKOM 2"));
              lcd.setCursor(0, 2);lcd.print(F("> SAT X"));
              lcd.setCursor(0, 3);lcd.print(F("  Kalibrasi Kompas"));
              if(key=='D'){
                  stp = ke18;
                  lcd.clear();
                  sat="X";
                }
            break;
            case 5:
              lcd.setCursor(0, 0);lcd.print(F("  TELKOM 2"));
              lcd.setCursor(0, 1);lcd.print(F("  SAT X"));
              lcd.setCursor(0, 2);lcd.print(F("> Kalibrasi Kompas"));
              lcd.setCursor(0, 3);lcd.print(F("  Cek Sudut"));
              if(key=='D'){
                  stp = ke14;
                  lcd.clear();
                }
            break;
            case 6:
              lcd.setCursor(0, 0);lcd.print(F("  TELKOM 2"));
              lcd.setCursor(0, 1);lcd.print(F("  SAT X"));
              lcd.setCursor(0, 2);lcd.print(F("  Kalibrasi Kompas"));
              lcd.setCursor(0, 3);lcd.print(F("> Cek Sudut"));
              if(key=='D'){
                  stp = ke21;
                  lcd.clear();
                }
            break;
          }
      break; 
      
      case ke2:
        lcd.setCursor(0, 0); lcd.print(F("SATELIT :"));
        lcd.setCursor(0, 1); lcd.print(sat);
        lcd.setCursor(0, 2); lcd.print(F("LONG:"));
        lcd.setCursor(7, 2); lcd.print(longsat);lcd.write(byte(0)); 
        lcd.setCursor(0, 3); lcd.print(F("<C"));
        lcd.setCursor(18, 3); lcd.print(F("D>"));

        if(key=='C'){
            stp = ke1;
            lcd.clear();
            strlongsat="";
            longsat=0 , sat="";
          }
        if(key=='D'){
            stp = ke3;
            i=0;
            lcd.clear();
          }
      break;
      
      case ke3:
        lcd.setCursor(0, 0); lcd.print("SUDUT OFFSET POLAR");
        lcd.setCursor(0, 1); lcd.print("Offset : "); lcd.setCursor(9, 1); lcd.print(stroffsetpol); lcd.write(byte(0));
        //lcd.setCursor(0, 2); lcd.write(byte(1)); lcd.print(" :"); lcd.print(abs(mpu.getAngleX()));  lcd.write(byte(0));lcd.print(" ");
        lcd.setCursor(0, 3); lcd.print("<C");
        lcd.setCursor(18, 3); lcd.print("D>");
        
        if(key>= '0' && key <= '9' || key == '.' || key == '-'){
            stroffsetpol += key;
            lcd.setCursor(9, 1); lcd.print(stroffsetpol); lcd.write(byte(0));
          }

        if(key=='C'){
            stp = ke2;
            stroffsetpol="";
            lcd.clear();
          }
        if(key=='D'){
            offsetpol = stroffsetpol.toFloat();
            stp = ke17;
            lcd.clear();
          }
        if(key=='B'){ 
            mpu.calcOffsets();
            stp = ke3;
            lcd.clear(); 
          }
        if(key=='A'){
            stp = ke3;
            stroffsetpol="";
            lcd.clear();
          }
      break;

          
      case ke4:
        if (key=='A'){
             i--; //i>0; i<6;
             if(i<1){
                i=2;
              }
             delay(300);
             lcd.clear();
          }
         if (key=='B'){
             i++; //i>0; i<6;
             if(i>2){
                i=1;
              }
             delay(300);
             lcd.clear();
          }
         switch(i){
              case 0:
                lcd.setCursor(0, 0); lcd.print(F("KOORDINAT ANTENA"));
                lcd.setCursor(0, 1); lcd.print(F("Menentukan :"));
                lcd.setCursor(0, 2); lcd.print(F("  Otomatis"));
                lcd.setCursor(0, 3); lcd.print(F("  Manual"));
                if(key=='C'){
                 stp = ke17;
                 lcd.clear();
                }
              break;
              case 1:
                lcd.setCursor(0, 0); lcd.print(F("KOORDINAT ANTENA"));
                lcd.setCursor(0, 1); lcd.print(F("Menentukan :"));
                lcd.setCursor(0, 2); lcd.print(F("> Otomatis"));
                lcd.setCursor(0, 3); lcd.print(F("  Manual"));
                if(key=='D'){
                  stp = ke5;
                  lcd.clear();
                }
                if(key=='C'){
                 stp = ke17;
                 lcd.clear();
                }
              break;
              case 2:
                lcd.setCursor(0, 0); lcd.print(F("KOORDINAT ANTENA"));
                lcd.setCursor(0, 1); lcd.print(F("Menentukan :"));
                lcd.setCursor(0, 2); lcd.print(F("  Otomatis"));
                lcd.setCursor(0, 3); lcd.print(F("> Manual"));
                if(key=='D'){
                  stp = ke6;
                  lcd.clear();
                }
                if(key=='C'){
                 stp = ke17;
                 lcd.clear();
                }
              break;
            }
      break;

      case ke5:
        compass.read();
        lcd.setCursor(0, 0); lcd.print(F("MENENTUKAN KOORDINAT"));
        lcd.setCursor(0, 1); lcd.print(F("Lat:"));
        lcd.setCursor(0, 2); lcd.print(F("Long:"));
        lcd.setCursor(0, 3); lcd.print(F("<C"));
        lcd.setCursor(18, 3); lcd.print(F("D>"));

        if (gps.location.isUpdated()){
            lcd.setCursor(5, 1); lcd.print(gps.location.lat(), 5); lcd.write(byte(0));
            lcd.setCursor(6, 2); lcd.print(gps.location.lng(), 5); lcd.write(byte(0));
            }  
        else if (gps.location.isUpdated()== 0){
            lcd.setCursor(3, 3); lcd.print(F("GPS Scaning"));
          }  
        if(key=='C'){
            stp = ke4;
            lcd.clear();
          }  
        if(key=='D'){
            latant = gps.location.lat();
            longant = gps.location.lng();
            stp = ke9;
            lcd.clear();
          }
      break;
      
      case ke6:
        lcd.setCursor(0, 0); lcd.print(F("MENENTUKAN KOORDINAT"));
        lcd.setCursor(0, 1); lcd.print(F("Lat:"));
        lcd.setCursor(0, 2); lcd.print(F("Long:"));
        lcd.setCursor(0, 3); lcd.print(F("<C"));
        lcd.setCursor(18, 3); lcd.print(F("D>"));
      
        if(key=='C'){
            stp = ke4;
            strlatant = "";
            lcd.clear();
          }
        if(key=='D'){
            latant = strlatant.toFloat();
            stp = ke7;
            lcd.clear();
          }
        if(key=='A'){
            stp = ke6;
            strlatant = "";
            lcd.clear();
          }
        if(key>= '0' && key <= '9' || key == '.' || key == '-'){
            strlatant += key;
            lcd.setCursor(4, 1); lcd.print(strlatant); lcd.write(byte(0));
          }
      break;
      
      case ke7:
        lcd.setCursor(0, 0); lcd.print(F("MENENTUKAN KOORDINAT"));
        lcd.setCursor(0, 1); lcd.print(F("Lat: ")); lcd.print(latant); lcd.write(byte(0));
        lcd.setCursor(0, 2); lcd.print(F("Long: "));
        lcd.setCursor(0, 3); lcd.print(F("<C"));
        lcd.setCursor(18, 3); lcd.print(F("D>"));
      
        if(key=='C'){
            stp = ke6;
            strlongant="";
            lcd.clear();
          }
        if(key=='D'){
            longant = strlongant.toFloat();
            stp = ke8;
            lcd.clear();
          }
        if(key=='A'){
            stp = ke7;
            strlongant="";
            lcd.clear();
          }
        if(key>= '0' && key <= '9' || key == '.' || key == '-'){
            strlongant += key;
            lcd.setCursor(6, 2); lcd.print(strlongant); lcd.write(byte(0));
          }
      break;
      
      case ke8:
        lcd.setCursor(0, 0); lcd.print(F("KOORDINAT DITENTUKAN"));
        lcd.setCursor(0, 1); lcd.print(F("Lat: ")); lcd.print(latant); lcd.write(byte(0));
        lcd.setCursor(0, 2); lcd.print(F("Long: ")); lcd.print(longant); lcd.write(byte(0));
        lcd.setCursor(0, 3); lcd.print(F("<C"));
        lcd.setCursor(6, 3); lcd.print(F("Pointing?"));
        lcd.setCursor(18, 3); lcd.print(F("D>"));
      
        if(key=='C'){
            stp = ke7;
            strlongant="";
            longant=0;
            lcd.clear();
          }
        if(key=='D'){
            stp = ke10;
            lcd.clear();
          }
      break;
      
      case ke9:
        lcd.setCursor(0, 0); lcd.print(F("KOORDINAT DITENTUKAN"));
        lcd.setCursor(0, 1); lcd.print(F("Lat: ")); lcd.print(latant); lcd.write(byte(0));
        lcd.setCursor(0, 2); lcd.print(F("Long: ")); lcd.print(longant); lcd.write(byte(0));
        lcd.setCursor(0, 3); lcd.print(F("<C"));
        lcd.setCursor(6, 3); lcd.print(F("Pointing?"));
        lcd.setCursor(18, 3); lcd.print(F("D>"));
      
        if(key=='C'){
            stp = ke5;
            strlatant = "";
            strlongant="";
            latant=0;
            longant=0;
            lcd.clear();
          }
        if(key=='D'){
            stp = ke11;
            lcd.clear();
          }
      break;

      case ke10:
        longsat1=longsat*phi/r;
        longant1=longant*phi/r; 
        latant1=latant*phi/r;
        cxx=abs(longant1-longsat1);
        cx=sin(latant1);
        bxx=tan(cxx);
        bx=bxx;
        axx=bx/cx;
        ax=atan(axx);
        
        if (longant<=longsat){
            if(latant<=0){
                az=abs(ax)*r/phi;
                azi=az-declin;
              }
             if(latant>=0){
                az=(180-abs(ax)*r/phi);
                azi=az-declin;
              }    
          }
        if (longant>=longsat){
            if(latant<=0){
                az=(360-abs(ax)*r/phi);
                azi=az-declin;
              }
             if(latant>=0){
                az=(180+abs(ax)*r/phi);
                azi=az-declin;
              }  
          }
          
        if(x_min==0 && x_max==0 && y_min==0 && y_max==0 && z_min==0 && z_max==0){
          compass.init();
          a = compass.getAzimuth();
          }
        else{
          compass.init();
          compass.setCalibration(x_min, x_max, y_min, y_max, z_min, z_max);
          a = compass.getAzimuth();
          }
        delay(250);

        lcd.setCursor(0, 0); lcd.print(F("Azimut : "));lcd.print(az); lcd.write(byte(0));lcd.print(" ");
        lcd.setCursor(0, 1); lcd.write(byte(1)); lcd.print(F(" Deklinasi:"));lcd.print(declin); lcd.write(byte(0));lcd.print(" ");
        lcd.setCursor(0, 2); lcd.print(F("Target : ")); lcd.print(azi); lcd.write(byte(0));lcd.print(" ");
        lcd.setCursor(0, 3); lcd.print(F("Azimut ")); lcd.write(byte(2)); lcd.print(" :"); lcd.print(a); lcd.write(byte(0));lcd.print(" ");

        if(key=='C'){
            stp = ke8;
            lcd.clear();
          }
        if(key=='D'){
            stp = ke12;
            lcd.clear();
          }
      break;

      case ke11:
        longsat1=longsat*phi/r;
        longant1=longant*phi/r; 
        latant1=latant*phi/r;
        cxx=abs(longant1-longsat1);
        cx=sin(latant1);
        bxx=tan(cxx);
        bx=bxx;
        axx=bx/cx;
        ax=atan(axx);

        if (longant<=longsat){
            if(latant<=0){
                az=abs(ax)*r/phi;
                azi=az-declin;
              }
             if(latant>=0){
                az=(180-abs(ax)*r/phi);
                azi=az-declin;
              }    
          }
        if (longant>=longsat){
            if(latant<=0){
                az=(360-abs(ax)*r/phi);
                azi=az-declin;
              }
             if(latant>=0){
                az=(180+abs(ax)*r/phi);
                azi=az-declin;
              }  
          }
          
        if(x_min==0 && x_max==0 && y_min==0 && y_max==0 && z_min==0 && z_max==0){
          compass.init();
          a = compass.getAzimuth();
          }
        else{
          compass.init();
          compass.setCalibration(x_min, x_max, y_min, y_max, z_min, z_max);
          a = compass.getAzimuth();
          }
        delay(250);

        lcd.setCursor(0, 0); lcd.print(F("Azimut : "));lcd.print(az); lcd.write(byte(0));lcd.print(" ");
        lcd.setCursor(0, 1); lcd.write(byte(1)); lcd.print(F(" Deklinasi:"));lcd.print(declin); lcd.write(byte(0));lcd.print(" ");
        lcd.setCursor(0, 2); lcd.print(F("Target : ")); lcd.print(azi); lcd.write(byte(0));lcd.print(" ");
        lcd.setCursor(0, 3); lcd.print(F("Azimut ")); lcd.write(byte(2)); lcd.print(" :"); lcd.print(a); lcd.write(byte(0));lcd.print(" ");
        if(key=='C'){
            stp = ke9;
            lcd.clear();
          }
        if(key=='D'){
            stp = ke13;
            lcd.clear();
          }
      break;

      case ke12:
        byy=(cos(latant1)*cos(cxx));
        by=byy-0.151;
        cyyy=pow(byy,2);
        cyy=1-cyyy;
        cy=sqrt(cyy);
        ayy=by/cy;
        ay=atan(ayy);
        e=ay*r/phi;
        el=e-offsetpol;

        lcd.setCursor(0, 0); lcd.print(F("Elevasi : "));lcd.print(e); lcd.write(byte(0));lcd.print(" ");
        lcd.setCursor(0, 1); lcd.write(byte(1)); lcd.print(F(" Offset:"));lcd.print(offsetpol); lcd.write(byte(0));lcd.print(" ");
        lcd.setCursor(0, 2); lcd.print(F("Target : ")); lcd.print(el); lcd.write(byte(0));lcd.print(" ");
        lcd.setCursor(0, 3); lcd.print(F("Elevasi ")); lcd.write(byte(2)); lcd.print(" :"); lcd.print(abs(mpu.getAngleX())); lcd.write(byte(0));lcd.print(" ");

        if(key=='B'){ 
            mpu.calcOffsets();
            stp = ke12;
            lcd.clear();
            }
        if(key=='C'){
            stp = ke10;
            lcd.clear();
          }
        if(key=='D'){
            lcd.clear();
            lcd.setCursor(2, 1); lcd.print(F("Poiting selesai"));
            lcd.setCursor(0, 3); lcd.print(F("<C"));
            lcd.setCursor(18, 3); lcd.print(F("D>"));
            delay (3000);
            stroffsetpol="";
            strlatant="";
            strlongant="";
            strdeclin="";
            strlongsat="";
            longsat=0;
            i=0;
            stp = ke1; 
            lcd.clear();
          }
      break; 

      case ke13:
        byy=(cos(latant1)*cos(cxx));
        by=byy-0.151;
        cyyy=pow(byy,2);
        cyy=1-cyyy;
        cy=sqrt(cyy);
        ayy=by/cy;
        ay=atan(ayy);
        e=ay*r/phi;
        el=e-offsetpol;

        lcd.setCursor(0, 0); lcd.print(F("Elevasi : "));lcd.print(e); lcd.write(byte(0));lcd.print(" ");
        lcd.setCursor(0, 1); lcd.write(byte(1)); lcd.print(F(" Offset:"));lcd.print(offsetpol); lcd.write(byte(0));lcd.print(" ");
        lcd.setCursor(0, 2); lcd.print(F("Target : ")); lcd.print(el); lcd.write(byte(0));lcd.print(" ");
        lcd.setCursor(0, 3); lcd.print(F("Elevasi ")); lcd.write(byte(2)); lcd.print(" :"); lcd.print(abs(mpu.getAngleX())); lcd.write(byte(0));lcd.print(" ");
        
        if(key=='B'){ 
            mpu.calcOffsets();
            stp = ke13;
            lcd.clear();
            }
        if(key=='C'){
            stp = ke11;
            lcd.clear();
          }
        if(key=='D'){
            lcd.clear();
            lcd.setCursor(2, 1); lcd.print(F("Poiting selesai"));
            lcd.setCursor(0, 3); lcd.print(F("<C"));
            lcd.setCursor(18, 3); lcd.print(F("D>"));
            delay (3000);
            stroffsetpol="";
            strlatant="";
            strlongant="";
            strdeclin="";
            strlongsat="";
            longsat=0;
            i=0;
            stp = ke1; 
            lcd.clear();
          }
      break;
      
      case ke14:
        compass.init();
        a = compass.getAzimuth(); // Return Azimuth reading
        delay(250);

        lcd.setCursor(0, 0);lcd.print(F("Kalibrasi?A=")); lcd.print(a); lcd.write(byte(0));lcd.print(" ");
        lcd.setCursor(0, 1);lcd.print(F("-+x= ")); lcd.print(x_min); lcd.print(" "); lcd.print(x_max);
        lcd.setCursor(0, 2);lcd.print(F("-+y= ")); lcd.print(y_min); lcd.print(" "); lcd.print(y_max);
        lcd.setCursor(0, 3);lcd.print(F("-+z= ")); lcd.print(z_min); lcd.print(" "); lcd.print(z_max);
              
        if(key=='D'){
          stp=ke15;
          calibrationData[0][0]=0;
          calibrationData[0][1]=0;
          calibrationData[1][0]=0;
          calibrationData[1][1]=0;
          calibrationData[2][0]=0;
          calibrationData[2][1]=0;
          done=false;
          lcd.clear();
          }
        if(key=='C'){
            stp = ke1;
            lcd.clear();
          }
      break;
      
      case ke15:
        compass.init();
        lcd.setCursor(0, 0);lcd.print(F("Kalibrasi Kompas"));
        lcd.setCursor(0, 1);lcd.print(F("dimulai dalam 5 sec")); 
        delay (5000);

         int xc, yc, zc;
         compass.read(); // Read compass values

         // Return XYZ readings
         xc = compass.getX();
         yc = compass.getY();
         zc = compass.getZ();

         changed = false;

         if(xc < calibrationData[0][0]) {
              calibrationData[0][0] = xc;
              changed = true;
              }
         if(xc > calibrationData[0][1]) {
              calibrationData[0][1] = xc;
              changed = true;
              }
         if(yc < calibrationData[1][0]) {
             calibrationData[1][0] = yc;
             changed = true;
             }   
         if(yc > calibrationData[1][1]) {
            calibrationData[1][1] = yc;
            changed = true;
             }
          if(zc < calibrationData[2][0]) {
            calibrationData[2][0] = zc;
            changed = true;
            }
          if(zc > calibrationData[2][1]) {
            calibrationData[2][1] = zc;
            changed = true;
            }
          if (changed && !done) {
            lcd.setCursor(0, 2);lcd.print(F("Keep moving"));
            c = millis();
            }
            t = millis();

          if ( (t - c > 5000) && !done) {
            done = true;
            lcd.setCursor(0, 3);lcd.print(F("CALIBRATING done"));
            x_min = calibrationData[0][0];
            x_max = calibrationData[0][1];
            y_min = calibrationData[1][0];
            y_max = calibrationData[1][1];
            z_min = calibrationData[2][0];
            z_max = calibrationData[2][1];
            delay (3000);
            stp = ke16;
            lcd.clear();
            }
      break;
      
      case ke16:
        compass.init();
        compass.setCalibration(x_min, x_max, y_min, y_max, z_min, z_max);
        a = compass.getAzimuth();
        lcd.setCursor(0, 0);lcd.print(F("Hasil Klbr A="));lcd.print(a); lcd.write(byte(0));lcd.print(" ");
        lcd.setCursor(0, 1);lcd.print(F("-+x= ")); lcd.print(x_min); lcd.print(" "); lcd.print(x_max);
        lcd.setCursor(0, 2);lcd.print(F("-+y= ")); lcd.print(y_min); lcd.print(" "); lcd.print(y_max);
        lcd.setCursor(0, 3);lcd.print(F("-+z= ")); lcd.print(z_min); lcd.print(" "); lcd.print(z_max);
        
        if(key=='D'){
            stp=ke1;
            done=false;
            lcd.clear();
            } 
        if(key=='C'){
            stp = ke15;
            calibrationData[0][0]=0;
            calibrationData[0][1]=0;
            calibrationData[1][0]=0;
            calibrationData[1][1]=0;
            calibrationData[2][0]=0;
            calibrationData[2][1]=0;
            done=false;
            lcd.clear();
          }    
      break;
      
      case ke17:
        lcd.setCursor(0, 0); lcd.print(F("SUDUT DEKLINASI"));
        lcd.setCursor(0, 1); lcd.print(F("Deklinasi : "));
        lcd.setCursor(12, 1); lcd.print(strdeclin); lcd.write(byte(0));
        lcd.setCursor(0, 3); lcd.print(F("<C"));
        lcd.setCursor(18, 3); lcd.print(F("D>"));

        if(key=='C'){
            stp = ke3;
            strdeclin="";
            lcd.clear();
          }
        if(key=='D'){
            declin = strdeclin.toFloat();
            stp = ke4;
            lcd.clear();
          }
        if(key>= '0' && key <= '9' || key == '.' || key == '-'){
            strdeclin += key;
            lcd.setCursor(12, 1); lcd.print(strdeclin); lcd.write(byte(0));
          }
        if(key=='A'){
            stp = ke17;
            strdeclin="";
            lcd.clear();
          }
      break;

      case ke18:
        lcd.setCursor(0, 0); lcd.print(F("SATELIT :"));
        lcd.setCursor(0, 1); lcd.print(sat);
        lcd.setCursor(0, 2); lcd.print(F("LONG:"));
        lcd.setCursor(0, 3); lcd.print(F("<C"));
        lcd.setCursor(18, 3); lcd.print(F("D>"));
        
        if(key=='A'){
            stp = ke18;
            strlongsat="";
            lcd.clear();
          }
        if(key=='C'){
            stp = ke1;
            strlongsat="";
            sat="";
            lcd.clear();
          }
        if(key=='D'){
            longsat = strlongsat.toFloat();
            i=0;
            stp = ke185;
            lcd.clear();
          }
        if(key>= '0' && key <= '9' || key == '.' || key == '-'){
            strlongsat += key;
            lcd.setCursor(6, 2); lcd.print(strlongsat); lcd.write(byte(0));
          }
      break;

      case ke185:
        lcd.setCursor(0, 0); lcd.print(F("SATELIT :"));
        lcd.setCursor(0, 1); lcd.print(sat);
        lcd.setCursor(0, 2); lcd.print(F("LONG:"));
        lcd.setCursor(7, 2); lcd.print(strlongsat);lcd.write(byte(0)); 
        lcd.setCursor(0, 3); lcd.print(F("<C"));
        lcd.setCursor(5, 3); lcd.print(F("Sudah Benar?"));
        lcd.setCursor(18, 3); lcd.print(F("D>"));

        if(key=='C'){
            stp = ke18;
            strlongsat="";
            lcd.clear();
          }
        if(key=='D'){
            stp = ke19;
            lcd.clear();
          }
      break;

      case ke19:
        lcd.setCursor(0, 0); lcd.print(F("SUDUT OFFSET POLAR"));
        lcd.setCursor(0, 1); lcd.print(F("Offset : ")); lcd.setCursor(9, 1); lcd.print(stroffsetpol); lcd.write(byte(0));
        //lcd.setCursor(0, 2); lcd.write(byte(1)); lcd.print(" :"); lcd.print(abs(mpu.getAngleX()));  lcd.write(byte(0));lcd.print(" ");
        lcd.setCursor(0, 3); lcd.print(F("<C"));
        lcd.setCursor(18, 3); lcd.print(F("D>"));
        
        if(key>= '0' && key <= '9' || key == '.' || key == '-'){
            stroffsetpol += key;
            lcd.setCursor(9, 1); lcd.print(stroffsetpol); lcd.write(byte(0));
          }
        if(key=='C'){
            stp = ke2;
            stroffsetpol="";
            lcd.clear();
          }
        if(key=='D'){
            offsetpol = stroffsetpol.toFloat();
            stp = ke20;
            lcd.clear();
          }
        if(key=='B'){ 
            mpu.calcOffsets();
            stp = ke19;
            lcd.clear(); 
          }
        if(key=='A'){
            stp = ke19;
            stroffsetpol="";
            lcd.clear();
          }
      break;

      case ke20:
        lcd.setCursor(0, 0); lcd.print(F("SUDUT DEKLINASI")); 
        lcd.setCursor(0, 1); lcd.print(F("Deklinasi : "));lcd.print(strdeclin); lcd.write(byte(0));
        lcd.setCursor(0, 3); lcd.print(F("<C"));
        lcd.setCursor(18, 3); lcd.print(F("D>"));
        
        if(key>= '0' && key <= '9' || key == '.' || key == '-'){
            strdeclin += key;
            lcd.setCursor(12, 1); lcd.print(strdeclin); lcd.write(byte(0));
          }
        if(key=='C'){
            stp = ke19;
            strdeclin="";
            declin = 0;
            lcd.clear();
          }
        if(key=='D'){
            declin = strdeclin.toFloat();
            stp = ke4;
            i=0;
            lcd.clear();
          }
        if(key=='A'){
            stp = ke20;
            strdeclin="";
            lcd.clear();
          }
      break;

      case ke21:
        lcd.setCursor(0, 0); lcd.print(F("Cek Sudut Kemiringan"));
        //lcd.setCursor(0, 1); lcd.print("Offset : "); lcd.setCursor(9, 1); lcd.print(stroffsetpol); lcd.write(byte(0));
        lcd.setCursor(0, 1); lcd.write(byte(1)); lcd.print(" :"); lcd.print(abs(mpu.getAngleX()));  lcd.write(byte(0));lcd.print(" ");
        lcd.setCursor(0, 3); lcd.print(F("<C"));
        lcd.setCursor(5, 3); lcd.print(F("B = set 0"));lcd.write(byte(0));

        if(key=='D'){
             stp = ke1;
             lcd.clear();
           }
         if(key=='C'){
             stp = ke1;
             lcd.clear();
           }
         if(key=='B'){ 
            mpu.calcOffsets();
            stp = ke21;
            lcd.clear(); 
          }
      break;
  }
}
