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

// static�ؼ���ֻ�������ඨ�����ڲ��������У�����ʱ���ܱ�ʾΪstatic
const std::string ImageWrite::kOutputPath_ = "../output/";

ImageWrite::ImageWrite(std::string imageName, int image_width, int image_height, int channel)
	: image_path_(kOutputPath_ + imageName), width_(image_width), height_(image_height), channel_(channel)
{
	// ʹ��stbi_image_free()�ͷţ���˲���new
	image_data_ = (unsigned char*)malloc(this->width_ * this->height_ * this->channel_); // ��ʼ��ͼƬ�ڴ�
}

// ��������
void ImageWrite::set_pixel(const int& row, const int& col, const int& r, const int& g, const int& b)
{
	image_data_[(row * width_ + col) * channel_] = r;
	image_data_[(row * width_ + col) * channel_ + 1] = g;
	image_data_[(row * width_ + col) * channel_ + 2] = b;
}

// ٤��У��
inline double linear_to_gamma(double linear_component)
{
	// ����ͨ��٤��ֵ2.2
	return pow(linear_component, 1 / 2.2);
}

void ImageWrite::set_pixel(const int& row, const int& col, Color c, const int& samples_per_pixel)
{
	// һ�����ز������ξ͵����˼�����ɫ�����ݲ����������Ż�ȥ
	double scale = 1. / samples_per_pixel;
	c *= scale;

	// ٤��У��
	double r = linear_to_gamma(c.x());
	double g = linear_to_gamma(c.y());
	double b = linear_to_gamma(c.z());
	
	// ��ֵ������[0,1]�����ŵ�[0,256)��������ȡ��
	r = std::clamp(r, 0., 1.) * 255.999;
	g = std::clamp(g, 0., 1.) * 255.999;
	b = std::clamp(b, 0., 1.) * 255.999;
	set_pixel(row, col, (int)r, (int)g, (int)b);
}

// д��ָ��ͼƬ
void ImageWrite::write()
{
	stbi_write_png(image_path_.c_str(), width_, height_, channel_, image_data_, 0);
}

ImageWrite::~ImageWrite()
{
	LOG("free the image");
	stbi_image_free(image_data_);
}

const std::string ImageRead::kInputPath_ = "../texture/";

ImageRead::ImageRead(std::string image_name) : channel_(3)
{
	image_data_ = stbi_load(image_name.c_str(), &width_, &height_, &channel_, channel_);
}

Color ImageRead::get_pixel(const int& row, const int& col) const
{
	unsigned char r = image_data_[(row * width_ + col) * channel_];
	unsigned char g = image_data_[(row * width_ + col) * channel_ + 1];
	unsigned char b = image_data_[(row * width_ + col) * channel_ + 2];

	auto scale = [](unsigned char x) -> double
	{
		return x / 255.;
	};
	return Color(scale(r), scale(g), scale(b));
}
