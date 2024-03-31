#include <RX480E.hpp>
#include <config.h>
#include <Arduino.h> 

void setup_RX480E(void)
{
    Serial.println("Init TX");
    pinMode(KEY, OUTPUT);
    digitalWrite(KEY, LOW);

}


void reset_RX480E(void)
{
    Serial.println("Reset TX");
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(KEY, HIGH);
        delay(500);
        digitalWrite(KEY, LOW);
        delay(500);
    }
}

void pairing_RX480E(void)
{
    digitalWrite(KEY, HIGH);
    delay(500);
    digitalWrite(KEY, LOW);
}