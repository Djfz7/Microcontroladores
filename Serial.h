//#define _115200 3
//#define _57600 8
//#define _38400 12
//#define _19200 25
//#define _9600 51
//#define _4800 103
#define _XTAL_FREQ 32000000UL

void uc_config(void);
void send_string(const unsigned char *vector, unsigned int numero);
void send_char(unsigned char letrita);
void send_newline(void);
