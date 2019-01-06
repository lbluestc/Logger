#pragma once

#include "types.h"
#include "StringPiece.h"
#include "noncopyable.h"

/*
C++�ļ̳з�ʽ��
	private�̳У��ü̳�ʹ�û�������г�Ա�������еķ���Ȩ�ޱ�Ϊprivate��
	public�̳У�����ı�����Ա�����������еķ���Ȩ��
	protected�̳У�ʹ�û����public��Ա������ķ���Ȩ�ޱ�Ϊpritected��
*/

namespace Logger {
	namespace detail {
		const int kSmallBuffer = 4000;
		const int kLargerBuffer = 4000 * 1000;

		template<int SIZE>//ģ�����
		class FixedBuffer :nocopyable {//Ĭ����private�̳У�
		private:
			char data_[SIZE];//��������
			char*cur_;//��ǰ��д��λ��
			void(*cookie_)();


		public:
			FixedBuffer()
				:cur_(data_) {
				setCookie(cookieStart);
			}

			~FixedBuffer() {
				setCookie(cookieEnd);
			}

			void setCookie(void(*cookie)()) {
				cookie_ = cookie;
			}

			int avail()const {//�ܹ����ɵ��ַ���Ŀ
				return static_cast<int>(end() - cur_);//����ָ��ָ��ͬһ����ֵ����������õ���ֵΪ����֮���Ԫ����Ŀ
			}

			void append(const char* buf, int len) {
				if (implicate_cast<size_t>(avail()) >len) {//����ʹ�õĿռ��㹻׷��
					memcpy(cur_, buf, len);
					cur_ += len;
				}
			}

			const char*data()const { return data_; }

		private:
			static void cookieStart();
			static void cookieEnd();

			const char*end()const {
				return data_ + sizeof data_;//sizeof ����������ͣ����������ţ��������ʽһ������Բ�������
			}
		};
	}
}