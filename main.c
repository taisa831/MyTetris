#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#define FIELD_WIDTH 12
#define FIELD_HEIGHT 22

#define MINO_TYPE_MAX 7
#define MINO_ANGLE_MAX 4

#define MINO_WIDTH 4
#define MINO_HEIGHT 4

char field[FIELD_HEIGHT][FIELD_WIDTH];
char displayBuffer[FIELD_HEIGHT][FIELD_WIDTH];

char minoShapes[MINO_TYPE_MAX][MINO_ANGLE_MAX][MINO_HEIGHT][MINO_WIDTH] = {
        {
                // MINO_ANGLE_0
                {
                        0, 1, 0, 0,
                        0, 1, 0, 0,
                        0, 1, 0, 0,
                        0, 1, 0, 0,
                },
                // MINO_ANGLE_90
                {
                        0, 0, 0, 0,
                        0, 0, 0, 0,
                        1, 1, 1, 1,
                        0, 0, 0, 0,
                },
                // MINO_ANGLE_180
                {
                        0, 0, 1, 0,
                        0, 0, 1, 0,
                        0, 0, 1, 0,
                        0, 0, 1, 0,
                },
                // MINO_ANGLE_270
                {
                        0, 0, 0, 0,
                        1, 1, 1, 1,
                        0, 0, 0, 0,
                        0, 0, 0, 0,
                },
        },
        // MINO_TYPE_O
        {
                {
                        0, 0, 0, 0,
                        0, 1, 1, 0,
                        0, 1, 1, 0,
                        0, 0, 0, 0,
                },
                {
                        0, 0, 0, 0,
                        0, 1, 1, 0,
                        0, 1, 1, 0,
                        0, 0, 0, 0,
                },
                {
                        0, 0, 0, 0,
                        0, 1, 1, 0,
                        0, 1, 1, 0,
                        0, 0, 0, 0,
                },
                {
                        0, 0, 0, 0,
                        0, 1, 1, 0,
                        0, 1, 1, 0,
                        0, 0, 0, 0,
                },
        },
        // MINO_TYPE_S
        {
                {
                        0, 0, 0, 0,
                        0, 1, 1, 0,
                        1, 1, 0, 0,
                        0, 0, 0, 0,
                },
                {
                        0, 1, 0, 0,
                        0, 1, 1, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 0,
                },
                {
                        0, 0, 0, 0,
                        0, 1, 1, 0,
                        1, 1, 0, 0,
                        0, 0, 0, 0,
                },
                {
                        0, 1, 0, 0,
                        0, 1, 1, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 0,
                },
        },
        // MINO_TYPE_Z
        {
                {
                        0, 0, 0, 0,
                        1, 1, 0, 0,
                        0, 1, 1, 0,
                        0, 0, 0, 0,
                },
                {
                        0, 0, 1, 0,
                        0, 1, 1, 0,
                        0, 1, 0, 0,
                        0, 0, 0, 0,
                },
                {
                        0, 0, 0, 0,
                        1, 1, 0, 0,
                        0, 1, 1, 0,
                        0, 0, 0, 0,
                },
                {
                        0, 0, 1, 0,
                        0, 1, 1, 0,
                        0, 1, 0, 0,
                        0, 0, 0, 0,
                },
        },
        // MINO_TYPE_J
        {
                {
                        0, 0, 1, 0,
                        0, 0, 1, 0,
                        0, 1, 1, 0,
                        0, 0, 0, 0,
                },
                {
                        0, 0, 0, 0,
                        1, 1, 1, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 0,
                },
                {
                        0, 1, 1, 0,
                        0, 1, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 0, 0,
                },
                {
                        0, 0, 0, 0,
                        1, 0, 0, 0,
                        1, 1, 1, 0,
                        0, 0, 0, 0,
                },
        },
        // MINO_TYPE_L
        {
                {
                        0, 1, 0, 0,
                        0, 1, 0, 0,
                        0, 1, 1, 0,
                        0, 0, 0, 0,
                },
                {
                        0, 0, 0, 0,
                        0, 0, 0, 1,
                        0, 1, 1, 1,
                        0, 0, 0, 0,
                },
                {
                        0, 1, 1, 0,
                        0, 0, 1, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 0,
                },
                {
                        0, 0, 0, 0,
                        0, 1, 1, 1,
                        0, 1, 0, 0,
                        0, 0, 0, 0,
                },
        },
        // MINO_TYPE_T
        {
                {
                        0, 0, 0, 0,
                        1, 1, 1, 0,
                        0, 1, 0, 0,
                        0, 0, 0, 0,
                },
                {
                        0, 1, 0, 0,
                        0, 1, 1, 0,
                        0, 1, 0, 0,
                        0, 0, 0, 0,
                },
                {
                        0, 1, 0, 0,
                        1, 1, 1, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0,
                },
                {
                        0, 1, 0, 0,
                        1, 1, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 0, 0,
                },
        },
};

bool kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

bool isHit(int _minoX, int _minoY, int _minoType, int _minoAngle) {
    for (int i = 0; i < MINO_HEIGHT; i++) {
        for (int j = 0; j < MINO_WIDTH; j++) {
            if (minoShapes[_minoType][_minoAngle][i][j] && field[_minoY + i][_minoX + j]) {
                return true;
            }
        }
    }
    return false;
}

int minoType = 0, minoAngle = 0;
int minoX = 5, minoY = 0;

int getRandom(int min, int max) {

    static int flag;

    if (flag == 0) {
        srand((unsigned int) time(NULL));
        flag = 1;
    }

    return min + (int) (rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}

void resetMino() {
    minoX = 5;
    minoY = 0;
    //minoType = rand() % MINO_TYPE_MAX;
    minoType = getRandom(0, MINO_TYPE_MAX - 1);
    //minoAngle = rand() % MINO_ANGLE_MAX;
    minoAngle = getRandom(0, MINO_ANGLE_MAX - 1);
}

void display() {

    memcpy(displayBuffer, field, sizeof(field));

    for (int i = 0; i < MINO_HEIGHT; i++) {
        for (int j = 0; j < MINO_WIDTH; j++) {
            displayBuffer[minoY + i][minoX + j] |= minoShapes[minoType][minoAngle][i][j];
        }
    }

    system("clear");

    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            printf(displayBuffer[i][j] ? "■" : " ");
        }
        printf("\n");
    }
}

int main() {

    memset(field, 0, sizeof(field));

    for (int i = 0; i < FIELD_HEIGHT; i++) {
        field[i][0] = field[i][FIELD_WIDTH - 1] = 1;
    }

    for (int i = 0; i < FIELD_WIDTH; i++) {
        field[FIELD_HEIGHT - 1][i] = 1;
    }

    resetMino();

    time_t t = time(NULL);

    while (1) {

        if (kbhit()) {
            switch (getchar()) {
                case 's':
                    if (!isHit(minoX, minoY + 1, minoType, minoAngle)) {
                        minoY++;
                    }
                    break;
                case 'a':
                    if (!isHit(minoX - 1, minoY, minoType, minoAngle)) {
                        minoX--;
                    }
                    break;
                case 'd':
                    if (!isHit(minoX + 1, minoY, minoType, minoAngle)) {
                        minoX++;
                    }
                    break;
                case 0x20:
                    if (!isHit(minoX, minoY, minoType, (minoAngle + 1) % MINO_ANGLE_MAX)) {
                        minoAngle = (minoAngle + 1) % MINO_ANGLE_MAX;
                    }
                    break;
            }
            display();
        }

        if (t != time(NULL)) {

            t = time(NULL);
            if (isHit(minoX, minoY + 1, minoType, minoAngle)) {
                for (int i = 0; i < MINO_HEIGHT; i++) {
                    for (int j = 0; j < MINO_WIDTH; j++) {
                        field[minoY + i][minoX + j] |= minoShapes[minoType][minoAngle][i][j];
                    }
                }

                for (int i = 0; i < FIELD_HEIGHT - 1; i++) {
                    int lineFill = 1;
                    for (int j = 1; j < FIELD_WIDTH - 1; j++) {
                        if (!field[i][j]) {
                            lineFill = 0;
                        }
                    }

                    if (lineFill) {
                        for (int j = i; 0 < j; j--) {
                            memcpy(field[j], field[j - 1], FIELD_WIDTH);
                        }
                    }
                }

                resetMino();

            } else {
                minoY++;
            }
            display();
        }
    }
}

