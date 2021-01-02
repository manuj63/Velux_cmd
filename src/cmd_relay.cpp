#include <cmd_relay.hpp>
#include <config.h>
#include <Arduino.h> 

hw_timer_t * timer = NULL;

#ifndef REVERSE_CMD_RELAY
void command_shutter(CMD_SHUTTER state)
{
    switch (state)
    {
    case STOP:
        Serial.println("cmd_stop");
        digitalWrite(RELAY_1, LOW);
        digitalWrite(RELAY_2, LOW);
        break;
    case UP:
        Serial.println("cmd_up");
        digitalWrite(RELAY_1, LOW);
        digitalWrite(RELAY_2, HIGH);
        break;
    case DOWN:
        Serial.println("cmd_down");
        digitalWrite(RELAY_1, HIGH);
        digitalWrite(RELAY_2, LOW);
        break;
    default:
        digitalWrite(RELAY_1, LOW);
        digitalWrite(RELAY_2, LOW);
    }
}
#endif

#ifdef REVERSE_CMD_RELAY
void command_shutter(CMD_SHUTTER state)
{
    switch (state)
    {
    case STOP:
        Serial.println("cmd_stop");
        digitalWrite(RELAY_1, HIGH);
        digitalWrite(RELAY_2, HIGH);
        break;
    case UP:
        Serial.println("cmd_up");
        digitalWrite(RELAY_1, HIGH);
        digitalWrite(RELAY_2, LOW);
        break;
    case DOWN:
        Serial.println("cmd_down");
        digitalWrite(RELAY_1, LOW);
        digitalWrite(RELAY_2, HIGH);
        break;
    default:
        digitalWrite(RELAY_1, HIGH);
        digitalWrite(RELAY_2, HIGH);
    }
}
#endif

bool commande_up_pressed = false;

void IRAM_ATTR isr_cmd_up()
{
    Serial.println("isr_cmd_up");
    commande_up_pressed = true;
}

bool commande_down_pressed = false;

void IRAM_ATTR isr_cmd_down()
{
    Serial.println("isr_cmd_down");
    commande_down_pressed = true;
}

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
volatile bool TimerOut = false;

void IRAM_ATTR onTimer() {
    portENTER_CRITICAL_ISR(&timerMux);
    TimerOut = true;
    portEXIT_CRITICAL_ISR(&timerMux);

}

void setup_cmd_shutter(void)
{
    // Relays
    Serial.println("config relay_1");
    pinMode(RELAY_1, OUTPUT);
    Serial.println("config relay_2");
    pinMode(RELAY_2, OUTPUT);

    //Telecommande
    pinMode(COMMAND_UP, INPUT);
    attachInterrupt(COMMAND_UP, isr_cmd_up, FALLING);
    pinMode(COMMAND_DOWN, INPUT);
    attachInterrupt(COMMAND_DOWN, isr_cmd_down, FALLING);

    //Timer
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 30000000, true);

}


void run(void)
{
    static state_shutter state = STATE_STOP;
    switch (state)
    {
    case state_shutter::STATE_STOP:
        if (commande_up_pressed)
        {
            Serial.println("STATE UP");
            timerRestart(timer);
            timerAlarmEnable(timer);
            command_shutter(UP);
            state = STATE_UP;
        }
        if (commande_down_pressed)
        {
            Serial.println("STATE DOWN");
            //timerAlarmWrite(timer, 10000000, false);
            timerRestart(timer);
            timerAlarmEnable(timer);
            command_shutter(DOWN);
            state = STATE_DOWN;
        }
        break;
    case state_shutter::STATE_UP:
        if (commande_up_pressed || commande_down_pressed || TimerOut)
        {
            Serial.println("STATE STOP");
            timerStop(timer);
            timerAlarmDisable(timer);
            command_shutter(STOP);
            state = STATE_STOP;
        }
        break;
    case state_shutter::STATE_DOWN:
        if (commande_up_pressed || commande_down_pressed || TimerOut)
        {
            Serial.println("STATE STOP");
            timerStop(timer);
            timerAlarmDisable(timer);
            command_shutter(STOP);
            state = STATE_STOP;
        }
        break;
    default:
        Serial.println("STATE DEFAULT");
        timerStop(timer);
        timerAlarmDisable(timer);
        command_shutter(STOP);
        state = STATE_STOP;
        break;
    }
    if (TimerOut)
    {
        Serial.println("timeout");
    }
    commande_up_pressed = false;
    commande_down_pressed = false;
    portENTER_CRITICAL(&timerMux);
    TimerOut = false;
    portEXIT_CRITICAL(&timerMux);
}
/*

void loop() {


    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);


    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);

  }
}
*/