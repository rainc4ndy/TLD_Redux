
/*
   Some voodoo here.

   Whenever a button is pressed or released, we increment count by 1.

   After every cheat_hook() call, and if count is > 0, we decrement
   count by one, and invert the flip bit.

   This gives us a very simple key queue, which can be used to
   emulate the old KEY_DOWN/UP/PRESSED/RELEASED macros.
*/

#ifndef B87A0970_AEA1_415B_ADBF_22FBEE5C554C
#define B87A0970_AEA1_415B_ADBF_22FBEE5C554C

#include <Windows.h>
#include <stdint.h>

struct keycombo
{
	unsigned int count;
	uint8_t key[128];
};

struct key_state
{
	uint8_t count : 5;
	uint8_t flip : 1;
	uint8_t pstate : 1;		/* previous state (1 down, 0 up) */
	uint8_t consume : 1;	/* KEY_CONSUME(vkey) */
};

#define KEY_DOWN			keyhook_key_down
#define KEY_UP				keyhook_key_up
#define KEY_PRESSED			keyhook_key_pressed
#define KEY_RELEASED		keyhook_key_released
#define KEY_CONSUME			keyhook_key_consume

#define KEYCOMBO_DOWN		keyhook_keycombo_down
#define KEYCOMBO_UP			keyhook_keycombo_up
#define KEYCOMBO_PRESSED	keyhook_keycombo_pressed
#define KEYCOMBO_RELEASED	keyhook_keycombo_released

void keyhook_process ( HWND wnd, UINT umsg, WPARAM wparam, LPARAM lparam );

void					keyhook_run ( void );

int						keyhook_key_down ( int v );
int						keyhook_key_up ( int v );
int						keyhook_key_pressed ( int v );
int						keyhook_key_released ( int v );
void					keyhook_key_consume ( int v );
void					keyhook_clear_states ( void );

bool					keyhook_keycombo_down(const keycombo &comb);
bool					keyhook_keycombo_up(const keycombo &comb);
bool					keyhook_keycombo_pressed(const keycombo &comb);
bool					keyhook_keycombo_released(const keycombo &comb);

extern struct key_state key_table[256];
extern int				key_being_pressed;

#endif /* B87A0970_AEA1_415B_ADBF_22FBEE5C554C */
