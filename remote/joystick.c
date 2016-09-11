#include <stdio.h>
#include <linux/input.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

char *types[] = {
	"EV_SYN",
	"EV_KEY",
	"EV_REL",
	"EV_ABS",
	"EV_MSC"
	"EV_SW"
	"","","","","","","","","","","","",
	"EV_LED",
	"EV_SND",
	"EV_REP",
	"EV_FF",
	"EV_PWR",
	"EV_FF_STATUS",
};

typedef struct {
	char *c[20];
} codes_t;

codes_t codes[] = {
	{ /* EV_SYN */
		"SYN_REPORT",
		"SYN_CONFIG",
		"SYN_MT_REPORT",
		"SYN_DROPPED",
	}, 
	{}, // EV_KEY
	{
		"REL_X",
		"REL_Y",
		"REL_Z",
		"REL_RX",
		"REL_RY",
		"REL_RZ",
		"REL_HWHEEL",
		"REL_DIAL",
		"REL_WHEEL",
		"REL_MISC",
	}, // EV_REL
	{},
	{
		"MSC_SERIAL",
		"MSC_PULSELED",
		"MSC_GESTURE",
		"MSC_RAW",
		"MSC_SCAN",
		"MSC_TIMESTAMP",	
	}, // MISC
};

#define REL_IDLE_MAX     (2)
#define REL_ACTIVE_MIN   (4)


// NOTE:
// joystick to full right: 
//	REL_X: 2 -> 7 (7 -> 2 releasing it)
//	REL_Y: stable to -1

// joystick to down-right:
//	REL_X: 2 -> 7
//	REL_Y: 1 -> 7

// joystick to full down: 
//	REL_X: stable to -2
//	REL_Y: 2 -> 7 (7 -> 2 releasing it)

// joystick to down-left:
//	REL_X: -2 -> -8
//	REL_Y: 2 -> 7

// joystick to full left: 
//	REL_X: -5 -> -8 (-85 -> -5 releasing it)
//	REL_Y: stable to -1


// joystick to up-left:
//	REL_X: -4 -> -7
//	REL_Y: -5 -> -8

// joystick to full up: 
//	REL_X: stable to -2
//	REL_Y: -5 -> -8 (-8 -> -5 releasing it)

// joystick to up-right:
//	REL_X: 1 -> 5
//	REL_Y: -5 -> -8

void event_dump(struct input_event *ie) {
	char buffer[5];	
	sprintf(buffer,"%x",ie->code);
	fprintf(stderr,"[%16s] code [%16s] value\t[%16d]\n",
		types[ie->type],
		codes[ie->type].c[ie->code] ? : buffer,
		ie->value);
}

int value_is_idle(struct input_event *ie) {
	return (ie->value <= REL_IDLE_MAX && ie->value >= -REL_IDLE_MAX);
}

int value_is_pos(struct input_event *ie) {
	return  ie->value >= REL_ACTIVE_MIN;
}

int value_is_neg(struct input_event *ie) {
	return ie->value <= -REL_ACTIVE_MIN;
}

int main() {
	int fd;
	struct input_event event, 
		rel_x, 
		rel_y;
	char last = '5';
	
	fd =  open("/dev/input/event18",0);
	
	if (fd <= 0) {
		fprintf(stderr,"can't open file\n");
		return 0;
	}
	
	while (1) {
		size_t count = read(fd, &event, sizeof(struct input_event));
		if (sizeof(struct input_event) ==  count) {	
#ifdef DEBUG
			event_dump(&event);
#endif
			if (EV_REL == event.type) {
				char cur;
				switch ( event.code ) {
				case REL_X:
					memcpy(&rel_x,&event,sizeof(struct input_event));
					break;
				case REL_Y:
					memcpy(&rel_y,&event,sizeof(struct input_event));
					break;
				default:
					break;
				}
				if ( value_is_pos(&rel_x) && value_is_idle(&rel_y) ) {
					cur = '6';
				} else if ( value_is_pos(&rel_x) && value_is_pos(&rel_y) ) {
					cur = '3';
				} else if ( value_is_idle(&rel_x) && value_is_pos(&rel_y) ) {
					cur = '2';
				} else if ( value_is_neg(&rel_x) && value_is_pos(&rel_y) ) {
					cur = '1';
				} else if ( value_is_neg(&rel_x) && value_is_idle(&rel_y) ) {
					cur = '4';
				} else if ( value_is_neg(&rel_x) && value_is_neg(&rel_y) ) {
					cur = '7';
				} else if ( value_is_idle(&rel_x) && value_is_neg(&rel_y) ) {
					cur = '8';
				} else if ( value_is_pos(&rel_x) && value_is_neg(&rel_y) ) {
					cur = '9';
				} else if ( value_is_idle(&rel_x) && value_is_idle(&rel_y) ) {
					cur = '5';
				} else {
					cur = '5';
				}
				if (cur != last) {
					putchar(cur);
					last = cur;
				}
				fflush(stdout);
			}
		} else {
#ifdef DEBUG
			fprintf(stderr,"read: %lu bytes\n",count);
#endif
		}
	}
	return 0;
}
