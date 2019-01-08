#pragma once

#include "types.h"
#include "StringPiece.h"
#include "noncopyable.h"

/*
C++的继承方式：
	private继承：该继承使得基类的所有成员在子类中的访问权限变为private的
	public继承：不会改变基类成员对象在子类中的访问权限
	protected继承：使得基类的public成员在子类的访问权限变为pritected的
*/

/*
typedef 的作用域可以在类中，也可以的在函数中
在类中如果是在private下，则只有该类可以使用，如果是public的，其他类亦可，使用方式为：Test::index;
在类中使用会有先后顺序的限制

在函数中，其作用域也就在函数体中。
*/

/*
#define PI 3.14 和 const double PI =3.14 的区别
前者只是进行简单的替换，后者还需要进行一系列类型的检查

*/

/*
static_assert(常量表达式,提示字符串)
C++11静态断言，用于做编译期间的断言；
用编译器强制保证契约，尤其是用于模板的时候；
编译器遇到static_assert的时候 通常会立即计算表达式的值，但是如果表达式依赖于某些模板参数，则会延迟到模板实例化时在进行演算；
这就让检查模板参数成为了可能。
由于是编译期间的断言，因此不会造成任何运行期间的损失；
*/

/*
std::numeric_limits<type>..
https://blog.csdn.net/wordwarwordwar/article/details/39344131
在C++11中 numeric_limits为模板类，在编译平台提供极值等信息，取代传统C语言的预处理常数；
常用的使用是对于给定的基础类型用来判断在当前系统上的最大值和最小值等。
min() 返回可取的最小值
digits10() 返回目标类型在十进制下可以表示的最大位数
epsilon()  返回目标数据类型表示的最逼近1的正数和1的绝对值
*/
namespace Logger {
	namespace detail {
		const int kSmallBuffer = 4000;//最小缓存大小
		const int kLargerBuffer = 4000 * 1000;//最大缓存大小

	/*-----------固定大小的Buffer------------*/
		template<int SIZE>//模板参数
		class FixedBuffer :nocopyable {//默认是private继承，
		private:
			char data_[SIZE];//整个数组
			char*cur_;//当前可写的位置
			void(*cookie_)();//函数指针--返回空，参数空


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

			int avail()const {//能够容纳的字符数目
				return static_cast<int>(end() - cur_);//两个指针指向同一个数值，他们相减得到的值为他们之间的元素数目
			}

			void append(const char* buf, int len) {
				if (implicate_cast<size_t>(avail()) >len) {//可以使用的空间足够追加
					memcpy(cur_, buf, len);
					cur_ += len;
				}
			}

			const char*data()const { return data_; }

			int length()const { return static_cast<int>(cur_ - data_); }//当前已经使用的长度

			char * current() { return cur_; }//当前可写的位置

			void add(size_t len) { cur_ += len; }//添加偏移量

			void reset() { cur_ = data_; }//将已经赋值的数据重新回到初始状态

			void bzero() { memZero(data_, sizeof(data_)); }//重置为空

			const char* debugString();

			std::string toString()const { return std::string(data_, length()); }//将已经有的值转换为string

			StringPiece toStringPiece()const { return StringPiece(data_, length()); }


		private:
			static void cookieStart();
			static void cookieEnd();

			const char*end()const {
				return data_ + sizeof data_;//sizeof 如果计算类型，则必须带括号，如果与表达式一起则可以不用括号
			}
		};

		/*---------日志流----------*/
		class LogStream :nocopyable {
			typedef LogStream self;
			typedef detail::FixedBuffer<detail::kSmallBuffer> Buffer;//小的缓存片
		private:
			void staticCheck();//编译期间对kMaxNumericSize进行检查

			template<class T>
			void formatInteger(T);
			static const int kMaxNumericSize = 32;
			Buffer buffer_;
		public:
			self& operator<<(bool v);
			self& operator<<(short v);
			self& operator<<(unsigned short v);
			self& operator<<(int v);
			self& operator<<(unsigned int v);
			self& operator<<(long v);
			self& operator<<(unsigned long v);
			self& operator<<(long long v);
			self& operator<<(unsigned long long v);
			self& operator<<(const void *v);
			self& operator<<(float v);
			self& operator<<(double v);
			self& operator<<(char v);
			self& operator<<(const char* v);
			self& operator<<(const unsigned char* v);
			self& operator<<(const std::string& v);
			self& operator<<(const StringPiece &v);
			self& operator<<(const Buffer& v);

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