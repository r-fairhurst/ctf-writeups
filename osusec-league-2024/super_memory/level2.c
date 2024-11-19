#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEVELWIDTH 256
#define LEVELHEIGHT 256

#define current_block_properties blocks[level[player_info->ypos][player_info->xpos]] // For brevity
#define current_level_location level[player_info->ypos][player_info->xpos] // For brevity

char level[LEVELHEIGHT][LEVELWIDTH];

struct PlayerInfo {
    int jumpCount;
    int collected_items;
    unsigned char xpos;
    unsigned char ypos;
    char inventory[4];
    uint dollars;
};

struct BlockProperties {
    int solid;
    int collectable;
    int disappears;
    int mustInteract;
    char* (*specialProperty)(char, struct PlayerInfo *);
};

struct BlockProperties blocks[256] = {0};

char* unlock(char key, struct PlayerInfo *xxx) {
    blocks[key-0x20].solid = 0;
    return "";
};

char* paid_unlock(char key, struct PlayerInfo *player_info) {
    if (blocks[key].solid == 1) {
        if (player_info->dollars == 1650549605) {
            player_info->dollars = 0;
            blocks[key].solid = 0;
            return "Unlocked!";
        }
        else if (player_info->dollars > 1650549605) {
            return "Your excessive wealth disgusts me.";
        }
        else {
            return "Sorry! You need $1650549605 to unlock me!";
        }
    }
    return "";
};

char* get_dollar(char key, struct PlayerInfo *player_info) {
    player_info->dollars ++;
    return "";
};

char* finish(char xxx, struct PlayerInfo *yyy) {
    return
"Congratulations! You \"Won!\"\n\
(I am not proud of you \
(I am proud of you, but it's for reasons other than your alleged \"completion\" \
of this level (I'm proud of you for showing up and trying hard (Alas, not as \
proud as I would be if you f1n15h3d th3 13v3l))) Plus, your bank account is pathetic)";
}

char* read_flag(char xxx, struct PlayerInfo *yyy) {
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
    return "";
}

void buildLevel(char level[LEVELHEIGHT][LEVELWIDTH], unsigned char *xpos, unsigned char *ypos) {
    FILE *file;
    file = fopen("./level2.txt", "r");

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

char* handleMovement(struct PlayerInfo *player_info, char level[LEVELHEIGHT][LEVELWIDTH], int input) {
    char *message = "";
    if (blocks[level[player_info->ypos+1][player_info->xpos]].solid == 1) {
        player_info->jumpCount = 3;
    }
    if (input == 'd')
        (player_info->xpos) ++;
    if (input == 'a')
        (player_info->xpos) --;
    if (input == 'w') {
        if (player_info->jumpCount > 0) {
            (player_info->ypos) --;
        }
    }
    if (input == 's' && current_block_properties.mustInteract) {
        if (current_block_properties.specialProperty) {
            message = (*current_block_properties.specialProperty)(current_level_location, player_info);
        }
        if (current_block_properties.collectable) {
            player_info->inventory[player_info->collected_items] = current_level_location;
            player_info->collected_items ++;
        }
        if (current_block_properties.disappears) {
            current_level_location = ' ';
        }
    }
    else if (!current_block_properties.mustInteract) {
        if (current_block_properties.specialProperty) {
            message = (*current_block_properties.specialProperty)(current_level_location, player_info);
        }
        if (current_block_properties.collectable) {
            player_info->inventory[player_info->collected_items] = current_level_location;
            player_info->collected_items ++;
        }
        if (current_block_properties.disappears) {
            current_level_location = ' ';
        }
    }
    if (current_block_properties.solid == 1) {
        if (input == 'w')
            (player_info->ypos)++;
        else if (input == 'a')
            (player_info->xpos)++;
        else if (input == 'd')
            (player_info->xpos)--;
    }
    if (blocks[level[player_info->ypos+1][player_info->xpos]].solid == 0) {
        player_info->jumpCount --;
        if (player_info->jumpCount <= 0 || input != 'w') {
            (player_info->ypos) ++;
            player_info->jumpCount = 0;
        }
    }
    return message;
}

void printInventory(char *inventory, int dollars, int inventory_size) {
    printf("-");
    for (int i = 0; i < inventory_size; i ++) {
        printf("--------");
    }
    printf("\n+");
    for (int i = 0; i < inventory_size; i ++) {
        printf("       +");
    }
    printf("\n+");
    for (int i = 0; i < inventory_size; i ++) {
        printf("   %c   +", inventory[i]);
    }
    printf("  $%d\n+", dollars);
    for (int i = 0; i < inventory_size; i ++) {
        printf("       +");
    }
    printf("\n-");
    for (int i = 0; i < inventory_size; i ++) {
        printf("--------");
    }
    printf("\n");
}

int main() {
    struct PlayerInfo player_info;
    char *message_buffer;
    int inventory_size = 4;

    player_info.jumpCount = 0;
    player_info.collected_items = 0;
    player_info.dollars = 0;
    memset(player_info.inventory, 32, inventory_size);

    blocks['+'].solid = 1;
    blocks['-'].solid = 1;

    blocks['['].solid = 1;
    blocks['['].specialProperty = (void *)paid_unlock;

    blocks['$'].disappears = 1;
    blocks['$'].specialProperty = (void *)get_dollar;

    blocks['f'].specialProperty = (void *)finish;

    blocks['1'].specialProperty = (void *)read_flag;

    for (char i = 'a'; i <= 'd'; i ++) {
        blocks[i].collectable = 1;
        blocks[i].mustInteract = 1;
        blocks[i].specialProperty = (void *)unlock;
    }
    for (char i = 'A'; i <= 'D'; i ++) {
        blocks[i].solid = 1;
    }

    buildLevel(level, &player_info.xpos, &player_info.ypos);

    printf("\e[1;1H\e[2J");
    renderLevel(level, player_info.xpos, player_info.ypos, 80, 20);
    printInventory(player_info.inventory, player_info.dollars, inventory_size);
    system ("/bin/stty cbreak");
    printf("Capital letters are gates. Press 's' to pick up their matching key!\n");
    char input = 0;
    while ((input = getchar()) != 3) {
        system ("/bin/stty cooked");
        // Clear screen
        printf("\e[1;1H\e[2J");
        message_buffer = handleMovement(&player_info, level, input);
        renderLevel(level, player_info.xpos, player_info.ypos, 80, 20);
        printInventory(player_info.inventory, player_info.dollars, inventory_size);
        printf("%s\n", message_buffer);
        system ("/bin/stty cbreak");
    }
}
