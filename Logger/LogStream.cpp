#include "pch.h"
#include "LogStream.h"


using namespace Logger;
using namespace Logger::detail;
namespace Logger {
	namespace detail {
		const char digits[] = "9876543210123456789";//��ʵ�Ǹ��ַ������н�β��\0
		const char* zero = digits + 9;//��0��ʼ��ʮλ��
		static_assert(sizeof(digits) == 20, "wrong number of digits");
		const char digitsHex[] = "0123456789ABCDEF";
		static_assert(sizeof digitsHex == 17, "wrong number of digitsHex");

		template<class T>
		size_t convert(char buf[], T value) {//��Ч���������ַ�����ת��
			T i = value;
			char *p = buf;
			do {
				int lsd = static_cast<int>(i % 10);
				i /= 10;
				*p++ = zero[lsd];
			} while (i != 0);
			
			if (value < 0) {
				*p++ = '-';
			}
			*p = '\0';

			std::reverse(buf, p);
			return p - buf;
		}

		size_t convertHex(char buf[], uintptr_t value) {//��Ч�Ľ�����ת��Ϊʮ�������ַ���
			uintptr_t i= value;
			char *p = buf;
			do {
				int lsd = static_cast<int>(value % 16);
				i /= 16;
				*p++ = digitsHex[lsd];
			} while (i != 0);
			*p = '\0';

			std::reverse(buf, p);
			return p - buf;
		}

		template class FixedBuffer<kSmallBuffer>;//?????
		template class FixedBuffer<kLargerBuffer>;//?????
	}
}


template<int SIZE>
void FixedBuffer<SIZE>::cookieStart() {

}

template<int SIZE>
void FixedBuffer<SIZE>::cookieEnd() {

}

template<int SIZE>
const char * FixedBuffer<SIZE>::debugString() {
	*cur_ = '\0';//��β
	return data_;
}

void Logger::detail::LogStream::staticCheck()
{
	static_assert(kMaxNumericSize - 10 > std::numeric_limits<double>::digits10, "kMaxNumericSize is large enough");
	static_assert(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10, "kMaxNumericSize is large enough");
	static_assert(kMaxNumericSize - 10 > std::numeric_limits<long>::digits10, "kMaxNumericSize is large enough");
	static_assert(kMaxNumericSize - 10 > std::numeric_limits<long long>::digits10, "kMaxNumericSize is large enough");
}

template<class T>
void LogStream::formatInteger(T v) {
	if (buffer_.avail() >= kMaxNumericSize) {
		size_t len = convert(buffer_.current(), v);
		buffer_.add(len);
	}
}