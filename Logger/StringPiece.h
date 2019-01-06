#pragma once
#include<string>
#include <iosfwd>
#include "types.h"

/*
旧的C++头文件类似于<iostream.h>
新的C++头文件类似于<iostream>其基本功能与旧的头文件相同，但是但头文件的内容在命名空间std中，
标准C的头文件如<stdio.h>，头文件内容不再std中
具有C库功能的新的C++头文件具有<cstdio>这样的名字，内容与旧的C头文件相同，但是其内容在命名空间std中

<string.h>对应的是旧的C头文件，对应的是基于char*的字符串处理
<string>是包含了std的新的C++头文件，对应得是新的string类
<cstriing>是包含在std中的旧的C头文件

*/

/*
C++成员变量的初始化顺序：
	1.普通变量：
		成员变量在使用初始化列表时，与构造函数的初始化列表顺序无关，只与成员变量的定义顺序有关（因为成员变量的初始化次序依据变量在内存中的次序，而内存中的顺序早在编译期就确定了）；
		如果不使用初始化列表，在构造函数内部初始化时，此时与成员变量在构造函数中的顺序有关系
	2.类的const成员变量
		const成员变量必须在初始化列表中进行初始化
	3.类的static成员变量必须在类外初始化

	---基类的静态变量或全局变量---
	---派生类的静态变量或全局变量---
	---基类的成员变量---
	---派生类的成员变量---
*/

/*
memecmp();对比字符串的大小，前两个参数为 需要比较的串，第三个为需要对比的长度
*/

/*
输入输出操作符的重载(<<   >>)
输入输出操作符只能以全局函数的方式进行（因为不能在ostream和istream类中编写成员函数）(友元的应用场景)
友元：其他类要访问该类的非公有函数，可以令其他类或者函数为他的友元来实现，友元可以直接访问类的私有成员，他是定义在类外的普通函数，不属于任何类，但需要在类中加以声明friend，
class Test{
	friend std::ostream operator<<(std::ostream&os,const Test&test);//他是定义在类外的普通函数，不属于任何类
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
	/***构造函数***/
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

/***成员函数***/
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

		void remove_prefix(int n) {//从前往后截断n个字符
			ptr_ += n;
			length_ -= n;
		}

		void remove_suffix(int n) {
			length_ -= n;
		}

		bool operator==(const StringPiece &x)const {
			return (x.length_ == length_) && (memcmp(x.ptr_, ptr_,length_));
		}

		bool operator!=(const StringPiece &x)const {//同一个对象
			return !(x==(*this));
		}
		//判断一个是不是一个的子集
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

		int compare(const StringPiece &x)const {//对比，相等返回0，自己小返回-1，自己大返回+1
			int r = memcmp(ptr_, x.ptr_, length_ < x.length_ ? length_ : x.length_);
			if (r == 0) {//之前的对比结果两个相同
				if (length_ < x.length_)//长的那个大
					return -1;
				else if (length_ > x.length_)
					return +1;
			}
			return r;
		}

		std::string as_string() const {//作为string返回
			return std::string(data(),size());
		}

		void CopyToString(std::string *target)const {//转换为字符串
			target->assign(data(), size());
		}

		bool starts_with(const StringPiece &x)const {
			return ((length_ >= x.length_) && (memcmp(ptr_, x.ptr_, length_)==0));
		}
	};
}

std::ostream operator<<(std::ofstream &o, const Logger::StringPiece &piece);