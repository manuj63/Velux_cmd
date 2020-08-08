#ifndef CMD_RELAY_H
#define CMD_RELAY_H

typedef enum CMD_SHUTTER
{
	STOP = 0,
	UP = 1,
	DOWN = 2
}CMD_SHUTTER;

void command_vollet(CMD_SHUTTER state);

#endif