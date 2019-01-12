/*
Logstream是C++stream风格，用起来很自然，不必考虑字符串格式与参数类型的一致性，且是类型安全的
LogStream类类似于C++的输出流对象cout，cout是输出到终端，而LogStream是将其输出到子集的缓冲区中，可以将其
重定向到标准输出，文件或者socket中。
*/

#include "pch.h"
#include "LogStream.h"


using namespace Logger;
using namespace Logger::detail;
namespace Logger {
	namespace detail {
		const char digits[] = "9876543210123456789";//其实是个字符串，有结尾的\0
		const char* zero = digits + 9;//从0开始的十位数
		static_assert(sizeof(digits) == 20, "wrong number of digits");
		const char digitsHex[] = "0123456789ABCDEF";
		static_assert(sizeof digitsHex == 17, "wrong number of digitsHex");

		template<class T>
		size_t convert(char buf[], T value) {//有效的整数到字符串的转换
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

		size_t convertHex(char buf[], uintptr_t value) {//有效的将整数转换为十六进制字符串
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

		template class FixedBuffer<kSmallBuffer>;//模板的特例化
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
	*cur_ = '\0';//结尾
	return data_;
}

void Logger::detail::LogStream::staticCheck()
{
	//std::numeric_limits<double>::digits10 表示底十位数
	//std::numeric_limits<double>::radix表示 指数
	static_assert(kMaxNumericSize - 10 > std::numeric_limits<double>::digits10, "kMaxNumericSize is large enough");
	static_assert(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10, "kMaxNumericSize is large enough");
	static_assert(kMaxNumericSize - 10 > std::numeric_limits<long>::digits10, "kMaxNumericSize is large enough");
	static_assert(kMaxNumericSize - 10 > std::numeric_limits<long long>::digits10, "kMaxNumericSize is large enough");
}

template<class T>
void LogStream::formatInteger(T v) {
	if (buffer_.avail() >= kMaxNumericSize) {
		size_t len = convert(buffer_.current(), v);//整数转换为字符串
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

LogStream & Logger::detail::LogStream::operator<<(const void * v)//整数转为16进制
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
