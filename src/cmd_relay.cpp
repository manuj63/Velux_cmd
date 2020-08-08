#include <cmd_relay.hpp>
#include <config.h>
#include <Arduino.h> 


void command_vollet(CMD_SHUTTER state)
{
    switch (state)
    {
    case STOP:
        digitalWrite(RELAY_1, LOW);
        digitalWrite(RELAY_2, LOW);
        break;
    case UP:
        digitalWrite(RELAY_1, LOW);
        digitalWrite(RELAY_2, HIGH);
        break;
    case DOWN:
        digitalWrite(RELAY_1, HIGH);
        digitalWrite(RELAY_2, LOW);
        break;
    default:
        digitalWrite(RELAY_1, LOW);
        digitalWrite(RELAY_2, LOW);
    }
}
