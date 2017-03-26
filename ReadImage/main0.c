#include "lib\bmp.h"

int main(int argc, char *argv[]) {
    BMP bmp;
    bmp_load(&bmp, argv[1]);
    bmpPrint(&bmp);
    bmp_con2(&bmp);
    bmpSave(&bmp, argv[2]);
    return 0;
}
