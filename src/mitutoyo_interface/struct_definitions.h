#ifndef STRUCT_DEFINITIONS_H
#define STRUCT_DEFINITIONS_H
typedef struct{
    byte raw_data[13];
    bool acknowledge;
    long number;
    int sign;
    int decimal;
    int units;
    int good_reading;
    } mitutoyo_interface;
#endif