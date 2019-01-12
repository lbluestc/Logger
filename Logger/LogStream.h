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

/*
static_assert(�������ʽ,��ʾ�ַ���)
C++11��̬���ԣ������������ڼ�Ķ��ԣ�
�ñ�����ǿ�Ʊ�֤��Լ������������ģ���ʱ��
����������static_assert��ʱ�� ͨ��������������ʽ��ֵ������������ʽ������ĳЩģ�����������ӳٵ�ģ��ʵ����ʱ�ڽ������㣻
����ü��ģ�������Ϊ�˿��ܡ�
�����Ǳ����ڼ�Ķ��ԣ���˲�������κ������ڼ����ʧ��
*/

/*
std::numeric_limits<type>..
https://blog.csdn.net/wordwarwordwar/article/details/39344131
https://en.cppreference.com/w/cpp/types/numeric_limits/digits10
��C++11�� numeric_limitsΪģ���࣬�ڱ���ƽ̨�ṩ��ֵ����Ϣ��ȡ����ͳC���Ե�Ԥ��������
���õ�ʹ���Ƕ��ڸ����Ļ������������ж��ڵ�ǰϵͳ�ϵ����ֵ����Сֵ�ȡ�
min() ���ؿ�ȡ����Сֵ
digits10() ����Ŀ��������ʮ�����¿��Ա�ʾ�����λ��
epsilon()  ����Ŀ���������ͱ�ʾ����ƽ�1��������1�ľ���ֵ
*/

/*
snprintf(char*des,size_t n,const char*format,...)����
���ɱ��������format�����ĸ�ʽ��ʽ��λ�ַ������ٿ�����des��
�����ʽ������ַ�����СС��n����ȫ��������des�У����ڽ�β������'\0',�����ʽ������ַ�������n����n-1���ȿ�����
des�У�����ĩβһλ���'\0'
format���ܣ�
https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/format-specification-syntax-printf-and-wprintf-functions?view=vs-2017
c�ַ���d������ʮ����������i������ʮ����������o�޷��Ű˽���������u�޷���ʮ��������
x�޷���ʮ������������X�޷���ʮ������������e������������������
*/
/*
size_t �� int������
size_t �����ϵͳ�йأ����С���Ա�ʾ����Ĵ�С
��32λ�£�size_t :  typedef unsigned int size_t  4�ֽ�
��64λ�£�size_t : typedef unsigned long size_t  8�ֽ�
*/

namespace Logger {
	namespace detail {
		const int kSmallBuffer = 4000;//��С�����С
		const int kLargerBuffer = 4000 * 1000;//��󻺴��С

	/*-----------�̶���С��Buffer------------*/
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

			void append(const char* buf, size_t len) {
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

		/*---------��־��----------*/
		class LogStream :nocopyable {
			typedef LogStream self;
			typedef detail::FixedBuffer<detail::kSmallBuffer> Buffer;//С�Ļ���Ƭ
		private:
			void staticCheck();//�����ڼ��kMaxNumericSize���м��

			template<class T>
			void formatInteger(T);
			static const int kMaxNumericSize = 32;//������������32λ
			Buffer buffer_;
		public:
			self& operator<<(bool v);//���벼��ֵ
			self& operator<<(short v);//���������
			self& operator<<(unsigned short v);//�����޷��Ŷ�����
			self& operator<<(int v);//��������
			self& operator<<(unsigned int v);//�����޷�������
			self& operator<<(long v);//���볤����
			self& operator<<(unsigned long v);//�����޷��ų�����
			self& operator<<(long long v);//����longlong��
			self& operator<<(unsigned long long v);
			self& operator<<(const void *v);//16������
			self& operator<<(float v);
			self& operator<<(double v);//���ָ�ʽ���󿽱�
			self& operator<<(char v);
			self& operator<<(const char* v);//ʮ������
			self& operator<<(const unsigned char* v);//�޷���16����
			self& operator<<(const std::string& v);
			self& operator<<(const StringPiece &v);//�ַ�������
			self& operator<<(const Buffer& v);//�̶���С������

			void append(const char*data, int len) {
				buffer_.append(data, len);
			}

			const Buffer& buffer()const {
				return buffer_;
			}

			void resetBuffer() {
				buffer_.reset();
			}
		};
	}
}