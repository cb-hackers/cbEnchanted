#include <stdio.h>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

const int SCREEN_W = 400;
const int SCREEN_H = 300;

const int KEYCODES_MAX = 228;


int main(int argc, char **argv)
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font = NULL;
	int keycode = 0;
	char keycodestr[20];
	std::string keynamestr;
	std::string KEYCODES[KEYCODES_MAX];
	bool redraw = true;
	bool doexit = false;

	for (int i = 0; i < KEYCODES_MAX; i++) {
		KEYCODES[i] = "-none-";
	}

	KEYCODES[1] = "ALLEGRO_KEY_A";
	KEYCODES[2] = "ALLEGRO_KEY_B";
	KEYCODES[3] = "ALLEGRO_KEY_C";
	KEYCODES[4] = "ALLEGRO_KEY_D";
	KEYCODES[5] = "ALLEGRO_KEY_E";
	KEYCODES[6] = "ALLEGRO_KEY_F";
	KEYCODES[7] = "ALLEGRO_KEY_G";
	KEYCODES[8] = "ALLEGRO_KEY_H";
	KEYCODES[9] = "ALLEGRO_KEY_I";
	KEYCODES[10] = "ALLEGRO_KEY_J";
	KEYCODES[11] = "ALLEGRO_KEY_K";
	KEYCODES[12] = "ALLEGRO_KEY_L";
	KEYCODES[13] = "ALLEGRO_KEY_M";
	KEYCODES[14] = "ALLEGRO_KEY_N";
	KEYCODES[15] = "ALLEGRO_KEY_O";
	KEYCODES[16] = "ALLEGRO_KEY_P";
	KEYCODES[17] = "ALLEGRO_KEY_Q";
	KEYCODES[18] = "ALLEGRO_KEY_R";
	KEYCODES[19] = "ALLEGRO_KEY_S";
	KEYCODES[20] = "ALLEGRO_KEY_T";
	KEYCODES[21] = "ALLEGRO_KEY_U";
	KEYCODES[22] = "ALLEGRO_KEY_V";
	KEYCODES[23] = "ALLEGRO_KEY_W";
	KEYCODES[24] = "ALLEGRO_KEY_X";
	KEYCODES[25] = "ALLEGRO_KEY_Y";
	KEYCODES[26] = "ALLEGRO_KEY_Z";
	KEYCODES[27] = "ALLEGRO_KEY_0";
	KEYCODES[28] = "ALLEGRO_KEY_1";
	KEYCODES[29] = "ALLEGRO_KEY_2";
	KEYCODES[30] = "ALLEGRO_KEY_3";
	KEYCODES[31] = "ALLEGRO_KEY_4";
	KEYCODES[32] = "ALLEGRO_KEY_5";
	KEYCODES[33] = "ALLEGRO_KEY_6";
	KEYCODES[34] = "ALLEGRO_KEY_7";
	KEYCODES[35] = "ALLEGRO_KEY_8";
	KEYCODES[36] = "ALLEGRO_KEY_9";
	KEYCODES[37] = "ALLEGRO_KEY_PAD_0";
	KEYCODES[38] = "ALLEGRO_KEY_PAD_1";
	KEYCODES[39] = "ALLEGRO_KEY_PAD_2";
	KEYCODES[40] = "ALLEGRO_KEY_PAD_3";
	KEYCODES[41] = "ALLEGRO_KEY_PAD_4";
	KEYCODES[42] = "ALLEGRO_KEY_PAD_5";
	KEYCODES[43] = "ALLEGRO_KEY_PAD_6";
	KEYCODES[44] = "ALLEGRO_KEY_PAD_7";
	KEYCODES[45] = "ALLEGRO_KEY_PAD_8";
	KEYCODES[46] = "ALLEGRO_KEY_PAD_9";
	KEYCODES[47] = "ALLEGRO_KEY_F1";
	KEYCODES[48] = "ALLEGRO_KEY_F2";
	KEYCODES[49] = "ALLEGRO_KEY_F3";
	KEYCODES[50] = "ALLEGRO_KEY_F4";
	KEYCODES[51] = "ALLEGRO_KEY_F5";
	KEYCODES[52] = "ALLEGRO_KEY_F6";
	KEYCODES[53] = "ALLEGRO_KEY_F7";
	KEYCODES[54] = "ALLEGRO_KEY_F8";
	KEYCODES[55] = "ALLEGRO_KEY_F9";
	KEYCODES[56] = "ALLEGRO_KEY_F10";
	KEYCODES[57] = "ALLEGRO_KEY_F11";
	KEYCODES[58] = "ALLEGRO_KEY_F12";
	KEYCODES[59] = "ALLEGRO_KEY_ESCAPE";
	KEYCODES[60] = "ALLEGRO_KEY_TILDE";
	KEYCODES[61] = "ALLEGRO_KEY_MINUS";
	KEYCODES[62] = "ALLEGRO_KEY_EQUALS";
	KEYCODES[63] = "ALLEGRO_KEY_BACKSPACE";
	KEYCODES[64] = "ALLEGRO_KEY_TAB";
	KEYCODES[65] = "ALLEGRO_KEY_OPENBRACE";
	KEYCODES[66] = "ALLEGRO_KEY_CLOSEBRACE";
	KEYCODES[67] = "ALLEGRO_KEY_ENTER";
	KEYCODES[68] = "ALLEGRO_KEY_SEMICOLON";
	KEYCODES[69] = "ALLEGRO_KEY_QUOTE";
	KEYCODES[70] = "ALLEGRO_KEY_BACKSLASH";
	KEYCODES[71] = "ALLEGRO_KEY_BACKSLASH2";
	KEYCODES[72] = "ALLEGRO_KEY_COMMA";
	KEYCODES[73] = "ALLEGRO_KEY_FULLSTOP";
	KEYCODES[74] = "ALLEGRO_KEY_SLASH";
	KEYCODES[75] = "ALLEGRO_KEY_SPACE";
	KEYCODES[76] = "ALLEGRO_KEY_INSERT";
	KEYCODES[77] = "ALLEGRO_KEY_DELETE";
	KEYCODES[78] = "ALLEGRO_KEY_HOME";
	KEYCODES[79] = "ALLEGRO_KEY_END";
	KEYCODES[80] = "ALLEGRO_KEY_PGUP";
	KEYCODES[81] = "ALLEGRO_KEY_PGDN";
	KEYCODES[82] = "ALLEGRO_KEY_LEFT";
	KEYCODES[83] = "ALLEGRO_KEY_RIGHT";
	KEYCODES[84] = "ALLEGRO_KEY_UP";
	KEYCODES[85] = "ALLEGRO_KEY_DOWN";
	KEYCODES[86] = "ALLEGRO_KEY_PAD_SLASH";
	KEYCODES[87] = "ALLEGRO_KEY_PAD_ASTERISK";
	KEYCODES[88] = "ALLEGRO_KEY_PAD_MINUS";
	KEYCODES[89] = "ALLEGRO_KEY_PAD_PLUS";
	KEYCODES[90] = "ALLEGRO_KEY_PAD_DELETE";
	KEYCODES[91] = "ALLEGRO_KEY_PAD_ENTER";
	KEYCODES[92] = "ALLEGRO_KEY_PRINTSCREEN";
	KEYCODES[93] = "ALLEGRO_KEY_PAUSE";
	KEYCODES[94] = "ALLEGRO_KEY_ABNT_C1";
	KEYCODES[95] = "ALLEGRO_KEY_YEN";
	KEYCODES[96] = "ALLEGRO_KEY_KANA";
	KEYCODES[97] = "ALLEGRO_KEY_CONVERT";
	KEYCODES[98] = "ALLEGRO_KEY_NOCONVERT";
	KEYCODES[99] = "ALLEGRO_KEY_AT";
	KEYCODES[100] = "ALLEGRO_KEY_CIRCUMFLEX";
	KEYCODES[101] = "ALLEGRO_KEY_COLON2";
	KEYCODES[102] = "ALLEGRO_KEY_KANJI";
	KEYCODES[103] = "ALLEGRO_KEY_PAD_EQUALS";
	KEYCODES[104] = "ALLEGRO_KEY_BACKQUOTE";
	KEYCODES[105] = "ALLEGRO_KEY_SEMICOLON2";
	KEYCODES[106] = "ALLEGRO_KEY_COMMAND";
	KEYCODES[107] = "ALLEGRO_KEY_UNKNOWN";
	KEYCODES[215] = "ALLEGRO_KEY_MODIFIERS";
	KEYCODES[215] = "ALLEGRO_KEY_LSHIFT";
	KEYCODES[216] = "ALLEGRO_KEY_RSHIFT";
	KEYCODES[217] = "ALLEGRO_KEY_LCTRL";
	KEYCODES[218] = "ALLEGRO_KEY_RCTRL";
	KEYCODES[219] = "ALLEGRO_KEY_ALT";
	KEYCODES[220] = "ALLEGRO_KEY_ALTGR";
	KEYCODES[221] = "ALLEGRO_KEY_LWIN";
	KEYCODES[222] = "ALLEGRO_KEY_RWIN";
	KEYCODES[223] = "ALLEGRO_KEY_MENU";
	KEYCODES[224] = "ALLEGRO_KEY_SCROLLLOCK";
	KEYCODES[225] = "ALLEGRO_KEY_NUMLOCK";
	KEYCODES[226] = "ALLEGRO_KEY_CAPSLOCK";



	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	al_init_font_addon(); // initialize the font addon
	al_init_ttf_addon();// initialize the ttf (True Type Font) addon

	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard!\n");
		return -1;
	}

	timer = al_create_timer(1);
	if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	font = al_load_ttf_font("..\\OpenSans-Regular.ttf",16,0);



	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0,0,0));
	al_flip_display();

	al_start_timer(timer);

	while(!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {

		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			keycode = ev.keyboard.keycode;

			if (keycode == ALLEGRO_KEY_ESCAPE) {
				doexit = true;
			}
			redraw = true;
		}

		if(redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			al_clear_to_color(al_map_rgb(0,0,0));

			sprintf(keycodestr, "Keycode is: %d", keycode);
			keynamestr = "Key name is: " + KEYCODES[keycode];
			al_draw_text(font, al_map_rgb(255,255,255), 20, 20, 0, keycodestr);
			al_draw_text(font, al_map_rgb(255,255,255), 20, 40, 0, keynamestr.c_str());
			al_draw_text(font, al_map_rgb(255,255,255), 20, 60, 0, al_keycode_to_name(keycode));

			al_flip_display();
		}
	}

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}
