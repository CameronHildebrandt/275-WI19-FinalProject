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

// input buffer
char* buffer = (char *) malloc(buf_size);
int array[1000];
int colour[1000];
int size=0;
int index=0;

void swapnums(int* x,int* y) {
    int temp=*x;
    *x=*y;
    *y=temp;
}
void binheaparray(int* tree, int n, int i) {
    int largest=i;
    int left=2*i+1;
    int right=2*i+2;
    if (left<n and tree[left]>tree[largest]) {
        largest=left;
    }
    if (right<n and tree[right]>tree[largest]) {
        largest=right;
    }
    if (largest!=i) {
        swapnums(&tree[i], &tree[largest]);
        binheaparray(tree,n,largest);
    }
}
void heapSort(int* tree, int n) {
    for (int i=n/2-1; i>=0; i--) {
        binheaparray(tree, n, i);
    }
    for (int i=n-1; i>=0; i--) {
        swapnums(&tree[0],&tree[i]);
        binheaparray(tree,i,0);
    }
}
void printtree(int* formatted, int n) {
    tft.fillScreen(ILI9341_WHITE);
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
        if (formatted[i]!=0) {
            if (2 * i + 1 < n) {
                if (formatted[2 * i + 1] != 0) {
                    tft.drawLine(xcoord - 10, ycoord, xcoord - 20, ycoord + 30, ILI9341_BLACK);
                }
            }
            if (2 * i + 2 < n) {
                if (formatted[2 * i + 2] != 0) {
                    tft.drawLine(xcoord + 10, ycoord, xcoord + 20, ycoord + 30, ILI9341_BLACK);
                }
                if (colour[i]==0) {
                    tft.fillCircle(xcoord, ycoord, 12, ILI9341_RED);
                }
                else {
                    tft.fillCircle(xcoord,ycoord, 12, ILI9341_BLACK);
                }
                tft.setCursor(xcoord - 2, ycoord - 5);
                tft.setTextSize(1);
                tft.setTextColor(ILI9341_WHITE);
                tft.print(formatted[i]);
            }
        }
    }
}

void process_line() {
    if (buffer[0]=='N') {

        index=0;
        int i=2;
        size=0;
        size=size*10+buffer[i]-48;
        i++;
        while (buffer[i]>=48 and buffer[i]<=57) {
            size=size*10+buffer[i]-48;
            i++;
        }
        Serial.println('A');
        buf_len = 0;
        buffer[buf_len] = 0;
    }
    if (buffer[0]=='E') {
        int i=2;

        array[index]=0;
        array[index]=array[index]*10+buffer[i]-48;
        i++;

        while (buffer[i]>=48 and buffer[i]<=57) {
            array[index]=array[index]*10+buffer[i]-48;
            i++;
        }
        i++;
        colour[index]=buffer[i]-48;
        index++;
        Serial.flush();
        Serial.println('A');
        buf_len = 0;
        buffer[buf_len] = 0;
    }
    if (buffer[0]=='X') {
        printtree(array,size);
        Serial.println('A');
        buf_len = 0;
        buffer[buf_len] = 0;
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