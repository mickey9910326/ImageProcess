#include "lib\bmp.h"

void bmp_g(BMP *bmp,int a);

Pixel* get_pixel_at(BMP* bmp,uint32_t x,uint32_t y);
float mean_RGB(Pixel* p);
void pixel_put(Pixel* p,float data);
void filter_3x3_mean(BMP* bmp,float* filter);

int main(int argc, char *argv[]) {
    BMP bmp;
    float sharpen_fliter_3x3[9] ={ -1,-1,-1,-1,9,-1,-1,-1,-1 };
    float zero_fliter_3x3[9] ={ 0,0,0,0,1,0,0,0,0 };
    float filter_Sobel_edge_3x3[9] ={ 1.,2.,1.,0,0,0,-1.,-2.,-1. };
    float mean_fliter_3x3[9] ={ 1/9.,1/9.,1/9.,1/9.,-1/9.,1/9.,1/9.,1/9.,1/9. };
    float sharpen[9] ={ -1/9.,-1/9.,-1/9.,-1/9.,1.-1/9.,-1/9.,-1/9.,-1/9.,-1/9. };
    float mean[9] ={ 1/9.,1/9.,1/9.,1/9.,-1.+1/9.,1/9.,1/9.,1/9.,1/9. };
    bmp_load(&bmp, argv[1]);
    filter_3x3_mean(&bmp,mean);
    bmpSave(&bmp,"t1.bmp");
    filter_3x3_mean(&bmp,sharpen);
    bmpSave(&bmp,"t2.bmp");
    return 0;
}

void filter_3x3_mean(BMP* bmp,float* filter) {
	printf("size = %ld \n", bmp->infoheader.size);
	printf("Width = %ld \n", bmp->infoheader.width);
	printf("Height = %ld \n", bmp->infoheader.height);

	uint32_t width  = bmp->infoheader.width;
	uint32_t height = bmp->infoheader.height;

	uint32_t x,y;
    float data = 0;
	for (x = 1; x < width-1; x++) {
		for (y = 1; y < height-1; y++) {
            data = 0.0;
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

float mean_RGB(Pixel* p){
    return (float)((float)p->R + (float)p->G + (float)p->B)/3.;
}

Pixel* get_pixel_at(BMP* bmp,uint32_t x,uint32_t y){
    uint32_t i = y * bmp->infoheader.width + x;
    return (Pixel*) &bmp->data[i* sizeof(Pixel)];
}

void pixel_put(Pixel* p,float data){
    uint8_t d = data;
    p->R = d;
    p->G = d;
    p->B = d;
}
