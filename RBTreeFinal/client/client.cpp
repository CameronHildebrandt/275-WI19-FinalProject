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

// max size of buffer, including null terminator
const uint16_t buf_size = 256;
// current number of chars in buffer, not counting null terminator
uint16_t buf_len = 0;
bool numcheck=false;
// input buffer
char* buffer = (char *) malloc(buf_size);
//int array[1000];
//int colour[1000];
int index=0;
int level=0;
int horizontal=0;
int num=0;
bool colour=false;
void printtree() {

    int xcoord,ycoord=0;

    ycoord=level+1*40;
    int temp= pow(2,level+1);
    xcoord=(horizontal+1)*(DISPLAY_WIDTH/(pow(2,level)+1));
    if (colour) {
        tft.fillCircle(xcoord, ycoord, 12, ILI9341_RED);
    }
    else {
        tft.fillCircle(xcoord,ycoord, 12, ILI9341_BLACK);
    }
    tft.setCursor(xcoord - 2, ycoord - 5);
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_WHITE);
    tft.print(num);
}

void process_line() {

    if (buffer[0]>=48 and buffer[0]<=57) {
        if (numcheck) {
            numcheck=false;
            if (buffer[0]==49) {
                colour=true;
            }
            if (buffer[0]==48) {
                colour=false;
            }
            printtree();
            level++;
            buf_len = 0;
            buffer[buf_len] = 0;
            Serial.println('A');

        }
        else {
            int i = 0;
            num=0;
            while (buffer[i] >= 48 and buffer[i] <= 57) {
                num = num * 10 + buffer[i] - 48;
                i++;

            }
            tft.setCursor(0,0);
            tft.setTextColor(ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(num);
            numcheck=true;

            buf_len = 0;
            buffer[buf_len] = 0;
            Serial.println("A");
        }
    }
    if (buffer[0]=='R') {
        horizontal++;

        buf_len = 0;
        buffer[buf_len] = 0;
        Serial.println('A');
    }
    if (buffer[0]=='U') {
        level--;

        buf_len = 0;
        buffer[buf_len] = 0;
        Serial.println('A');
//        int a,b;
//        a=micros();
//        heapSort(array,size);
//        b=micros();
//        tft.setCursor(0,200);
//        tft.setTextColor(ILI9341_BLACK);
//        tft.setTextSize(2);
//        tft.println("Sorting took:");
//        String message=String((b-a));
//        message=message+String(" microseconds");
//        tft.println(message);
//        tft.println();
//        tft.println("The sorted values:");
//        for (int i=0; i<size; i++) {
//            tft.print(array[i]);
//            tft.print(" ");
//        }

    }
}

void setup() {
    init();
    Serial.begin(9600);
    Serial.flush(); // get rid of any leftover bits
    tft.begin();
    tft.fillScreen(ILI9341_WHITE);
    randomSeed(analogRead(0));
}

int main() {

    setup();
    tft.setCursor(0,0);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(3);
    while(Serial.read()!='x') {
        Serial.println("PHASE01");
        delay(100);
    }
    Serial.println("thanks");


    while (true) {

        if (Serial.available()) {
            // read the incoming byte:
            char in_char = Serial.read();

            // if end of line is received, waiting for line is done:
            if (in_char == '\n' || in_char == '\r') {

                // now we process the buffer
                process_line();
                Serial.flush();
//                Serial.println('A');
            } else {
                // add character to buffer, provided that we don't overflow.
                // drop any excess characters1.
                if (buf_len < buf_size - 1) {
                    buffer[buf_len] = in_char;
                    buf_len++;
                    buffer[buf_len] = 0;
                }
            }
        }
    }
}