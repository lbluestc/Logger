#pragma once
#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <cstddef>
#include <cstdio>
//#define NODEBUG //������assert����֮ǰ����
#include <cassert>//assert(param) ������Ϊfalseʱ��ֹ����ĵ��ã���Щ������DEBUGģʽ�²������ã������NODEBUGģʽ�������ã���Ҫ��include<assert>֮ǰ��Ӻ궨��
namespace Logger {
	//using std::string;

	inline void memZero(void *p, size_t n) {//��ָ���ʼ��Ϊ��
		memset(p, 0, n);
	}

	/*
	ǿ������ת��������
	C����ǿ������ת��Ϊ TypeA ver=(TypeA)ver1;
	C++�ṩ������ǿ������ת���ķ�ʽ��
	const_cast   static_cast  dynamic_cast  reinterpreter_cast

	const_cast: ȥ�����͵�const ���Ի���volatile���������ױ�ã���Ҫ�ӼĴ���ȡֵ��ÿ�ζ���ȡ�µ�ֵ������

	static_cast:������C����ǿ��ת����������ת������̬����ת������û������ʱ���ͼ������֤���İ�ȫ�ԣ�ֻ�ڱ����ڽ������ͼ�֣�
		1.����������֮���ָ������õ�ת��������ת���లȫ������ת���಻��ȫ��
			����ָ��ת��Ϊ����ָ���ǰ�ȫ�ģ����Ǹ���ָ��ת��Ϊ�����ǲ���ȫ�ģ������������ת������ʹ��dynamic_cast����
		2.�����������͵�ת��
			static_cast���ܽ����޹�����֮���ת����������ȫû�й�ϵ�Ķ������໥ת������
		3.�ѿ�ָ��ת��Ϊ��Ҫ���͵�ָ��
		4.���κ����͵ı��ʽת��Ϊvoid����
		5.static_cast����ȥ�����ʽ��const �� volatile���ԣ���const_cast����

	dynamic_cast:������ת����ֻ���ָ������ã���̬����ת��������ʱ��ȫ���,ת��ʧ��ʱ����NULL��
		1.��ȫ�Ļ������������ת��
		2.����Ҫ���麯�����������Ҫ���麯�������ֶ�̬�Բ���ʹ��dynamic_cast��--������ʱ���͵ļ����Ҫ����ʱ������Ϣ����Щ��Ϣ�洢���麯����ֻ�ж������麯����������麯����
		3.��ͬ����Ĳ�ͬ�����໥ת������������ΪNULL

	reinterpreter_cast�����ڽ����޹����͵��໥ת��
	*/

	template<typename To,typename From>
	inline To implicate_cast(From const &f) {//����ת��
		return f;
	}

	template<class To,class From>
	inline To down_cast(From*f) {
		return static_cast<To>(f);
	}
}

#endif;