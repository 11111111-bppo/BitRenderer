/*
* ��־��
* ���������־
*/
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#include "singleton.h"

#define PRINT_LOCATION std::cout << __FILE__ << "��" << __LINE__ << "�У�" << __FUNCTION__ << "����";
#define LOG(X) { PRINT_LOCATION Logger::get_instance().print_info(X); }

class Logger : public Singleton<Logger>
{
public:
	
	inline void print_info(std::string info)
	{
		std::cout << info << std::endl;
	}

	// C++17
	template <typename... Types>
	inline void print_info(const Types... args)
	{
		(std::cout << ... << args) << std::endl;
	}
};

#endif // !LOGGER_H
