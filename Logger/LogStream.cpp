/*
Logstream��C++stream�������������Ȼ�����ؿ����ַ�����ʽ��������͵�һ���ԣ��������Ͱ�ȫ��
LogStream��������C++�����������cout��cout��������նˣ���LogStream�ǽ���������Ӽ��Ļ������У����Խ���
�ض��򵽱�׼������ļ�����socket�С�
*/

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

		template class FixedBuffer<kSmallBuffer>;//ģ���������
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
	//std::numeric_limits<double>::digits10 ��ʾ��ʮλ��
	//std::numeric_limits<double>::radix��ʾ ָ��
	static_assert(kMaxNumericSize - 10 > std::numeric_limits<double>::digits10, "kMaxNumericSize is large enough");
	static_assert(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10, "kMaxNumericSize is large enough");
	static_assert(kMaxNumericSize - 10 > std::numeric_limits<long>::digits10, "kMaxNumericSize is large enough");
	static_assert(kMaxNumericSize - 10 > std::numeric_limits<long long>::digits10, "kMaxNumericSize is large enough");
}

template<class T>
void LogStream::formatInteger(T v) {
	if (buffer_.avail() >= kMaxNumericSize) {
		size_t len = convert(buffer_.current(), v);//����ת��Ϊ�ַ���
		buffer_.add(len);
	}
}

LogStream& LogStream::operator<<(bool v) {
	buffer_.append(v ? "1": "0",1);
	return *this;
}

LogStream& LogStream::operator<<(short v) {
	*this << static_cast<int>(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned short v) {
	*this << static_cast<unsigned int>(v);
	return *this;
}

LogStream& LogStream::operator<<(int v) {
	formatInteger(v);
	return *this;
}

LogStream & LogStream::operator<<(unsigned int v)
{
	formatInteger(v);
	return *this;
}

LogStream & Logger::detail::LogStream::operator<<(long v)
{
	formatInteger(v);
	return *this;
}

LogStream & Logger::detail::LogStream::operator<<(unsigned long v)
{
	formatInteger(v);
	return *this;
}

LogStream & Logger::detail::LogStream::operator<<(long long v)
{
	formatInteger(v);
	return *this;
}

LogStream & Logger::detail::LogStream::operator<<(unsigned long long v)
{
	formatInteger(v);
	return *this;
}

LogStream & Logger::detail::LogStream::operator<<(const void * v)//����תΪ16����
{
	uintptr_t p = reinterpret_cast<uintptr_t>(v);
	if (buffer_.avail() > kMaxNumericSize) {
		char*buff = buffer_.current();
		buff[0] = '0';
		buff[1] = 'x';
		size_t len = convertHex(buff+2, p);
		buffer_.add(len);
	}
	return *this;
}

LogStream & Logger::detail::LogStream::operator<<(float v)
{
	*this << static_cast<double>(v);
	return *this;
}

LogStream & Logger::detail::LogStream::operator<<(double v)
{
	if (buffer_.avail() >= kMaxNumericSize) {
		int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", v);
		buffer_.add(len);
	}
	return *this;
}

LogStream & Logger::detail::LogStream::operator<<(char v)
{
	buffer_.append(&v, 1);
	return *this;
}

LogStream & Logger::detail::LogStream::operator<<(const char * v)
{
	if (v) {
		buffer_.append(v, strlen(v));
	}
	else {
		buffer_.append("(null)", 6);
	}
	return *this;
}

LogStream & Logger::detail::LogStream::operator<<(const unsigned char * v)
{
	*this << reinterpret_cast<const char*>(v);
	return *this;
}

LogStream & Logger::detail::LogStream::operator<<(const std::string & v)
{
	*this << v.data();
	return *this;
}

LogStream & Logger::detail::LogStream::operator<<(const StringPiece & v)
{
	buffer_.append(v.data(), v.size());
	return *this;
}

LogStream & Logger::detail::LogStream::operator<<(const Buffer & v)
{
	*this << v.toString();
	return *this;
}
