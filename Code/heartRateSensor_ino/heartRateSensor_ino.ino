#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// If using software SPI (the default case): SPI: 10 (SS), 11 (MOSI), 12 (MISO), 13 (SCK). 
#define OLED_MOSI   11
#define OLED_CLK   13
#define OLED_DC    7
#define OLED_CS    10
#define OLED_RESET 8
#define pulsePin  0                 // Pulse Sensor purple wire connected to analog pin 0

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 64 
#define LOGO16_GLCD_WIDTH  128 

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
int xPosition = 30;
int yPosition =0;
int lastYPosition=0;

// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, must be seeded! 
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.



void setup(){
  interruptSetup(); 
  pinMode(pulsePin,INPUT);
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(800);
 
  // Clear the buffer.
  display.clearDisplay();
  display.display();
  Serial.begin(115200);             // we agree to talk fast!
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS 
  //initial OLED font  
  display.setTextSize(2.2);
  display.setTextColor(WHITE);  
  
}


void loop(){
  showPulse();
  showBPM();
  //sendDataToProcessing('S', Signal);     // send Processing the raw Pulse Sensor data
  // if (QS == true){                       // Quantified Self flag is true when arduino finds a heartbeat
  //    showBPM();
  //      //  fadeRate = 255;                  // Set 'fadeRate' Variable to 255 to fade LED with pulse
  //      //  sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefix
  //      //  sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix
  // }
       
  //   QS = false;             // reset the Quantified Self flag for next time 
 
  

}

void showBPM(){
  display.setCursor(0,0);
  display.drawRect(0, 0, 40, 14, BLACK);
  display.fillRect(0, 0, 40, 14, BLACK);
  display.println(BPM);

}

void showPulse(){
  lastYPosition = yPosition  ;
  yPosition = map(analogRead(pulsePin),0,1023,63,0);
  xPosition = xPosition +1;
  if( xPosition > 128){
    xPosition = 30;
    display.clearDisplay();
     }
  display.drawLine(xPosition-1, lastYPosition, xPosition, yPosition, WHITE);
  display.display();
}
