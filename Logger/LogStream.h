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
namespace Logger {
	namespace detail {
		const int kSmallBuffer = 4000;//最小缓存大小
		const int kLargerBuffer = 4000 * 1000;//最大缓存大小

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

		class LogStream :nocopyable {
			typedef LogStream self;
			typedef detail::FixedBuffer<detail::kSmallBuffer> Buffer;//小的缓存片
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