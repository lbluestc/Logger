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

namespace Logger {
	namespace detail {
		const int kSmallBuffer = 4000;
		const int kLargerBuffer = 4000 * 1000;

		template<int SIZE>//模板参数
		class FixedBuffer :nocopyable {//默认是private继承，
		private:
			char data_[SIZE];//整个数组
			char*cur_;//当前可写的位置
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

		private:
			static void cookieStart();
			static void cookieEnd();

			const char*end()const {
				return data_ + sizeof data_;//sizeof 如果计算类型，则必须带括号，如果与表达式一起则可以不用括号
			}
		};
	}
}