
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "gaussian.h"

#define BLURMTX g16_7_7
#define BLURMTX_W 7
#define BLURMTX_H 7
#define PI 3.14159265358979323846264338327950288

struct __attribute__((packed)) bmpHead
{
	unsigned short signature;
	unsigned size_ignore;
	unsigned res;
	unsigned offset;
	unsigned headsize;
	unsigned width;
	unsigned height;
	unsigned short planes;
	unsigned short bpp;
	unsigned compression;
	unsigned size;
	unsigned hres;
	unsigned vres;
	unsigned colorcount;
	unsigned impcolorcount;
};

struct bitmap
{
	struct bmpHead header;
	unsigned char *image;
	float *grayscale;
	float *blur;
} bmp;

void renderScene(void);
int readBmp(char *file);
void createGrayscale();
void createGaussian();
int initDisp(int *argc, char **argv, void (*func)(void), unsigned width, unsigned height);

int main(int argc, char **argv)
{
	int ret = 0;
	ret = readBmp(argv[1]);
	if(ret) {
		printf("Error reading bitmap");
		return ret;
	}
	ret = initDisp(&argc, argv, &renderScene, 2 * bmp.header.width, bmp.header.height);
	if(ret) {
		printf("Error Initializing the Display");
		return ret;
	}
	createGrayscale();
	glutMainLoop();
	return 0;
}

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	/*	printf("Width: %d, Height: %d, format: %x, type: %x\n",
		   bmp.header.width, bmp.header.height, GL_RGBA,
		   GL_UNSIGNED_INT_8_8_8_8); */
	glWindowPos2s(0, 0);
	glDrawPixels(bmp.header.width, bmp.header.height,
		GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, bmp.image);
	glWindowPos2s(bmp.header.width, 0);
	glDrawPixels(bmp.header.width, bmp.header.height,
				 GL_LUMINANCE, GL_FLOAT, bmp.grayscale);
	glFlush();
	glutSwapBuffers();
}

void createGaussian()
{
	if(!bmp.grayscale)
		return;
	bmp.blur = malloc(sizeof(bmp.header.size));
	int i;
	for(i = 0; i < bmp.header.height; i++) {
		int j;
		for(j = 0; j < bmp.header.width; j++) {
			int k;
			int maxw = ((BLURMTX_W - i) < (BLURMTX_W / 2)) ? (BLURMTX_W - i) : (BLURMTX_W / 2);
			int minw = i < (BLURMTX_W / 2) ? -i : (-BLURMTX_W / 2);
			for(int k = minw; k < maxw; k++) {
			}
		}
	}
}

void createGrayscale()
{
	if(!bmp.image)
		return;
	bmp.grayscale = malloc(bmp.header.size);
	float buf[4];
	int i;
	for(i = 0; i < bmp.header.size / 4; i++) {
		buf[0] = bmp.image[i * 4];
		buf[1] = bmp.image[i * 4 + 1];
		buf[2] = bmp.image[i * 4 + 2];
		buf[0] /= 255.0f;
		buf[1] /= 255.0f;
		buf[2] /= 255.0f;
		bmp.grayscale[i] = 0.299f * buf[0] + 0.587f * buf[1] + 0.114f * buf[2];
	}
}

int initDisp(int *argc, char **argv, void (*func)(void), unsigned width, unsigned height)
{
	glutInit(argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	if(!glutCreateWindow("Canny"))
		return 4;
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glutDisplayFunc(func);
	return 0;
}

int readBmp(char *file)
{
	FILE *fbmp;
	fbmp = fopen(file, "r");
	if(!fbmp)
		return 1;
	fread(&bmp.header, 1, sizeof(bmp.header), fbmp);
	if(bmp.header.signature != 0x4d42) {
		fclose(fbmp);
		return 1;
	}
	/*	printf("Signature: %x\n"
		   "Size (Unreliable): %d\n"
		   "Reserved (0): %d\n"
		   "Offset: %x\n"
		   "Sizeof BITMAPINFOHEADER: %d\n"
		   "Image Width: %d\n"
		   "Image Height: %d\n"
		   "Planes: %d\n"
		   "Bits Per Pixel: %d\n"
		   "Compression: %d\n"
		   "Size: %d\n"
		   "Horizontal Resolution: %d\n"
		   "Vertical Resolution: %d\n"
		   "Number of Colors: %d\n"
		   "Number of Important Colors: %d\n",
		   bmp.header.signature, bmp.header.size_ignore, bmp.header.res,
		   bmp.header.offset, bmp.header.headsize, bmp.header.width,
		   bmp.header.height, bmp.header.planes, bmp.header.bpp,
		   bmp.header.compression, bmp.header.size, bmp.header.hres,
		   bmp.header.vres, bmp.header.colorcount, bmp.header.impcolorcount); */
	bmp.image = malloc(bmp.header.size);
	if(!bmp.image) {
		fclose(fbmp);
		return 2;
	}
	fseek(fbmp, bmp.header.offset - sizeof(bmp.header), SEEK_CUR);
	size_t bytes = fread(bmp.image, 1, bmp.header.size, fbmp);
	if(bytes != bmp.header.size) {
		printf("Number of bytes read not number needed!!!\n");
	}
	fclose(fbmp);
	return 0;
}
