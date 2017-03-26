#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

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
    uint8_t R;
    uint8_t G;
    uint8_t B;
} Pixel;

static void u32_to_bytes(uint32_t u32, uint8_t *bytes) {
	*bytes = (uint8_t) (u32 >> 24);
	*(bytes + 1) = (uint8_t) (u32 >> 16);
	*(bytes + 2) = (uint8_t) (u32 >> 8);
	*(bytes + 3) = (uint8_t) u32;
}

static void bytes_to_u32(uint8_t *bytes, uint32_t *u32) {
	*u32 = (uint32_t) ((*(bytes) ) |
           (uint32_t) (*(bytes + 1) << 8 ) |
		   (uint32_t) (*(bytes + 2) << 16) |
           (uint32_t) (*(bytes + 3) << 24) );
}

static void u16_to_bytes(uint16_t u16, uint8_t *bytes) {
	*bytes = (uint8_t) (u16 >> 8);
	*(bytes + 1) = (uint8_t) u16;
}

static void bytes_to_u16(uint8_t *bytes, uint16_t *u16) {
	*u16 = (uint16_t) ((*bytes ) | *(bytes + 1) << 8);
}

int bmp_load(BMP *bmp, char *filename) {
    FILE *file;
    uint8_t header[HEADER_SIZE];
    uint8_t info[INFO_SIZE];

    if ((file = fopen(filename, "rb") ) == NULL ) {
       fprintf(stderr, "Error: bmpLoad(), File open fail!\n");
       exit(0);
    }

    fread(header, 1, HEADER_SIZE, file);
    fread(info, 1, INFO_SIZE, file);
    memcpy(bmp->fileheader.data, header, HEADER_SIZE);
    memcpy(bmp->infoheader.data, info, INFO_SIZE);
    // header
    FILEHEADER *fh = &bmp->fileheader;
    bytes_to_u16(header+0 ,&fh->type);
    printf("%04x\n",fh->type);
    assert(fh->type == 0x4d42);
    bytes_to_u32(header+2 ,&fh->size);
    bytes_to_u16(header+6 ,&fh->reserved1);
    bytes_to_u16(header+8 ,&fh->reserved2);
    bytes_to_u32(header+10,&fh->pixel_array_offset);
    // info-header
    INFOHEADER *ih = &bmp->infoheader;
    bytes_to_u32(info+0 ,&ih->size);
    printf("%d\n",ih->size);
    assert(ih->size==40);
    bytes_to_u32(info+4 ,&ih->width);
    bytes_to_u32(info+8 ,&ih->height);
    bytes_to_u16(info+12,&ih->planes);
    bytes_to_u16(info+14,&ih->bits_per_pixel);
    bytes_to_u32(info+16,&ih->compression);
    bytes_to_u32(info+20,&ih->imagesize);
    bytes_to_u32(info+24,&ih->xresolution);
    bytes_to_u32(info+28,&ih->yresolution);
    bytes_to_u32(info+32,&ih->ncolours);
    bytes_to_u32(info+36,&ih->importantcolours);

    bmp->data = malloc(pixel_array_size(bmp));
    fseek(file, bmp->fileheader.pixel_array_offset, SEEK_SET);
    fread(bmp->data, 1, pixel_array_size(bmp), file);

    fclose (file);
}


int bmpPrint(BMP *bmp) {
   printf("==== Header ====\n");
   printf("Type = %04X\n", bmp->fileheader.type); // 0x4d42 = BM
   printf("FileSize = %ld \n", bmp->fileheader.size);
   printf("PixelArrayOffset = %ld \n", bmp->fileheader.pixel_array_offset);
   printf("==== Info ======\n");
   printf("size = %ld \n", bmp->infoheader.size);
   printf("Width = %ld \n", bmp->infoheader.width);
   printf("Height = %ld \n", bmp->infoheader.height);
   printf("Planes = %d \n", bmp->infoheader.planes);
   printf("BitsPerPixel = %d \n", bmp->infoheader.bits_per_pixel);
   printf("Compression = %ld \n", bmp->infoheader.compression);
   printf("Imagesize = %ld \n", bmp->infoheader.imagesize);
   printf("Xresolution = %ld \n", bmp->infoheader.xresolution);
   printf("Yresolution = %ld \n", bmp->infoheader.yresolution);
   printf("Ncolours = %ld \n", bmp->infoheader.ncolours);
   printf("Importantcolours = %ld \n", bmp->infoheader.importantcolours);
}

int bmpSetPixel(BMP *bmp, int x, int y, uint8_t R, uint8_t G, uint8_t B) {
    Pixel *pixel;
    int idx = y * bmp->infoheader.width + x;
    pixel = (Pixel*) &bmp->data[idx * sizeof(Pixel)];
    pixel->R = R;
    pixel->G = G;
    pixel->B = B;
}

int bmpSetBox(BMP *bmp, int x, int y, int w, int h, uint8_t R, uint8_t G, uint8_t B) {
    int i,j;
    for (i=0; i< 0+w; i++)
        for (j=0; j<h; j++)
            bmpSetPixel(bmp, x+i, y+j, R, G, B);
}

int bmpSave(BMP *bmp, char *fileName) {
    FILE *file;
    if ((file = fopen(fileName, "wb") ) == NULL ) {
        fprintf(stderr, "Error: bmpSave(), File create fail!\n");
        exit(0);
    }
    fwrite(&bmp->fileheader, 1, HEADER_SIZE, file);
    fwrite(&bmp->infoheader, 1, INFO_SIZE, file);
    fseek(file, bmp->fileheader.pixel_array_offset, SEEK_SET);
    fwrite(bmp->data, 1, pixel_array_size(bmp), file);
    fclose(file);
}

int main(int argc, char *argv[]) {
    BMP bmp;
    bmp_load(&bmp, argv[1]);
    bmpPrint(&bmp);
    bmpSetBox(&bmp, 0, 0, 20, 20, 0x66, 0x33, 0x99);
    bmpSave(&bmp, argv[2]);
    return 0;
}
