#ifndef CMD_RELAY_H
#define CMD_RELAY_H

typedef enum CMD_SHUTTER
{
	STOP = 0,
	UP = 1,
	DOWN = 2
}CMD_SHUTTER;

typedef enum state_shutter
{
	STATE_STOP = 0,
	STATE_UP = 1,
	STATE_DOWN = 2
}state_shutter;

void command_shutter(CMD_SHUTTER state);

void setup_cmd_shutter(void);

void run(void);

#endif