/*
* ��־��
* ���������־
*/
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <memory>

#include "singleton.h"

#define PRINT_LOCATION std::cout << __FILE__ << "��" << __LINE__ << "�У�" << __FUNCTION__ << "����";
#define LOG(X) PRINT_LOCATION Logger::getInstance().printInfo(X);

class Logger : public Singleton<Logger>
{
public:
	// �����Ϣ
	inline void printInfo(std::string info)
	{
		std::cout << info << std::endl;
	}
};

#endif // !LOGGER_H
