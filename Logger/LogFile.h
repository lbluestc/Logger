#pragma once

#include"types.h"
#include "noncopyable.h"
#include <memory>
#include <string>
//#include <thread>
#include <mutex>

namespace Logger {
	namespace FileUtil {
		class AppendFile;
	}
	class LogFile :nocopyable {
	public:
		LogFile(const std::string &basename, off_t rollSize, bool threadSafe = true, int flushinterval = 3, int checkEveryN = 1024);
		~LogFile();

		void append(const char*logline, int len);
		void flush();
		bool rollFile();

	private:
		void append_unlock(const char*logline, int len);
		static std::string getLogFileName(const std::string &basename,time_t *now);

		const std::string basename_;
		const off_t rollsize_;
		const int flushinterval_;
		const int checkEveryN_;

		int count_;

		std::unique_ptr<MutexLock> mutex_;
		time_t startOfPeriod_;
		time_t lastRoll_;
		time_t lastFlush_;
		std::unique_ptr<FileUtil::AppendFile>file_;

		const static int kRollPerSeconds_ = 60 * 60 * 24;
	};
}