#include "lib\bmp.h"
void bmp_g(BMP *bmp,int a);

int main(int argc, char *argv[]) {
    BMP bmp;
    bmp_load(&bmp, argv[1]);
    bmp_g(&bmp,1);
    bmpSave(&bmp,"r.bmp");
    bmp_load(&bmp, argv[1]);
    bmp_g(&bmp,2);
    bmpSave(&bmp,"g.bmp");
    bmp_load(&bmp, argv[1]);
    bmp_g(&bmp,3);
    bmpSave(&bmp,"b.bmp");
    return 0;
}

void bmp_g(BMP *bmp,int a) {
	printf("size = %ld \n", bmp->infoheader.size);
	printf("Width = %ld \n", bmp->infoheader.width);
	printf("Height = %ld \n", bmp->infoheader.height);

	uint32_t width  = bmp->infoheader.width;
	uint32_t height = bmp->infoheader.height;

	uint32_t x,y;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			uint32_t i1 = y * width + x;
			Pixel *p;
		    p= (Pixel*) &bmp->data[i1 * sizeof(Pixel)];
            if (a==1) {
                // p->R = 0;
                p->G = 0;
                p->B = 0;
            }
            if (a==2) {
                p->R = 0;
                // p->G = 0;
                p->B = 0;
            }
            if (a==3) {
                p->R = 0;
                p->G = 0;
                // p->B = 0;
            }
		}
	}
}
