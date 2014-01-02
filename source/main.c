typedef volatile unsigned int vu32;
typedef unsigned short u16;
typedef volatile unsigned char vu8;

#define REG_DISPCNT_MAIN  (*(vu32*)0x04000000)
#define REG_DISPCNT_SUB   (*(vu32*)0x04001000)
#define MODE_FB0    (0x00020000)
#define VRAM_A        ((u16*)0x6800000)
#define VRAM_A_CR         (*(vu8*)0x04000240)
#define VRAM_ENABLE       (1<<7)

#define COLOR(r,g,b) ((r) | (g)<<5 | (b)<<10)
#define PIXEL_ENABLE (1<<15)

int main(void) {

    REG_DISPCNT_MAIN = MODE_FB0;
    REG_DISPCNT_SUB= MODE_FB0; //TODO no worky

    VRAM_A_CR = VRAM_ENABLE;

    int i;
    for(i=0; i<256*192; i++) {
        VRAM_A[i] = COLOR(31, 15, 0);
    }

    while(1);

    return 0;
}
