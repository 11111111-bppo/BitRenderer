/*
* ����ģ����
* Meyers' Singleton��ȷ������ʹ��ǰ���ȱ���ʼ����Effective C++ ����4����
* ��static������������static�����ڲ���Ϊ�ֲ�static������
* C++ֻ�ܱ�֤��ͬһ���ļ���������static�����ĳ�ʼ��˳���������������˳��һ�£������ܱ�֤��ͬ���ļ��е�static�����ĳ�ʼ��˳��
*/
#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
class Singleton
{
public:

    static T& getInstance()
    {
        static T instance;
        return instance;
    }

protected:

    Singleton() = default;
    ~Singleton() = default;
    
private:

    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;
};

#endif // !SINGLETON_H


