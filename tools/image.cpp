#include "image.h"

// ����ͷ�ļ������������壬���ܰ�����image.h�У�����image.h������ʱ�ᵼ���ض������LNK2005��LNK1169
#define STB_IMAGE_IMPLEMENTATION
#include "../tools/stb_image.h"
#define __STDC_LIB_EXT1__ // ����stb_image_write.h��C4996����δʹ��_s����
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../tools/stb_image_write.h"

// static �ؼ���ֻ�������ඨ�����ڲ��������У�����ʱ���ܱ�ʾΪ static
const std::string Image::outputPath = "../output/";

Image::Image(std::string imageName, int imageWidth, int imageHeight, int channel)
	: imagePath(outputPath + imageName), imageWidth(imageWidth), imageHeight(imageHeight), channel(channel)
{
	// ʹ��stbi_image_free()�ͷţ�������new
	imageData = (unsigned char*)malloc(this->imageWidth * this->imageHeight * this->channel); // ��ʼ��ͼƬ�ڴ�
}

// ��������
void Image::setPixel(int row, int col, int r, int g, int b)
{
	imageData[(row * imageWidth + col) * channel] = r;
	imageData[(row * imageWidth + col) * channel + 1] = g;
	imageData[(row * imageWidth + col) * channel + 2] = b;
}

void Image::setPixel(int row, int col, color c)
{
	setPixel(row, col, (int)c.e[0], (int)c.e[1], (int)c.e[2]);
}

// д��ָ��ͼƬ
void Image::write()
{
	stbi_write_png(imagePath.c_str(), imageWidth, imageHeight, channel, imageData, 0);
}

Image::~Image()
{
	LOG("free the image");
	stbi_image_free(imageData);
}
