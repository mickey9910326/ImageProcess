#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include <string.h>
#include "byte_op.h"

#define HEADER_SIZE 14 // 14 bytes file-header
#define INFO_SIZE 40   // 40 bytes info-header
#define pixel_array_size(bmp) (bmp->infoheader.width*bmp->infoheader.height*3)

typedef struct {
    // 14 bytes file-header
    uint8_t  data[HEADER_SIZE];
    uint16_t type; // Magic identifier, usually be "BM" (0x42,0x4d)
    uint32_t size; // file size (bytes)
    uint16_t reserved1, reserved2; // Reserved 2 bytes
    uint32_t pixel_array_offset; // offset to pixel array (bytes)
} FILEHEADER;

typedef struct {
    uint16_t data[INFO_SIZE];
    uint32_t size; // info-size
    uint32_t width,height; // width and height of image
    uint16_t planes; // number of color planes (must be 1)
    uint16_t bits_per_pixel; // bits per pixel
    uint32_t compression; // compression type
    uint32_t imagesize; // image size in bytes
    uint32_t xresolution,yresolution; // pixels per meter
    uint32_t ncolours; // number of colours. default is 0, mean to 2^(bits_per_pixel)
    uint32_t importantcolours; // number of important colours, generally 0 = all
} INFOHEADER;

typedef struct {
    // ColorTable : present only if Info.BitsPerPixel <= 8 colors should be ordered by importance
    uint32_t size; // size (must be 16 bytes)
    uint16_t blue; // Blue intensity
    uint16_t green; // Green intensity
    uint16_t red; // Red intensity
    uint16_t creserved; // unused (=0)
} COLORTABLE;

typedef struct {
    FILEHEADER fileheader;
    INFOHEADER infoheader;
    COLORTABLE colortable;
    uint8_t *data;
} BMP;

typedef struct {
    uint8_t B;
    uint8_t G;
    uint8_t R;
} Pixel;

void bmp_converse(BMP *bmp);
int bmp_load(BMP *bmp, char *filename);
int bmpPrint(BMP *bmp);
int bmpSetPixel(BMP *bmp, int x, int y, uint8_t R, uint8_t G, uint8_t B);
int bmpSave(BMP *bmp, char *fileName);
int bmpSetBox(BMP *bmp, int x, int y, int w, int h, uint8_t R, uint8_t G, uint8_t B);
void swap_pixel(Pixel* p1,Pixel* p2);
void bmp_con2(BMP *bmp);
