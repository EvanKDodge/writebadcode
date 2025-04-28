#include <stdint.h>

#define MAX_LINE_LEN 256

typedef struct Hack_t {
    int16_t RAM[0x6001]; // RAM (0x0000-0x3FFF)
                          // screen (0x4000-0x5FFF)
                          // keyboard (0x6000)
    int16_t ROM[0x8000]; // ROM (0x0000-0x7FFF)
    int16_t PC;
    int16_t A;
    int16_t D;
} Hack;

void init(Hack*);
void loadROM(char*, Hack*);
void runHack(Hack*);
