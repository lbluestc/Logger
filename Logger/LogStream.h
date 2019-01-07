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

/*
typedef ����������������У�Ҳ���Ե��ں�����
�������������private�£���ֻ�и������ʹ�ã������public�ģ���������ɣ�ʹ�÷�ʽΪ��Test::index;
������ʹ�û����Ⱥ�˳�������

�ں����У���������Ҳ���ں������С�
*/

/*
#define PI 3.14 �� const double PI =3.14 ������
ǰ��ֻ�ǽ��м򵥵��滻�����߻���Ҫ����һϵ�����͵ļ��

*/
namespace Logger {
	namespace detail {
		const int kSmallBuffer = 4000;//��С�����С
		const int kLargerBuffer = 4000 * 1000;//��󻺴��С

		template<int SIZE>//ģ�����
		class FixedBuffer :nocopyable {//Ĭ����private�̳У�
		private:
			char data_[SIZE];//��������
			char*cur_;//��ǰ��д��λ��
			void(*cookie_)();//����ָ��--���ؿգ�������


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

			int length()const { return static_cast<int>(cur_ - data_); }//��ǰ�Ѿ�ʹ�õĳ���

			char * current() { return cur_; }//��ǰ��д��λ��

			void add(size_t len) { cur_ += len; }//���ƫ����

			void reset() { cur_ = data_; }//���Ѿ���ֵ���������»ص���ʼ״̬

			void bzero() { memZero(data_, sizeof(data_)); }//����Ϊ��

			const char* debugString();

			std::string toString()const { return std::string(data_, length()); }//���Ѿ��е�ֵת��Ϊstring

			StringPiece toStringPiece()const { return StringPiece(data_, length()); }


		private:
			static void cookieStart();
			static void cookieEnd();

			const char*end()const {
				return data_ + sizeof data_;//sizeof ����������ͣ����������ţ��������ʽһ������Բ�������
			}
		};

		class LogStream :nocopyable {
			typedef LogStream self;
			typedef detail::FixedBuffer<detail::kSmallBuffer> Buffer;//С�Ļ���Ƭ
		private:
			void staticCheck();

			template<class T>
			void formatInteger(T);

			Buffer buffer_;
		public:
			self& operator<<(bool v) {
				buffer_.append(v ? "1" : "0",1);
				return *this;
			}
		};
	}
}