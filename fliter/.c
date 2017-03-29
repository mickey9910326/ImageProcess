#include "lib\bmp.h"
void bmp_g(BMP *bmp,int a);

Pixel* get_pixel_at(BMP* bmp,uint32_t x,uint32_t y);
uint8_t mean_RGB(Pixel* p);
void pixel_put(Pixel* p,uint8_t data);

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

void filter_3x3_mean(BMP* bmp,uint8_t* filter) {
	printf("size = %ld \n", bmp->infoheader.size);
	printf("Width = %ld \n", bmp->infoheader.width);
	printf("Height = %ld \n", bmp->infoheader.height);

	uint32_t width  = bmp->infoheader.width;
	uint32_t height = bmp->infoheader.height;

	uint32_t x,y;
    uint16_t data = 0;
	for (x = 1; x < width-1; x++) {
		for (y = 1; y < height-1; y++) {
            data += filter[0]*mean_RGB(get_pixel_at(bmp,x-1,y-1));
            data += filter[1]*mean_RGB(get_pixel_at(bmp,x  ,y-1));
            data += filter[2]*mean_RGB(get_pixel_at(bmp,x+1,y-1));
            data += filter[3]*mean_RGB(get_pixel_at(bmp,x-1,y  ));
            data += filter[4]*mean_RGB(get_pixel_at(bmp,x  ,y  ));
            data += filter[5]*mean_RGB(get_pixel_at(bmp,x+1,y  ));
            data += filter[6]*mean_RGB(get_pixel_at(bmp,x-1,y+1));
            data += filter[7]*mean_RGB(get_pixel_at(bmp,x  ,y+1));
            data += filter[8]*mean_RGB(get_pixel_at(bmp,x+1,y+1));
            pixel_put(get_pixel_at(bmp,x,y),data);
		}
	}
}

uint8_t mean_RGB(Pixel* p){
    return (p->R + p->G + p->B)/3;
}

Pixel* get_pixel_at(BMP* bmp,uint32_t x,uint32_t y){
    return (Pixel*) &bmp->data[ y * bmp->infoheader.width + x];
}

void pixel_put(Pixel* p,uint8_t data){
    p->R = data;
    p->G = data;
    p->B = data;
}
