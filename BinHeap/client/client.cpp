// --------------------------------------------------------
// | Name: Cameron Hildebrandt, 1584696
// | Name: Ramana Vasanthan, 1458497
// | CMPUT 275, Winter 2019
// | Final Project - Data Structure Visualizer
// --------------------------------------------------------

#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <math.h>
using namespace std;

// Initialize the display and assign the corresponding pins
#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 6

// Initializing the Adafruit Display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Initialize the display dimensions
#define DISPLAY_HEIGHT  320
#define DISPLAY_WIDTH 240

// max size of buffer, including null terminator
const uint16_t buf_size = 256;

// current number of chars in buffer, not counting null terminator
uint16_t buf_len = 0;

// input buffer
char* buffer = (char *) malloc(buf_size);
int array[1000];
int size=0;
int index=0;

// Swap function for integer pointers to be used in arrays
void swapnums(int* x,int* y) {
    int temp=*x;
    *x=*y;
    *y=temp;
}

// heapify and heapSort code adapted from geeksforgeeks.org/heap-sort
void heapify(int* tree, int n, int i) {
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
        heapify(tree,n,largest);
    }
}

void heapSort(int* tree, int n) {
    for (int i=n/2-1; i>=0; i--) {
        heapify(tree, n, i);
    }
    for (int i=n-1; i>=0; i--) {
        swapnums(&tree[0],&tree[i]);
        heapify(tree,i,0);
    }
}

// Takes the array sent from the server formatted to fit BinaryHeap ordering and prints it to the display
void printtree(int* formatted, int n) {
    tft.fillScreen(ILI9341_WHITE);

    // level is used to determine the vertical position to print the current part of the tree
    int level=-1;

    // max is the current maximum index at the given level
    int max=-1;

    // prevmax is subtracted from the current index being printed to determine the horizontal location
    int prevmax=-3;

    // xcoord and ycoord are the coordinate to print the value at
    int xcoord,ycoord=0;

    for (int i=0; i<n; i++) {

        // reassign prevmax, level, and max if the current index is larger than the max index
        if (i>max) {
            prevmax=max;
            level++;
            max=max*2+2;
            ycoord=level*40+40;
        }

        // temp calculates the number of values that can be at a given level
        int temp= pow(2,level+1);
        xcoord=abs((i-prevmax)%(temp))*(DISPLAY_WIDTH/(pow(2,level)+1));

        // If there is a left child node, draw a line to it
        if (2*i+1<n) {
            tft.drawLine(xcoord-10,ycoord,xcoord-20, ycoord+30, ILI9341_BLACK);
        }

        // If there is a right child node, draw a line to it
        if (2*i+2<n) {
            tft.drawLine(xcoord+10, ycoord, xcoord+20, ycoord+30, ILI9341_BLACK);
        }

        // Draw the corresponding circle and value at the locations given by xcoord and ycoord
        tft.fillCircle(xcoord,ycoord,12,ILI9341_RED);
        tft.setCursor(xcoord-2, ycoord-5);
        tft.setTextSize(1);
        tft.setTextColor(ILI9341_BLACK);
        tft.print(formatted[i]);
    }
}

void process_line() {
    // N corresponds to the number of values in the binary heap
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

    }

    // E corresponds to the entry to be printed
    if (buffer[0]=='E') {
        int i=2;

        array[index]=0;

        // subtracting 48 is to account for ascii
        array[index]=array[index]*10+buffer[i]-48;
        i++;

        while (buffer[i]>=48 and buffer[i]<=57) {
            array[index]=array[index]*10+buffer[i]-48;
            i++;
        }
        index++;
    }
    if (buffer[0]=='X') {
        // X means the last value was sent so the tree should be printed to the LCD display
        printtree(array,size);

        // After the tree is printed, the array of numbers is sorted via heapSort and printed.
        // The time needed to sort the numbers is found and printed
        int start,finish;
        start=micros();
        heapSort(array,size);
        finish=micros();
        tft.setCursor(0,200);
        tft.setTextColor(ILI9341_BLACK);
        tft.setTextSize(2);
        tft.println("Sorting took:");
        String message=String((finish-start));
        message=message+String(" microseconds");
        tft.println(message);
        tft.println();
        tft.println("The sorted values:");
        for (int i=0; i<size; i++) {
            tft.print(array[i]);
            tft.print(" ");
        }

    }

    // The buffer is cleared and an acknowledgement is sent to the server
    buf_len = 0;
    buffer[buf_len] = 0;
    Serial.println('A');
}

// Initializing the Arduino, serial communication, and the display
void setup() {
    init();
    Serial.begin(9600);
    Serial.flush(); // get rid of any leftover bits
    tft.begin();
    tft.fillScreen(ILI9341_WHITE);
}

int main() {
    // setup is called and a handshake is performed with the server
    setup();
    tft.setCursor(0,0);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(3);
    while(Serial.read()!='x') {
        Serial.println("PHASE01");
        delay(100);
    }
    Serial.println("thanks");

    // Loop to process readings from Serial port adapted from code on eClass
    while (true) {

        if (Serial.available()) {
            // read the incoming byte:
            char in_char = Serial.read();

            // if end of line is received, waiting for line is done:
            if (in_char == '\n' || in_char == '\r') {

                // now we process the buffer
                process_line();
                Serial.flush();
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