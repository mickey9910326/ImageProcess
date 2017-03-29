#include "lib\bmp.h"
#include <math.h>

void bmp_g(BMP *bmp,int a);

Pixel* get_pixel_at(BMP* bmp,uint32_t x,uint32_t y);
float mean_RGB(Pixel* p);
void pixel_put(Pixel* p,float data);

void bmp2gray(BMP *bmp);
void filter_3x3_mean(BMP* bmp,BMP* bmp2,float* filter,float* filter2,float threshold);

int main(int argc, char *argv[]) {
    BMP bmp,bmp2;
    float filter_Sobel_edge_3x3[9] ={ -1,-2,-1,0,0,0,1,2,1 };
    float filter_Sobel_edge_3x3_inv[9] ={ -1,0,1,-2,0,2,-1,0,1 };
    bmp_load(&bmp, argv[1]);
    bmp_load(&bmp2, argv[1]);
    bmp2gray(&bmp2);
    bmpSave(&bmp2,"t1.bmp");
    filter_3x3_mean(&bmp,&bmp2,filter_Sobel_edge_3x3,filter_Sobel_edge_3x3_inv,0);
    bmpSave(&bmp2,"t1_sobel.bmp");
    filter_3x3_mean(&bmp,&bmp2,filter_Sobel_edge_3x3,filter_Sobel_edge_3x3_inv,25);
    bmpSave(&bmp2,"t1_25.bmp");
    filter_3x3_mean(&bmp,&bmp2,filter_Sobel_edge_3x3,filter_Sobel_edge_3x3_inv,50);
    bmpSave(&bmp2,"t1_50.bmp");
    filter_3x3_mean(&bmp,&bmp2,filter_Sobel_edge_3x3,filter_Sobel_edge_3x3_inv,75);
    bmpSave(&bmp2,"t1_75.bmp");
    filter_3x3_mean(&bmp,&bmp2,filter_Sobel_edge_3x3,filter_Sobel_edge_3x3_inv,100);
    bmpSave(&bmp2,"t1_100.bmp");
    filter_3x3_mean(&bmp,&bmp2,filter_Sobel_edge_3x3,filter_Sobel_edge_3x3_inv,100);
    bmpSave(&bmp2,"t1_100.bmp");
    filter_3x3_mean(&bmp,&bmp2,filter_Sobel_edge_3x3,filter_Sobel_edge_3x3_inv,125);
    bmpSave(&bmp2,"t1_125.bmp");
    filter_3x3_mean(&bmp,&bmp2,filter_Sobel_edge_3x3,filter_Sobel_edge_3x3_inv,150);
    bmpSave(&bmp2,"t1_150.bmp");

    // filter_3x3_mean(&bmp,filter_Sobel_edge_3x3_inv);
    // bmpSave(&bmp,"t2.bmp");
    // XXX gradient at 45 drg is useless
    return 0;
}

void filter_3x3_mean(BMP* bmp,BMP* bmp2,float* filter,float* filter2,float threshold) {
	uint32_t width  = bmp->infoheader.width;
	uint32_t height = bmp->infoheader.height;

	uint32_t x,y;
    float data1 = 0,data2 = 0,v;
	for (x = 1; x < width-1; x++) {
		for (y = 1; y < height-1; y++) {
            data1 = 0.0;
            data1 += filter[0]*mean_RGB(get_pixel_at(bmp,x-1,y-1));
            data1 += filter[1]*mean_RGB(get_pixel_at(bmp,x  ,y-1));
            data1 += filter[2]*mean_RGB(get_pixel_at(bmp,x+1,y-1));
            data1 += filter[3]*mean_RGB(get_pixel_at(bmp,x-1,y  ));
            data1 += filter[4]*mean_RGB(get_pixel_at(bmp,x  ,y  ));
            data1 += filter[5]*mean_RGB(get_pixel_at(bmp,x+1,y  ));
            data1 += filter[6]*mean_RGB(get_pixel_at(bmp,x-1,y+1));
            data1 += filter[7]*mean_RGB(get_pixel_at(bmp,x  ,y+1));
            data1 += filter[8]*mean_RGB(get_pixel_at(bmp,x+1,y+1));

            data2 = 0.0;
            data2 += filter2[0]*mean_RGB(get_pixel_at(bmp,x-1,y-1));
            data2 += filter2[1]*mean_RGB(get_pixel_at(bmp,x  ,y-1));
            data2 += filter2[2]*mean_RGB(get_pixel_at(bmp,x+1,y-1));
            data2 += filter2[3]*mean_RGB(get_pixel_at(bmp,x-1,y  ));
            data2 += filter2[4]*mean_RGB(get_pixel_at(bmp,x  ,y  ));
            data2 += filter2[5]*mean_RGB(get_pixel_at(bmp,x+1,y  ));
            data2 += filter2[6]*mean_RGB(get_pixel_at(bmp,x-1,y+1));
            data2 += filter2[7]*mean_RGB(get_pixel_at(bmp,x  ,y+1));
            data2 += filter2[8]*mean_RGB(get_pixel_at(bmp,x+1,y+1));
            v = sqrt(data1*data1+data2*data2);
            if (threshold==0.0) {
                continue;
            }
            if(v>threshold){
                pixel_put(get_pixel_at(bmp2,x,y),0);
            } else {
                pixel_put(get_pixel_at(bmp2,x,y),255);
            }
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


void bmp2gray(BMP *bmp) {
	uint32_t width  = bmp->infoheader.width;
	uint32_t height = bmp->infoheader.height;

	uint32_t x,y,mean;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			uint32_t i1 = y * width + x;
			Pixel *p;
		    p= (Pixel*) &bmp->data[i1 * sizeof(Pixel)];
            mean =(p->R + p->G + p->B)/3;
            p->R = mean;
            p->G = mean;
            p->B = mean;
		}
	}
}
