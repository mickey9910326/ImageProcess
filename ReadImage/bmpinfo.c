#include "lib\bmp.h"

int main(int argc, char *argv[]) {
    BMP bmp;
    bmp_load(&bmp, argv[1]);
    bmpPrint(&bmp);
    return 0;
}
