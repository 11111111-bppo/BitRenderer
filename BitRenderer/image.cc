#include "image.h"

#include <iostream>
#include <string>
#include <new>
#include <algorithm>

// ����ͷ�ļ������������壬���ܰ�����image.h�У�����image.h������ʱ�ᵼ���ض������LNK2005��LNK1169
#define STB_IMAGE_IMPLEMENTATION
#include "tool/stb_image.h"
#define __STDC_LIB_EXT1__ // ����stb_image_write.h��C4996����δʹ��_s����
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tool/stb_image_write.h"

#include "logger.h"

// static �ؼ���ֻ�������ඨ�����ڲ��������У�����ʱ���ܱ�ʾΪ static
const std::string Image::kOutputPath = "../output/";

Image::Image(std::string imageName, int image_width, int image_height, int channel)
	: image_path_(kOutputPath + imageName), image_width_(image_width), image_height_(image_height), channel_(channel)
{
	// ʹ��stbi_image_free()�ͷţ���˲���new
	image_data_ = (unsigned char*)malloc(this->image_width_ * this->image_height_ * this->channel_); // ��ʼ��ͼƬ�ڴ�
}

// ��������
void Image::set_pixel(int row, int col, int r, int g, int b)
{
	image_data_[(row * image_width_ + col) * channel_] = r;
	image_data_[(row * image_width_ + col) * channel_ + 1] = g;
	image_data_[(row * image_width_ + col) * channel_ + 2] = b;
}

// ٤��У��
inline double linear_to_gamma(double linear_component)
{
	// ����ͨ��٤��ֵ2.2
	return pow(linear_component, 1 / 2.2);
}

void Image::set_pixel(int row, int col, Color c, int samples_per_pixel)
{
	// һ�����ز������ξ͵����˼�����ɫ�����ݲ����������Ż�ȥ
	double scale = 1. / samples_per_pixel;
	c *= scale;

	// ٤��У��
	double r = linear_to_gamma(c.e_[0]);
	double g = linear_to_gamma(c.e_[1]);
	double b = linear_to_gamma(c.e_[2]);
	
	// ��ֵ������[0,1]�����ŵ�[0,256)��������ȡ��
	r = std::clamp(r, 0., 1.) * 255.999;
	g = std::clamp(g, 0., 1.) * 255.999;
	b = std::clamp(b, 0., 1.) * 255.999;
	set_pixel(row, col, (int)r, (int)g, (int)b);
}

// д��ָ��ͼƬ
void Image::write()
{
	stbi_write_png(image_path_.c_str(), image_width_, image_height_, channel_, image_data_, 0);
}

Image::~Image()
{
	LOG("free the image");
	stbi_image_free(image_data_);
}
