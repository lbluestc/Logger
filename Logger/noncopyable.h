#pragma once
#ifndef NOCOPYABLE_H
#define NOCOPYABLE_H

namespace Logger{
	class nocopyable {
	public:
		nocopyable(const nocopyable&) = delete;//禁止拷贝
		void operator=(const nocopyable&) = delete;//禁止赋值

	protected://不希望用户直接构造这个对象，可以构造他的子类
		nocopyable() = default;
		~nocopyable() = default;
	};
}

/*
protected 函数在子类的内部是可以调用的
private 函数只有在本类的内部可以调用
*/

#endif
