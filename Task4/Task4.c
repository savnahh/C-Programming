//Savona Shrestha

#include <stdio.h>
#include "lodepng.h"
#include <stdlib.h>
#include <pthread.h>

int ***arr;//3D array to store RGB
unsigned char *image;

struct pixels{
    int start_h, end_h;
	int h, w;
	int thread_id;
	int k_size;
};
//function to apply Gaussian blur
void *blur(void *ptr){
    int i, j, k, l, m;
    struct pixels *arg = (struct pixels *)ptr;
    int start_h = arg->start_h, end_h = arg->end_h, k_size = arg->k_size;
    int thread_id = arg->thread_id, h = arg->h, w = arg->w;

    for (i = start_h; i <= end_h; i++){
        for (j = 0; j < w; j++){
            int avg_red = 0, avg_green = 0, avg_blue = 0, num_pixels = 0;
            //kernel
            for (l = -k_size; l <= k_size; l += k_size){
                for (m = -k_size; m <= k_size; m += k_size){
                    int xPixel = i + l;
                    int yPixel = j + m;
                    //check pixel
                    if (xPixel >= 0 && yPixel >= 0 && xPixel < h && yPixel < w){
                    	//
                        avg_red += arr[xPixel][yPixel][0];
                        avg_green += arr[xPixel][yPixel][1];
                        avg_blue += arr[xPixel][yPixel][2];
                        num_pixels++;
                    }
                }
            }
            //Calculate the average RGB values
            image[4 * w * i + 4 * j + 0] = avg_red / num_pixels;
            image[4 * w * i + 4 * j + 1] = avg_green / num_pixels;
            image[4 * w * i + 4 * j + 2] = avg_blue / num_pixels;
        }
    }
    pthread_exit(0);
}

void main(){
    unsigned int error, w, h;
    int i, j, k;
    int n_Threads, k_size;
    char filename[20];
    //read the image
    printf("Enter image you want to blur (.png extension): ");
    scanf("%s", &filename);
    //decode the PNG image into memory
    error = lodepng_decode32_file(&image, &w, &h, filename);
    //Number of threads
    printf("Enter number of threads to blur the image: ");
    scanf("%d", &n_Threads);

    if (n_Threads > h)
    {
        n_Threads = n_Threads % h + 1;
        printf("The number of threads is greater than height\nAutomatically choose required number of threads: %d threads", n_Threads);
    }

    printf("Please enter a number for the level of blur: ");
    scanf("%d", &k_size);
    //Allocate memory for 3D array
    arr = (int ***)malloc(h * sizeof(int **));
    for (i = 0; i < h; i++){
        arr[i] = (int **)malloc(w * sizeof(int *));
        for (j = 0; j < w; j++){
            arr[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }

    if (error){
        printf("Error in decoding image %d: %s\n", error, lodepng_error_text(error));
    }
    else{
        for (i = 0; i < h; i++){
            for (j = 0; j < w; j++){
                for (k = 0; k < 3; k++){
                    arr[i][j][k] = image[4 * w * i + 4 * j + k];
                }
            }
        }
    }
    //calculate height 
    int slice_h[n_Threads];
    for (i = 0; i < n_Threads; i++){
        slice_h[i] = h / n_Threads;
    }
    int reminderH = h % n_Threads;
    for (i = 0; i < reminderH; i++){
        slice_h[i]++;
    }
    //calculate start and end heights
    int start_h[n_Threads], end_h[n_Threads];
    for (i = 0; i < n_Threads; i++){
        if (i == 0){
            start_h[i] = 0;
        }
        else{
            start_h[i] = end_h[i - 1] + 1;
        }
        end_h[i] = start_h[i] + slice_h[i] - 1;
    }
    //create threads
    struct pixels divider[n_Threads];
    pthread_t threads[n_Threads];
    int thid = 1;
    for (i = 0; i < n_Threads; i++){
        divider[i].start_h = start_h[i];
        divider[i].end_h = end_h[i];
        divider[i].h = h;
        divider[i].w = w;
        divider[i].thread_id = thid;
        divider[i].k_size = k_size;
        thid++;
        pthread_create(&threads[i], NULL, blur, &divider[i]);
    }

    for (i = 0; i < n_Threads; i++){
        pthread_join(threads[i], NULL);
    }
    //encode the blurred immage 
    unsigned char *png;
    char image_name[20];
    size_t pngsize;

    printf("Image has been blurred.\nEnter the name of blurred image(.png extension): ");
    scanf("%s", &image_name);
    lodepng_encode32(&png, &pngsize, image, w, h);
    lodepng_save_file(png, pngsize, image_name);
}
