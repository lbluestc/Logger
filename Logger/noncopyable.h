#pragma once
#ifndef NOCOPYABLE_H
#define NOCOPYABLE_H

namespace Logger{
	class nocopyable {
	public:
		nocopyable(const nocopyable&) = delete;//��ֹ����
		void operator=(const nocopyable&) = delete;//��ֹ��ֵ

	protected://��ϣ���û�ֱ�ӹ���������󣬿��Թ�����������
		nocopyable() = default;
		~nocopyable() = default;
	};
}

/*
protected ������������ڲ��ǿ��Ե��õ�
private ����ֻ���ڱ�����ڲ����Ե���
*/

#endif
