#include "scpi_comm.h"
#include "struct_definitions.h"

mitutoyo_interface _interface = {.raw_data = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                 .acknowledge = 0, .number = 0L, .sign = 0,
                                 .decimal = 0, .units = 0, .good_reading = 0};

int req = 5; //mic REQ line goes to pin 5 through q1 (arduino high pulls request line low)
int dat = 2; //mic Data line goes to pin 2
int clk = 3; //mic Clock line goes to pin 3
int led = 13;


void setup() {
    Serial.begin(9600);
    pinMode(req, OUTPUT);
    pinMode(clk, INPUT_PULLUP);
    pinMode(dat, INPUT_PULLUP);
    pinMode(led, OUTPUT);
    digitalWrite(req, LOW); // set request at LOW
}

void loop() { // get data from mic
    if(_interface.good_reading == 0)
    {
        // if our last reading was bad, try to recover by waiting for a bit.
        digitalWrite(req, LOW);
        digitalWrite(led, LOW);
        delay(10);
    }
    digitalWrite(req, HIGH); // generate set request
    digitalWrite(led, HIGH);
    _interface.good_reading = 1;
    for(int i = 0; i < 13; i++ ) {
        int k = 0;
        for (int j = 0; j < 4; j++) {
            while( digitalRead(clk) == LOW) { } // hold until clock is high
            while( digitalRead(clk) == HIGH) { } // hold until clock is low
            bitWrite(k, j, (digitalRead(dat) & 0x1)); // read data bits, and reverse order
        }
        // if the
        // error correction: if the first four bytes aren't 15, restart.
        if( i<4 && k != 15)
        {
            //digitalWrite(req, LOW);
            //digitalWrite(led, LOW);
            _interface.good_reading = 0;
        }
        if(_interface.good_reading){
            // extract data
            _interface.raw_data[i] = k;
        }
    }
    _interface.sign = _interface.raw_data[4];
    _interface.decimal = _interface.raw_data[12];
    _interface.units = _interface.raw_data[11];
    // assemble measurement from bytes
    char buf[7];
    for(int lp = 0;lp < 6; lp++ )
    {
        buf[lp]=_interface.raw_data[lp+5]+'0';
    }
    buf[6] = 0;
    _interface.number = atol(buf);
    if (Serial.available() > 0) {
        comm_protocol(Serial.read(), &_interface);
    }
}