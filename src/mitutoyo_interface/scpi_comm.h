#ifndef SCPI_COMM_H
#define SCPI_COMM_H

#include "struct_definitions.h"

char terminationByte = '\r';
int incomingByte = 0;
int string_pos = 0;
int current_state = 1;
char line[15];
char serialsecond[5];
char input_str[8];
char output_str[8];


void comm_protocol(byte incomingByte, mitutoyo_interface *interface){
   line[string_pos] = incomingByte;
   string_pos += 1;
   if(incomingByte == terminationByte)
   {
        if(interface->acknowledge==1)
        {
            Serial.write(line);
        }
        if(strncmp(line, ":ACKN", 5)==0)
        {
            char * loc = strchr(line, ' ');
            memcpy(serialsecond, loc+1, 3);
            if(strncmp(serialsecond, "ON", 2) == 0)
            {
                interface->acknowledge = 1;
            }
            else if(strncmp(serialsecond, "OF", 2) == 0)
            {
                interface->acknowledge = 0;
            }
        }
        else if(strncmp(line, "DECI", 4)==0 && strpbrk(line, "?") != 0){
            sprintf(output_str, "%d%c", interface->decimal, terminationByte);
            Serial.write(output_str);
        }
        else if(strncmp(line, "GOOD", 4)==0 && strpbrk(line, "?") != 0){
            sprintf(output_str, "%d%c", interface->good_reading, terminationByte);
            Serial.write(output_str);
        }
        else if(strncmp(line, "RAWD", 4)==0 && strpbrk(line, "?") != 0){
            //String raw_data_str = String((char*)interface->raw_data);
            //Serial.print(raw_data_str);
            for(int i=0;i<13;i++)
            {
                sprintf(output_str, "%d,", interface->raw_data[i]);
                Serial.write(output_str);
            }
            sprintf(output_str, "%c", terminationByte);
            Serial.write(output_str);
        }
        else if(strncmp(line, "READ", 4)==0 && strpbrk(line, "?") != 0){
            float reading = interface->number/1000.0;
            if(interface->sign == 8)
            {
                reading = -reading;
            }
            if(interface->units == 3)
            {
                sprintf(output_str, " mm%c", terminationByte);
            }
            else{
                reading = reading/10.0;
                sprintf(output_str, " thou%c", terminationByte);
            }
            Serial.print(reading, 4);
            Serial.write(output_str);
        }
        else if(strncmp(line, "SIGN", 4)==0 && strpbrk(line, "?") != 0){
            sprintf(output_str, "%d%c", interface->sign==0,
            terminationByte);
            Serial.write(output_str);
        }
        else if(strncmp(line, "UNIT", 4)==0 && strpbrk(line, "?") != 0){
            sprintf(output_str, "%d%c", interface->units==3,
            terminationByte);
            Serial.write(output_str);
        }
        else{
            sprintf(output_str, "Unknown%c", terminationByte);
            Serial.write(output_str);
        }
     // reset the string
     string_pos = 0;
     line[0] = '\0';
   }
}
#endif

