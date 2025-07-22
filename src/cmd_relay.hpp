#ifndef CMD_RELAY_H
#define CMD_RELAY_H

typedef enum CMD_SHUTTER
{
	STOP = 0,
	UP = 1,
	DOWN = 2,
	STOP_AND_DOWN = 3,
	STOP_AND_UP = 4,
	INIT = 9
}CMD_SHUTTER;

typedef enum state_shutter
{
	STATE_STOP = 0,
	STATE_UP = 1,
	STATE_DOWN = 2,
	STATE_INIT = 9,
}state_shutter;

void command_shutter(CMD_SHUTTER state);

void setup_cmd_shutter(void);

void run(void);

void set_up(void);
void set_down(void);
void set_stop(void);
void set_init_motor(void);

state_shutter get_state(void);

#endif