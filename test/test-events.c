#include <stdio.h>
#include <linux/input.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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

int main() {
	int fd;
	struct input_event ie;
	
	fd =  open("/dev/input/event18",0);
	
	if (fd <= 0) {
		printf("can't open file\n");
		return 0;
	}
	
	while (1) {
		size_t count = read(fd, &ie, sizeof(struct input_event));
		if (sizeof(struct input_event) ==  count) {
			char buffer[5];
			sprintf(buffer,"%x",ie.code);
			printf("[%16s] code [%16s] value\t[%16d]\n",
				types[ie.type],
				codes[ie.type].c[ie.code] ? : buffer,
				ie.value);
		} else {
			printf("read: %lu bytes\n",count);
		}
	}
	return 0;
}
