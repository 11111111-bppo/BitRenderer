#include "image.h"

#include <iostream>
#include <string>
#include <new>

// ����ͷ�ļ������������壬���ܰ�����image.h�У�����image.h������ʱ�ᵼ���ض������LNK2005��LNK1169
#define STB_IMAGE_IMPLEMENTATION
#include "../tools/stb_image.h"
#define __STDC_LIB_EXT1__ // ����stb_image_write.h��C4996����δʹ��_s����
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../tools/stb_image_write.h"

#include "logger.h"

// static �ؼ���ֻ�������ඨ�����ڲ��������У�����ʱ���ܱ�ʾΪ static
const std::string Image::output_path = "../output/";

Image::Image(std::string imageName, int image_width, int image_height, int channel)
	: image_path(output_path + imageName), image_width(image_width), image_height(image_height), channel(channel)
{
	// ʹ��stbi_image_free()�ͷţ�������new
	image_data = (unsigned char*)malloc(this->image_width * this->image_height * this->channel); // ��ʼ��ͼƬ�ڴ�
}

// ��������
void Image::set_pixel(int row, int col, int r, int g, int b)
{
	image_data[(row * image_width + col) * channel] = r;
	image_data[(row * image_width + col) * channel + 1] = g;
	image_data[(row * image_width + col) * channel + 2] = b;
}

void Image::set_pixel(int row, int col, Color c)
{
	c.rescale();
	set_pixel(row, col, (int)c.e[0], (int)c.e[1], (int)c.e[2]);
}

// д��ָ��ͼƬ
void Image::write()
{
	stbi_write_png(image_path.c_str(), image_width, image_height, channel, image_data, 0);
}

Image::~Image()
{
	LOG("free the image");
	stbi_image_free(image_data);
}
