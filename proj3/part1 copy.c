#include <emmintrin.h>
#define KERNX 3 //this is the x-size of the kernel. It will always be odd.
#define KERNY 3 //this is the y-size of the kernel. It will always be odd.
int conv2D(float* in, float* out, int data_size_X, int data_size_Y,
                    float* kernel)
{
    // the x coordinate of the kernel's center
    int kern_cent_X = (KERNX - 1)/2;
    // the y coordinate of the kernel's center
    int kern_cent_Y = (KERNY - 1)/2;
    
    // main convolution loop
    for(int i = -kern_cent_X; i <= kern_cent_X; i++){
        for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){
            for(int y = 0; y < data_size_Y; y++){
                for(int x = 0; x < data_size_X; x++){
					if(x+i>-1 && x+i<data_size_X && y+j>-1 && y+j<data_size_Y){
						out[x+y*data_size_X] += kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
					}
				}
			}
		}
	}
	return 1;
}
