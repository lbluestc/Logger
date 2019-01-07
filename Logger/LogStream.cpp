#include "LogStream.h"


using namespace Logger;
using namespace Logger::detail;

template<int SIZE>
void FixedBuffer<SIZE>::cookieStart() {

}

template<int SIZE>
void FixedBuffer<SIZE>::cookieEnd() {

}

template<int SIZE>
const char * FixedBuffer<SIZE>::debugString() {
	*cur_ = '\0';//Ω·Œ≤
	return data_;
}