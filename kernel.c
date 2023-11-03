 //Ankita Panchal and Sivani Kommineni

void printString(char* str);
void printChar(char c);
void readString(char* str);
void readSector(char* buffer, int sector);
void handleInterrupt21(int ax, int bx, int cx, int dx);
void printNewLine();

void main() {
    char inputLine[80];
    char sectorBuffer[512];

    makeInterrupt21();
    interrupt(0x21, 0, "Enter a line: ");
    interrupt(0x21, 1, inputLine, 0, 0);
    interrupt(0x21, 0, "Your input is: ", 0, 0);
    interrupt(0x21, 0, inputLine, 0, 0);

    // Read Sector
    interrupt(0x21, 2, sectorBuffer, 30, 0);
    interrupt(0x21, 0, sectorBuffer, 0, 0);

      while (1);
}

void printString(char* str) {
    while (*str != '\0') {
        printChar(*str);
        str += 0x1;
    }
}

void printChar(char c) {
    int ah, al, ax;
    ah = 0xe;
    al = c;
    ax = (ah * 256) + al;
    interrupt(0x10, ax, 0, 0, 0);
}

void readString(char* str) {
    char* start;
    char c;
    while (c != 0xd) {
        c = interrupt(0x16, 0, 0, 0, 0);
        if (c == 0x8) {
            if (str != start) {
                printChar(0x8);
                printChar(0x20);
                printChar(0x8);
                str -= 0x1;
                *str = 0x20;
            }
        } else {
            printChar(c);
            *str = c;
            str += 0x1;
        }
    }
    printChar(c);
    printChar(0xa);

    *str = 0xa;
    str += 0x1;
    *str = 0x0;
}

void readSector(char* buffer, int sector) {
    int ah, al, bx, ch, cl, dh, dl, ax, cx, dx;
    ah = 2;
    al = 1;
    bx = buffer;
    ch = 0; // track number
    cl = sector + 1; // relative sector number
    dh = 0; // head number
    dl = 0x80;

    ax = (ah * 256) + al;
    cx = (ch * 256) + cl;
    dx = (dh * 256) + dl;
    buffer = interrupt(0x13, ax, bx, cx, dx);
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
    if (ax == 0) {
        printString(bx);
    } else if (ax == 1) {
        readString(bx);
    } else if (ax == 2) {
        readSector(bx, cx);
    } else {
        printString("Invalid ax");
    }
}

void printNewLine() {
    printChar(0xd);
    printChar(0xa);
}
