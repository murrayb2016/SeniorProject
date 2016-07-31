
//battery lvl
int   battery=A4;
int readvalue;
float Volts;

//temp

int sensorPin = A0;

//LED with Force Sensor

#include "LPD8806.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif

// Example to control LPD8806-based RGB LED Modules in a strip

/*****************************************************************************/
//Force Sensor
int fsrAnalogPin = 1; // FSR is connected to analog 0
int fsrReading;      // the analog reading from the FSR resistor divider


// Number of RGB LEDs in strand:
int nLEDs = 32;

// two pins for output
int dataPin  = 46;
int clockPin = 44;


LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

//button to prompt alcohol sensor

const int button2Pin = 24;

//Breathalyzer
const int mq3Pin = A2;
const int button1Pin = 22;// The output from the MQ3 alcohol sensor goes into analog pin A0 of the arduino

//const int frequency = 450;
// initialize the library with the numbers of the interface pins


int value; // Holds the analog value from the MQ3 sensor
double percentage; // stores the percentage of alcohol in blood
double bac; // stores the calculated Blood Alcohol Content value

// Used for timing a 5 second long blow form the user
int startTime;
int currentTime;

boolean first = true; // compensate for humidity



// include the library code:
#include <LiquidCrystal.h>



// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void setup() {

   lcd.begin(16, 2);
  // put your setup code here, to run once:
   pinMode(battery,INPUT);

   #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

   Serial.begin(9600);  
  // Start up the LED strip
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
  

//alcohol prompt button set up
pinMode(button2Pin, INPUT);
 pinMode(button1Pin, INPUT);


}

void loop() {



      
  fsrReading = analogRead(fsrAnalogPin);
 // Serial.print("Analog reading = ");
  //Serial.println(fsrReading);
  //delay(100);

  if(fsrReading < 600)
  {



  //rainbow(10);
  rainbowCycle(0);  // make it go through the cycle fairly fast
  //theaterChaseRainbow(50);
  for (int i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0);
      strip.show();
  }

  }


      lcd.clear();


  int button2State = digitalRead(button2Pin);
  if(button2State == HIGH)  //button for breathalyzer
  {
  
/***********************************************************************************************************/

  lcd.setCursor(0,1);                      //part of breathalyzer prompting
  lcd.print("Press B2 for BAC");

    //getting the voltage reading from the temperature sensor
 int reading = analogRead(sensorPin); 
 int temp=0;
for(int i = 0;i<10;i++)
{
  temp+=reading;
}
reading=temp/10;
 
 // converting that reading to voltage, for 3.3v arduino use 3.3
 float voltage = reading * 4.3;


 
 voltage /= 1023.0; 
 
 // print out the voltage
 //Serial.print(voltage); Serial.println(" volts");
 
 // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
 //Serial.print(temperatureC); Serial.println(" degrees C");
 
 // now convert to Fahrenheit
 float temperatureF = ((temperatureC * 9.0 / 5.0) + 32.0);


 
 lcd.setCursor(0, 0);
 lcd.print(temperatureF);  lcd.print(" F");
 
  /*****************************************************************************************************************/
    readvalue = analogRead(battery); //give  4.0 V from Buck
    Volts = (readvalue/238.0); //increases accuracy and does the voltage conversion
    //lcd.print(Volts); 
    //lcd.setCursor(6, 1);
    //lcd.print(char(255));
    if(Volts >= 4.0)  //12.8
      {
        for(int x = 11;x<=15;x++)
        {
          lcd.setCursor(x, 0);
          lcd.print(char(255));
       
        }
      }

      else if(Volts <= 4.0 && Volts >= 3.8)  ///12.6
      {
        for(int x = 11;x<=14;x++)
        {
          lcd.setCursor(x, 0);
          lcd.print(char(255));
   
        }
      }

      else if(Volts <= 3.8 && Volts >= 3.6)  ///12.4
      {
        for(int x = 11;x<=13;x++)
        {
          lcd.setCursor(x, 0);
          lcd.print(char(255));
         
        }
      }

      else if(Volts <= 3.6 && Volts >= 3.4)  ///12.2
      {
        for(int x = 11;x<=12;x++)
        {
          lcd.setCursor(x, 0);
          lcd.print(char(255));
         
        }
      }
      else if(Volts <= 3.4 && Volts >= 3.2)  ///12.0
      {
     
          lcd.setCursor(11, 0);
          lcd.print(char(255));
         
        
      }
      else  //< 12.0
      {
        lcd.setCursor(10, 0);
        lcd.print("Charge");
       
      }

      delay(100);
  }
  /*******************************************************************/

while ( button2State == LOW)
{
 //Serial.println("Press Button To Start");  
  lcd.setCursor(0,0);
 lcd.print("B1 To Start");

   int button1State = digitalRead(button1Pin);

   
   if (button1State == LOW) 
{
  
  lcd.setCursor(0,1);
 lcd.print("BLOW NOW");
 
  bac = readBAC();

  
     //value = analogRead(mq3Pin); // JUST FOR TESTING
    // Serial.println(value);

  //
  // Print output String and assume that any BAC reading below 0.01 is really 0 because the sensor doesn't always drop to absolute 0 when testing.
  //
  if (bac > 0.02) {
  
     //Serial.println(String("BAC RESULT: ") + bac);
     
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print(String("BAC: ") + bac);
     delay(10000);
     break;
      

  } else {

 // Serial.println("NO ALCOHOL PRESENT");
 lcd.clear();
     lcd.setCursor(0,0);
 lcd.print("NO BAC");
 delay(10000);
 break;
  }
 
 }


}
}
  

void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 384; j++) {     // 3 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 384));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  
  for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 384-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 384 / strip.numPixels()) + j) % 384) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Fill the dots progressively along the strip.
void colorWipe(uint32_t c, uint8_t wait) {
  int i;

  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// Chase one dot down the full strip.
void colorChase(uint32_t c, uint8_t wait) {
  int i;

  // Start by turning all pixels off:
  for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);

  // Then display one pixel at a time:
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
    delay(wait);
  }

  strip.show(); // Refresh to turn off last pixel
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 384; j++) {     // cycle all 384 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 384));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}


//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r

uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      b = 127 - WheelPos % 128;  //blue down 
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break; 
  }
  return(strip.Color(r,g,b));
}

double readBAC() {

  Serial.println("BLOW INTO SENSOR");
  startTime = millis(); // begin timer
  currentTime = startTime;

  while (currentTime - startTime < 5000) {

    value = analogRead(mq3Pin); //reads the analaog value from the alcohol sensor's AOUT pin

    percentage = value / 1023.0;
    bac = percentage * 0.21; // calculate BAC

    currentTime = millis();
  } // end while
 
  
  return bac;
}
    
    
      
 
