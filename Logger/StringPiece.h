#pragma once
#include<string>
#include <iosfwd>
#include "types.h"

/*
�ɵ�C++ͷ�ļ�������<iostream.h>
�µ�C++ͷ�ļ�������<iostream>�����������ɵ�ͷ�ļ���ͬ�����ǵ�ͷ�ļ��������������ռ�std�У�
��׼C��ͷ�ļ���<stdio.h>��ͷ�ļ����ݲ���std��
����C�⹦�ܵ��µ�C++ͷ�ļ�����<cstdio>���������֣�������ɵ�Cͷ�ļ���ͬ�������������������ռ�std��

<string.h>��Ӧ���Ǿɵ�Cͷ�ļ�����Ӧ���ǻ���char*���ַ�������
<string>�ǰ�����std���µ�C++ͷ�ļ�����Ӧ�����µ�string��
<cstriing>�ǰ�����std�еľɵ�Cͷ�ļ�

*/

/*
C++��Ա�����ĳ�ʼ��˳��
	1.��ͨ������
		��Ա������ʹ�ó�ʼ���б�ʱ���빹�캯���ĳ�ʼ���б�˳���޹أ�ֻ���Ա�����Ķ���˳���йأ���Ϊ��Ա�����ĳ�ʼ���������ݱ������ڴ��еĴ��򣬶��ڴ��е�˳�����ڱ����ھ�ȷ���ˣ���
		�����ʹ�ó�ʼ���б��ڹ��캯���ڲ���ʼ��ʱ����ʱ���Ա�����ڹ��캯���е�˳���й�ϵ
	2.���const��Ա����
		const��Ա���������ڳ�ʼ���б��н��г�ʼ��
	3.���static��Ա���������������ʼ��

	---����ľ�̬������ȫ�ֱ���---
	---������ľ�̬������ȫ�ֱ���---
	---����ĳ�Ա����---
	---������ĳ�Ա����---
*/

/*
memecmp();�Ա��ַ����Ĵ�С��ǰ��������Ϊ ��Ҫ�ȽϵĴ���������Ϊ��Ҫ�Աȵĳ���
*/

/*
�������������������(<<   >>)
�������������ֻ����ȫ�ֺ����ķ�ʽ���У���Ϊ������ostream��istream���б�д��Ա������(��Ԫ��Ӧ�ó���)
��Ԫ��������Ҫ���ʸ���ķǹ��к�������������������ߺ���Ϊ������Ԫ��ʵ�֣���Ԫ����ֱ�ӷ������˽�г�Ա�����Ƕ������������ͨ�������������κ��࣬����Ҫ�����м�������friend��
class Test{
	friend std::ostream operator<<(std::ostream&os,const Test&test);//���Ƕ������������ͨ�������������κ���
	friend std::istream operator>>(std::istream &os,const Test&test);
};

*/
namespace Logger {
	class StringArg {
	public:
		StringArg(const char*str)//const char*
			:str_(str) {

		}
		StringArg(const std::string &str)//const string&
			:str_(str.c_str()) {

		}

		const char*c_str()const {
			return str_;
		}

	private:
		const char *str_;
	};

	class StringPiece {
	private:
		const char*ptr_;
		int length_;

	public:
	/***���캯��***/
		StringPiece()
			:ptr_(NULL),
			length_(0)
		{}

		StringPiece(const char*str)
			:ptr_(str),
			length_(static_cast<int>(strlen(ptr_)))
		{}

		StringPiece(const unsigned char*str)
			:ptr_(reinterpret_cast<const char*>(str)),
			length_(static_cast<int>(strlen(ptr_)))
		{}

		StringPiece(const std::string &str)
			:ptr_(str.data()),
			length_(static_cast<int>(str.size()))
		{}

		StringPiece(const char*offset,int len)
			:ptr_(offset),
			length_(len)
		{}

/***��Ա����***/
		const char*data() const{ return ptr_; }

		int size() const{ return length_; }

		bool empty() const{ return length_ == 0; }

		const char* begin() const { return ptr_; }

		const char* end() const { return ptr_ + length_; }

		void clear() { ptr_ = NULL; length_ = 0; }

		void set(const char*buffer, int len) { ptr_ = buffer; length_ = len; }

		void set(const char*str) {
			ptr_ = str;
			length_ = static_cast<int>(strlen(str));
		}

		void set(const void *buffer, int len) {
			ptr_ = reinterpret_cast<const char*>(buffer);
			length_ = len;
		}

		char operator[](int i)const {
			return ptr_[i];
		}

		void remove_prefix(int n) {//��ǰ����ض�n���ַ�
			ptr_ += n;
			length_ -= n;
		}

		void remove_suffix(int n) {
			length_ -= n;
		}

		bool operator==(const StringPiece &x)const {
			return (x.length_ == length_) && (memcmp(x.ptr_, ptr_,length_));
		}

		bool operator!=(const StringPiece &x)const {//ͬһ������
			return !(x==(*this));
		}
		//�ж�һ���ǲ���һ�����Ӽ�
		#define STRINGPIECE_BINARY_PREDICATE(cmp,auxcmp)					 \
		bool operator cmp(const StringPiece &x)const {								\
			int r=memcmp(ptr_,x.ptr_,length_<x.length_?length_:x.length_);		\
			return ((r auxcmp 0 )||((r==0)&&(length_ cmp x.length_ )));			\
		}

		STRINGPIECE_BINARY_PREDICATE(< , < );
		STRINGPIECE_BINARY_PREDICATE(<= , < );
		STRINGPIECE_BINARY_PREDICATE(>= , > );
		STRINGPIECE_BINARY_PREDICATE(> , > );

		#undef STRINGPIECE_BINARY_PREDICATE

		int compare(const StringPiece &x)const {//�Աȣ���ȷ���0���Լ�С����-1���Լ��󷵻�+1
			int r = memcmp(ptr_, x.ptr_, length_ < x.length_ ? length_ : x.length_);
			if (r == 0) {//֮ǰ�ĶԱȽ��������ͬ
				if (length_ < x.length_)//�����Ǹ���
					return -1;
				else if (length_ > x.length_)
					return +1;
			}
			return r;
		}

		std::string as_string() const {//��Ϊstring����
			return std::string(data(),size());
		}

		void CopyToString(std::string *target)const {//ת��Ϊ�ַ���
			target->assign(data(), size());
		}

		bool starts_with(const StringPiece &x)const {
			return ((length_ >= x.length_) && (memcmp(ptr_, x.ptr_, length_)==0));
		}
	};
}

std::ostream operator<<(std::ofstream &o, const Logger::StringPiece &piece);