#include "pch.h"
#include "LogFile.h"

#include <assert.h>
#include <time.h>

using namespace Logger;

Logger::LogFile::LogFile(const std::string & basename, off_t rollSize, bool threadSafe, int flushinterval, int checkEveryN)
	:basename_(basename),rollsize_(rollSize),flushinterval_(flushinterval),checkEveryN_(checkEveryN),count_(0),mutex_(threadSafe?std::mutex():NULL),startOfPeriod_(0),
	lastFlush_(0),lastRoll_(0)
{
	assert(basename_.find('/') == std::string::npos);
	rollFile();
}

Logger::LogFile::~LogFile() = default;

void Logger::LogFile::append(const char * logline, int len)
{
	if (mutex_) {
		MutexLockGuard lock(*mutex_);
		append_unlock(logline, len);
	}
	else {
		append_unlock(logline, len);
	}
}

void Logger::LogFile::flush()
{
	if (mutex_) {
		MutexLockGuard lock(*mutex_);
		file_->flush();
	}
	else {
		file_->flush();
	}
}

bool Logger::LogFile::rollFile()
{
	time_t now = 0;
	std::string filename = getLogFileName(basename_, &now);
	time_t start = now / kRollPerSeconds_*kRollPerSeconds_;

	if (now > lastRoll_) {
		lastRoll_ = now;
		lastFlush_ = now;
		startOfPeriod_ = start;
		file_.reset(new FileUtil::AppendFile(filename));
		return true;
	}
	return false;
}

void Logger::LogFile::append_unlock(const char * logline, int len)
{
	file_->append(logline, len);
	if (file_->writtenBytes() > rollsize_) {
		rollFile();
	}
	else {
		++count_;
		if (count_ >= checkEveryN_) {
			count_ = 0;
			time_t now = ::time(NULL);
			time_t thisPeriod_ = now / kRollPerSeconds_*kRollPerSeconds_;
			if (thisPeriod_ != startOfPeriod_) {
				rollFile();
			}
			else if (now - lastFlush_ > flushinterval_) {
				lastFlush_ = now;
				file_->flush();
			}
		}
	}
}

std::string Logger::LogFile::getLogFileName(const std::string & basename, time_t * now)
{
	std::string filename;
	filename.reserve(basename.size() + 64);
	filename = basename;
	char timebuf[32];
	struct tm tm;
	*now = time(NULL);
	tm=*(gmtime(now));
	strftime(timebuf, sizeof timebuf, ".%Y%m%d-%H%M%S.", &tm);
	filename += timebuf;
	filename += ".log";
	return filename;
}
