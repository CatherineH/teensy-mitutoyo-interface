#include "scpi_comm.h"
#include "struct_definitions.h"

mitutoyo_interface _interface = {.acknowledge = 0, .number = 0L, .sign = 0,
                                 .decimal = 0, .units = 0};

int req = 5; //mic REQ line goes to pin 5 through q1 (arduino high pulls request line low)
int dat = 2; //mic Data line goes to pin 2
int clk = 3; //mic Clock line goes to pin 3


byte mydata[13];

void setup() {
    Serial.begin(9600);
    pinMode(req, OUTPUT);
    pinMode(clk, INPUT_PULLUP);
    pinMode(dat, INPUT_PULLUP);
    digitalWrite(req, LOW); // set request at LOW
}

void loop() { // get data from mic
    digitalWrite(req, HIGH); // generate set request
    for(int i = 0; i < 13; i++ ) {
        int k = 0;
        for (int j = 0; j < 4; j++) {
            while( digitalRead(clk) == LOW) { } // hold until clock is high
            while( digitalRead(clk) == HIGH) { } // hold until clock is low
            bitWrite(k, j, (digitalRead(dat) & 0x1)); // read data bits, and reverse order
        }
        // extract data
        mydata[i] = k;
        _interface.sign = mydata[4];
        _interface.decimal = mydata[12];
        _interface.units = mydata[11];
    }

    // assemble measurement from bytes
    char buf[7];
    for(int lp = 0;lp < 6; lp++ )
    {
        buf[lp]=mydata[lp+5]+'0';
    }
    buf[6] = 0;
    _interface.number = atol(buf);
    if (Serial.available() > 0) {
        comm_protocol(Serial.read(), &_interface);
    }
}