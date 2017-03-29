#include "bmp.h"

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

int bmpSetBox(BMP *bmp, int x, int y, int w, int h, uint8_t R, uint8_t G, uint8_t B) {
    int i,j;
    for (i=0; i< 0+w; i++)
    for (j=0; j<h; j++)
    bmpSetPixel(bmp, x+i, y+j, R, G, B);
}

void bmp_converse(BMP *bmp) {
	printf("size = %ld \n", bmp->infoheader.size);
	printf("Width = %ld \n", bmp->infoheader.width);
	printf("Height = %ld \n", bmp->infoheader.height);

	uint32_t width  = bmp->infoheader.width;
	uint32_t height = bmp->infoheader.height;

	uint32_t x,y;
	for (x = 0; x < width/2; x++) {
		for (y = 0; y < height; y++) {
			uint32_t i1 = y * width + x;
		    uint32_t i2 = y * width + (width-x);
			Pixel *p1,*p2;
		    p1 = (Pixel*) &bmp->data[i1 * sizeof(Pixel)];
			p2 = (Pixel*) &bmp->data[i2 * sizeof(Pixel)];
			swap_pixel(p1,p2);
		}
	}
}

void bmp_con2(BMP *bmp) {
	printf("size = %ld \n", bmp->infoheader.size);
	printf("Width = %ld \n", bmp->infoheader.width);
	printf("Height = %ld \n", bmp->infoheader.height);

	uint32_t width  = bmp->infoheader.width;
	uint32_t height = bmp->infoheader.height;

	uint32_t x,y;
	for (x = 0; x < width/2; x++) {
		for (y = 0; y < height; y++) {
			uint32_t i1 = y * width + x;
		    uint32_t i2 = width*height-i1-1;
			Pixel *p1,*p2;
		    p1 = (Pixel*) &bmp->data[i1 * sizeof(Pixel)];
			p2 = (Pixel*) &bmp->data[i2 * sizeof(Pixel)];
			swap_pixel(p1,p2);
		}
	}
}

void swap_pixel(Pixel* p1,Pixel* p2) {
	uint8_t Rtmp = p1->R;
	uint8_t Gtmp = p1->G;
	uint8_t Btmp = p1->B;
	p1->R = p2->R;
	p1->G = p2->G;
	p1->B = p2->B;
	p2->R = Rtmp;
	p2->G = Gtmp;
	p2->B = Btmp;
}
