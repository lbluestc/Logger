#pragma once
#ifndef COPYABLE_H
#define COPYABLE_H

namespace Logger {
	class copyable {//可以拷贝的函数，将构造函数设置为protected 为了防止直接构造该对象
	protected:
		copyable() = default;
		~copyable() = default;
	};
}

#endif