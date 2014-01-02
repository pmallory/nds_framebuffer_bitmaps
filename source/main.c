typedef volatile unsigned int vu32;
typedef unsigned short u16;
typedef volatile unsigned short vu16;
typedef volatile unsigned char vu8;

#define REG_DISPCNT_MAIN  (*(vu32*)0x04000000)
#define REG_DISPCNT_SUB   (*(vu32*)0x04001000)
#define MODE_FB0    (0x00020000)
#define VRAM_A        ((u16*)0x6800000)
#define VRAM_A_CR         (*(vu8*)0x04000240)
#define VRAM_ENABLE       (1<<7)
#define SCANLINECOUNTER *(vu16 *)0x4000006

#define COLOR(r,g,b) ((r) | (g)<<5 | (b)<<10)
#define OFFSET(r,c,w) ((r)*(w)+(c))

#define SCREENWIDTH  (256)
#define SCREENHEIGHT (192)

void setPixel(int row, int col, u16 color);
void drawRect(int row, int col, int width, int height, u16 color);
void initialize_ball();
void update();
void draw();
void waitForVblank();

typedef struct BALL{
    int row;
    int col;
    int size;
    int rdel;
    int cdel;
    u16 color;
} BALL;

BALL ball;
BALL old_ball;

int main(void) {

    REG_DISPCNT_MAIN = MODE_FB0;

    VRAM_A_CR = VRAM_ENABLE;

    initialize_ball();

    while(1) {
        update();
        waitForVblank();
        draw();
    }

    return 0;
}

void initialize_ball() {
    ball.row = 10;
    ball.col= 10;
    ball.size = 5;
    ball.rdel= 1;
    ball.cdel = 2;
    ball.color = COLOR(15,27,7);
}

void update() {
    old_ball = ball;

    // move ball
    ball.row += ball.rdel;
    ball.col+= ball.cdel;

    // check for collisions with the sides of the screen
    if (ball.row + ball.size > SCREENHEIGHT) {
        ball.row = SCREENHEIGHT - ball.size;
        ball.rdel *= -1;
    }
    if (ball.col + ball.size > SCREENWIDTH) {
        ball.col = SCREENWIDTH- ball.size;
        ball.cdel *= -1;
    }
    if (ball.row < 0) {
        ball.row = 0;
        ball.rdel *= -1;
    }
    if (ball.col< 0) {
        ball.col = 0;
        ball.cdel *= -1;
    }
}

void draw() {
    // erase the ball
    drawRect(old_ball.row,
                   old_ball.col,
                   old_ball.size,
                   old_ball.size,
                   0);

    // draw it in its new position
    drawRect(ball.row,
                   ball.col,
                   ball.size,
                   ball.size,
                   ball.color);
}

void setPixel(int row, int col, u16 color) {
    VRAM_A[OFFSET(row, col, SCREENWIDTH)] = color;
}

void drawRect(int row, int col, int width, int height, u16 color) {
    int r, c;
    for (c=col; c<col+width; c++) {
        for (r=row; r<row+height; r++) {
            setPixel(r, c, color);
        }
    }
}

void waitForVblank() {
    while (SCANLINECOUNTER > SCREENHEIGHT);
    while (SCANLINECOUNTER < SCREENHEIGHT);
}

