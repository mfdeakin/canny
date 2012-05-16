
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>

#define PI 3.14159265358979323846264338327950288

int initDisp(int *argc, char **argv, void (*func)(void), unsigned width, unsigned height);

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
	void *image;
} bmp;

struct point
{
	float x, y, z;
	float r, g, b;
};

void renderScene(void);
int readBmp(char *file);

int main(int argc, char **argv)
{
	int ret = 0;
	readBmp(argv[1]);
	ret = initDisp(&argc, argv, &renderScene, bmp.header.width, bmp.header.height);
	if(ret)
		{
			printf("Error Initializing the Display");
			return ret;
		}
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
	glFlush();
	glutSwapBuffers();
}

int readBmp(char *file)
{
	FILE *fbmp;
	fbmp = fopen(file, "r");
	if(!fbmp)
		return 1;
	fread(&bmp.header, 1, sizeof(bmp.header), fbmp);
	if(bmp.header.signature != 0x4d42)
		{
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
	if(!bmp.image)
		{
			fclose(fbmp);
			return 2;
		}
	fseek(fbmp, bmp.header.offset - sizeof(bmp.header), SEEK_CUR);
	size_t bytes = fread(bmp.image, 1, bmp.header.size, fbmp);
	if(bytes != bmp.header.size)
		{
			printf("Number of bytes read not number needed!!!\n");
		}
	else
		printf("Read %d bytes\n", bmp.header.size);
	fclose(fbmp);
	return 0;
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
