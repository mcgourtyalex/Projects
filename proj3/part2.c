#include <emmintrin.h>
#include <nmmintrin.h>
#include <memory.h>
#define KERNX 3 //this is the x-size of the kernel. It will always be odd.
#define KERNY 3 //this is the y-size of the kernel. It will always be odd.
int conv2D(float* in, float* out, int data_size_X, int data_size_Y, float* kernel) {
	// the x coordinate of the kernel's center
	int kern_cent_X = (KERNX - 1)/2;
	// the y coordinate of the kernel's center
	int kern_cent_Y = (KERNY - 1)/2;
    int xCopy = data_size_X + kern_cent_X*2;
    int yCopy = data_size_Y + kern_cent_Y*2;
	int copysize = (xCopy) * (yCopy);
    
    omp_set_num_threads(16);
    
	// allocate a copy & fill with in & 0-buffer
    
	//float *copy = calloc(copysize, sizeof(float));
    float copy[copysize];
   	
    #pragma omp parallel for
    for (int a = 0; a < (copysize); a++) {
        *(copy + a) = 0;
    }
    
    #pragma omp parallel for 
    for (int b = 0; b < (data_size_Y); b++) {
        for (int a = 0; a < (data_size_X/16)*16; a+=16) {
            __m128 spot = _mm_loadu_ps(in + a + (b * data_size_X));
            _mm_storeu_ps(copy + a + (b + kern_cent_Y) * xCopy + kern_cent_X, spot);
            spot = _mm_loadu_ps(in + 4 + a + (b * data_size_X));
            _mm_storeu_ps(copy + 4 + a + (b + kern_cent_Y) * xCopy + kern_cent_X, spot);
            spot = _mm_loadu_ps(in + a + 8 + (b * data_size_X));
            _mm_storeu_ps(copy + a + 8 + (b + kern_cent_Y) * xCopy + kern_cent_X, spot);
            spot = _mm_loadu_ps(in + a + 12 + (b * data_size_X));
            _mm_storeu_ps(copy + a + 12 + (b + kern_cent_Y) * xCopy + kern_cent_X, spot);
        }
        
        for (int a = (data_size_X/16)*16; a < data_size_X; a++) {
            copy[a + (b+kern_cent_Y)*(xCopy) + kern_cent_X] = in[a + b*data_size_X];
        }
    }
    
    
	// print stuff here
    /*printf("\n");
     printf("copy:\n");
     printf("\n");
     for(int i = 0; i < (copysize); i++) {
     if (i % (xCopy) == 0) {
     printf("\n");
     }
     printf(" %f ", copy[i]);
     }
     printf("\n");
     printf("\n");
     printf("in:\n");
     printf("\n");
     for(int i = 0; i < (data_size_X * data_size_Y); i++) {
     if (i % (data_size_X) == 0) {
     printf("\n");
     }
     printf(" %f ", in[i]);
     }
     printf("\n");
    */
    
    // vars
    
    /*__m128 copyV;
     __m128 kerV;
     __m128 temp;
     __m128 outV;
     int kernelSpot;
     int kernelCopySpot;
     int *outSpot;
     int *copySpot;*/
    
	// main convolution loop
    int divisor = data_size_Y/16 - 1;
    while (data_size_Y % divisor != 0) {
        divisor--;
    }
    

    int data32 = (data_size_X/32)*32;
    int data8 = (data_size_X/8)*8;
    int data4 = (data_size_X/4)*4;
    
    #pragma omp parallel for
    for(int y = 0; y < data_size_Y; y++){
            // 32 increment
        for(int x = 0; x < data32; x+=32){
            int *outSpot = out + x + y * data_size_X;
            __m128 outV1 = _mm_loadu_ps(outSpot);
            __m128 outV2 = _mm_loadu_ps(outSpot + 4);
            __m128 outV3 = _mm_loadu_ps(outSpot + 8);
            __m128 outV4 = _mm_loadu_ps(outSpot + 12);
            __m128 outV5 = _mm_loadu_ps(outSpot + 16);
            __m128 outV6 = _mm_loadu_ps(outSpot + 20);
            __m128 outV7 = _mm_loadu_ps(outSpot + 24);
            __m128 outV8 = _mm_loadu_ps(outSpot + 28);
            for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){
                for(int i = -kern_cent_X; i <= kern_cent_X; i++){
                    //if(x+i>-1 && x+i<data_size_X && y+j>-1 && y+j<data_size_Y){}
                    /*out[x+y*data_size_X] += kernel[kernelSpot] * copy[x+i+1 + (y+j+1)*(data_size_X+2)];
                     out[x+1+y*data_size_X] += kernel[kernelSpot] * copy[x+i+2 + (y+j+1)*(data_size_X+2)];
                     out[x+2+y*data_size_X] += kernel[kernelSpot] * copy[x+i+3 + (y+j+1)*(data_size_X+2)];
                     out[x+3+y*data_size_X] += kernel[kernelSpot] * copy[x+i+4 + (y+j+1)*(data_size_X+2)];*/
                    
                    
                    int kernelSpot = (kern_cent_X-i)+(kern_cent_Y-j)*KERNX;
                    __m128 kerV = _mm_load1_ps(kernel + kernelSpot);
                    int *copySpot = x + y * (xCopy) + copy + i + 1  + j * (xCopy) + (xCopy);
                    
                    __m128 copyV = _mm_loadu_ps(copySpot);
                    __m128 temp = _mm_mul_ps(copyV,kerV);
                    outV1 = _mm_add_ps(temp, outV1);
                    _mm_storeu_ps(outSpot, outV1);
                    
                    copyV = _mm_loadu_ps(copySpot + 4);
                    temp = _mm_mul_ps(copyV,kerV);
                    outV2 = _mm_add_ps(temp, outV2);
                    _mm_storeu_ps(outSpot + 4, outV2);
                    
                    copyV = _mm_loadu_ps(copySpot + 8);
                    temp = _mm_mul_ps(copyV,kerV);
                    outV3 = _mm_add_ps(temp, outV3);
                    _mm_storeu_ps(outSpot + 8, outV3);
                    
                    copyV = _mm_loadu_ps(copySpot + 12);
                    temp = _mm_mul_ps(copyV,kerV);
                    outV4 = _mm_add_ps(temp, outV4);
                    _mm_storeu_ps(outSpot + 12, outV4);
                    
                    copyV = _mm_loadu_ps(copySpot + 16);
                    temp = _mm_mul_ps(copyV,kerV);
                    outV5 = _mm_add_ps(temp, outV5);
                    _mm_storeu_ps(outSpot + 16, outV5);
                    
                    copyV = _mm_loadu_ps(copySpot + 20);
                    temp = _mm_mul_ps(copyV,kerV);
                    outV6 = _mm_add_ps(temp, outV6);
                    _mm_storeu_ps(outSpot + 20, outV6);
                    
                    copyV = _mm_loadu_ps(copySpot + 24);
                    temp = _mm_mul_ps(copyV,kerV);
                    outV7 = _mm_add_ps(temp, outV7);
                    _mm_storeu_ps(outSpot + 24, outV7);
                    
                    copyV = _mm_loadu_ps(copySpot + 28);
                    temp = _mm_mul_ps(copyV,kerV);
                    outV8 = _mm_add_ps(temp, outV8);
                    _mm_storeu_ps(outSpot + 28, outV8);
                }
                
            }
        }
        
        // 8 increment edge
        for(int x = data32; x < data8; x+=8){
            int *outSpot = out + x + y * data_size_X;
            __m128 outV1 = _mm_loadu_ps(outSpot);
            __m128 outV2 = _mm_loadu_ps(outSpot + 4);
            for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){
                for(int i = -kern_cent_X; i <= kern_cent_X; i++){
                    
                    int kernelSpot = (kern_cent_X-i)+(kern_cent_Y-j)*KERNX;
                    __m128 kerV = _mm_load1_ps(kernel + kernelSpot);
                    int *copySpot = x + y * (xCopy) + copy + i + 1  + j * (xCopy) + (xCopy);
                    
                    __m128 copyV = _mm_loadu_ps(copySpot);
                    __m128 temp = _mm_mul_ps(copyV,kerV);
                    outV1 = _mm_add_ps(temp, outV1);
                    _mm_storeu_ps(outSpot, outV1);
                    
                    copyV = _mm_loadu_ps(copySpot + 4);
                    temp = _mm_mul_ps(copyV,kerV);
                    outV2 = _mm_add_ps(temp, outV2);
                    _mm_storeu_ps(outSpot + 4, outV2);
                    
                }
            }
        }
        
        // 4 increment edge
        for(int x = data8; x < data4; x+=4){
            int *outSpot = out + x + y * data_size_X;
            __m128 outV = _mm_loadu_ps(outSpot);
            for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){
                for(int i = -kern_cent_X; i <= kern_cent_X; i++){
                    int kernelSpot = (kern_cent_X-i)+(kern_cent_Y-j)*KERNX;
                    __m128 kerV = _mm_load1_ps(kernel + kernelSpot);
                    int *copySpot = x + y * (xCopy) + copy + i + 1  + j * (xCopy) + (xCopy);
                    
                    __m128 copyV = _mm_loadu_ps(copySpot);
                    __m128 temp = _mm_mul_ps(copyV,kerV);
                    outV = _mm_add_ps(temp, outV);
                    _mm_storeu_ps(outSpot, outV);
                }
            }
        }
        
        // 1 increment edge
        for(int x = data4; x < data_size_X; x++){
            int outSpot = x + y * data_size_X;
            for(int i = -kern_cent_X; i <= kern_cent_X; i++){
                for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){
                    int kernelSpot = (kern_cent_X-i)+(kern_cent_Y-j)*KERNX;
                    out[outSpot] += kernel[kernelSpot] * copy[x+i+1 + (y+j+1)*(xCopy)];
                }
            }
        }
    }

	return 1;
}
