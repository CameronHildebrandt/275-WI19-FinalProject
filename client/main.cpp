#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <math.h>
using namespace std;

#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 6

// touch screen pins, obtained from the documentaion
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM  5  // can be a digital pin
#define XP  4  // can be a digital pin

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define DISPLAY_HEIGHT  320
#define DISPLAY_WIDTH 240

// calibration data for the touch screen, obtained from documentation
// the minimum/maximum possible readings from the touch point
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

// thresholds to determine if there was a touch
#define MINPRESSURE   10
#define MAXPRESSURE 1000

void setup() {
    init();
    tft.begin();
    tft.fillScreen(ILI9341_WHITE);
}

int main() {
    setup();
    int n=12;
    int level=-1;
    int max=-1;
    int prevmax=-3;
    int xcoord,ycoord=0;
    for (int i=0; i<n; i++) {
        if (i>max) {
            prevmax=max;
            level++;
            max=max*2+2;
            ycoord=level*40+40;
        }
        int temp= pow(2,level+1);
        xcoord=abs((i-prevmax)%(temp))*(DISPLAY_WIDTH/(pow(2,level)+1));
        tft.fillCircle(xcoord,ycoord,12,ILI9341_RED);
    }

//    tft.fillCircle(DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2, 40, ILI9341_RED);
//    tft.drawLine(DISPLAY_WIDTH/2-20, DISPLAY_HEIGHT/2+20,DISPLAY_WIDTH/2-40,DISPLAY_HEIGHT/2+40, ILI9341_BLACK);
}