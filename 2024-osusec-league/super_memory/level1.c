#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEVELWIDTH 256
#define LEVELHEIGHT 256

char level[LEVELHEIGHT][LEVELWIDTH];
int jumpCount = 0;

void read_flag() {
    int i;
    char flag[100];
    FILE *file = fopen("flag.txt", "rb");
    if (!file)
        strcpy(flag, "osu{placeholder}");
    else {
        size_t bytesRead = fread(flag, 1, 100, file);
        fwrite(flag, 1, bytesRead, stdout);
        fclose(file);
    }
    for (int y = 0; y < (LEVELHEIGHT); ++y) {
        for (int x = 0, i = 0; x < (LEVELWIDTH); ++x, ++i) {
            i %= strlen(flag);
            level[y][x] = flag[i];
        }
    }
}
 
void buildLevel(char level[LEVELHEIGHT][LEVELWIDTH], unsigned char * xpos, unsigned char * ypos) {
    FILE *file;
    file = fopen("./level1.txt", "r");

    size_t tmpBufferSize = 1000;
    char tmpBuffer[tmpBufferSize];
    int i = 0;
    while (fgets(tmpBuffer, tmpBufferSize, file)) {
        memcpy(level[i], tmpBuffer, 256);
        i ++;
    }
    for (int y = 0; y <= LEVELHEIGHT; y ++) {
        for (int x = 0; x <= LEVELWIDTH; x ++) {
            if (level[y][x] == 'p') {
                *xpos = x;
                *ypos = y;
                level[y][x] = ' ';
            }
        }
    }
}

void renderLevel(char level[LEVELHEIGHT][LEVELWIDTH], unsigned char xpos, unsigned char ypos, char viewW, char viewH) {
    unsigned char xRange[2] = {xpos - viewW/2, xpos + viewW/2};
    unsigned char yRange[2] = {ypos - viewH/2, ypos + viewH/2};

    if (viewW / 2 > xpos) {
        xRange[0] = 0;
        xRange[1] = viewW;
    }
    if (255 - viewW / 2 < xpos) {
        xRange[0] = 255-viewW;
        xRange[1] = (char)255;
    }
    if (viewH / 2 > ypos) {
        yRange[0] = 0;
        yRange[1] = viewH;
    }
    if (255 - viewH / 2 < ypos) {
        yRange[0] = 255-viewH;
        yRange[1] = (char)255;
    }
    for (int y = yRange[0]; y <= yRange[1]; y ++) {
        for (int x = xRange[0]; x <= xRange[1]; x ++) {
            if (x == xpos && y == ypos)
                printf("p");
            else
                printf("%c", level[y][x]);
        }
        printf("\n");
    }
}

char *handleMovement(unsigned char *xpos, unsigned char *ypos, char level[LEVELHEIGHT][LEVELWIDTH], int input) {
    if (level[*ypos+1][*xpos] != ' ') {
        jumpCount = 0;
    }
    if (input == 'd')
        (*xpos) ++;
    if (input == 'a')
        (*xpos) --;
    if (input == 'w') {
        if (level[*ypos+1][*xpos] != ' ') {
            jumpCount = 3;
        }
        if (jumpCount > 0) {
            (*ypos) --;
        }
    }
    if (input == 's')
        (*ypos) ++;
    switch (level[*ypos][*xpos]) {
        case ' ':
            break;
        case 'f':
            return "Does this feel like the end?\n\
Are you proud of yourself?\n\
Question: Should Magellan have been proud of himself had he turned around after leaving the harbor?\n\
Answer: No.";
            break;
        case '1':
            read_flag();
            break;
        default:
            if (input == 'w')
                (*ypos)++;
            else if (input == 'a')
                (*xpos)++;
            else if (input == 'd')
                (*xpos)--;
            else if (input == 's')
                (*ypos)--;
    }
    if (level[*ypos+1][*xpos] == ' ') {
        jumpCount --;
        if (jumpCount <= 0 || input != 'w')
            (*ypos) ++;
    }
    return "";
}

int main() {
    unsigned char xpos;
    unsigned char ypos;
    char input = 0;
    char *message_buffer;

    buildLevel(level, &xpos, &ypos);
    // Clear screen
    printf("\e[1;1H\e[2J");
    renderLevel(level, xpos, ypos, 80, 40);
    system ("/bin/stty cbreak");
    while((input = getchar()) != 3) {
        system ("/bin/stty cooked");
        // Clear screen
        printf("\e[1;1H\e[2J");
        message_buffer = handleMovement(&xpos, &ypos, level, input);
        renderLevel(level, xpos, ypos, 80, 40);
        printf("%s", message_buffer);
        system ("/bin/stty cbreak");
    }
}
