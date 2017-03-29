#include "lib\bmp.h"
void bmp2gray(BMP *bmp);

int main(int argc, char *argv[]) {
    BMP bmp;
    bmp_load(&bmp, argv[1]);
    bmp2gray(&bmp);
    bmpSave(&bmp,argv[2]);

    return 0;
}

void bmp2gray(BMP *bmp) {
	printf("size = %ld \n", bmp->infoheader.size);
	printf("Width = %ld \n", bmp->infoheader.width);
	printf("Height = %ld \n", bmp->infoheader.height);

	uint32_t width  = bmp->infoheader.width;
	uint32_t height = bmp->infoheader.height;

	uint32_t x,y,mean;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			uint32_t i1 = y * width + x;
			Pixel *p;
		    p= (Pixel*) &bmp->data[i1 * sizeof(Pixel)];
            mean =(p->R + p->G + p->B)/3;
            printf("%d  ", mean);
            p->R = mean;
            p->G = mean;
            p->B = mean;
		}
	}
}
