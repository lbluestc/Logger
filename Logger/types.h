#pragma once
#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <cstddef>
#include <cstdio>
//#define NODEBUG //必须在assert包含之前定义
#include <cassert>//assert(param) 当参数为false时终止程序的调用，这些都是在DEBUG模式下采用作用，如果是NODEBUG模式则不起作用，需要在include<assert>之前添加宏定义
namespace Logger {
	//using std::string;

	inline void memZero(void *p, size_t n) {//将指针初始化为空
		memset(p, 0, n);
	}

	/*
	强制类型转换的区别：
	C风格的强制类型转换为 TypeA ver=(TypeA)ver1;
	C++提供了四种强制类型转换的方式：
	const_cast   static_cast  dynamic_cast  reinterpreter_cast

	const_cast: 去掉类型的const 属性或者volatile（变量是易变得，不要从寄存器取值，每次都读取新的值）属性

	static_cast:类似于C风格的强制转换，无条件转换，静态类型转换，（没有运行时类型检查来保证他的安全性，只在编译期进行类型坚持）
		1.基类与子类之间的指针和引用的转换（子类转父类安全，父类转子类不安全）
			子类指针转换为父类指针是安全的，但是父类指针转换为子类是不安全的（基类与子类的转换建议使用dynamic_cast）；
		2.基本数据类型的转换
			static_cast不能进行无关类型之间的转换（两个完全没有关系的对象不能相互转化）；
		3.把空指针转换为需要类型的指针
		4.把任何类型的表达式转换为void类型
		5.static_cast不能去掉表达式的const 和 volatile属性（用const_cast）；

	dynamic_cast:有条件转换，只针对指针和引用，动态类型转换（运行时安全检查,转换失败时返回NULL）
		1.安全的基类和子类类型转换
		2.必须要有虚函数（基类必须要有虚函数，保持多态性才能使用dynamic_cast）--（运行时类型的检查需要运行时类型信息，这些信息存储在虚函数表，只有定义了虚函数的类才有虚函数表）
		3.相同基类的不同子类相互转换，但是其结果为NULL

	reinterpreter_cast：用于进行无关类型的相互转换
	*/

	template<typename To,typename From>
	inline To implicate_cast(From const &f) {//类型转换
		return f;
	}

	template<class To,class From>
	inline To down_cast(From*f) {
		return static_cast<To>(f);
	}
}

#endif;