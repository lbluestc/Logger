#pragma once
#ifndef COPYABLE_H
#define COPYABLE_H

namespace Logger {
	class copyable {//���Կ����ĺ����������캯������Ϊprotected Ϊ�˷�ֱֹ�ӹ���ö���
	protected:
		copyable() = default;
		~copyable() = default;
	};
}

#endif