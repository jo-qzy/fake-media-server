//
// Created by BoringWednesday on 2021/5/9.
//

#ifndef FAKE_MEDIA_SERVER_LOG_H
#define FAKE_MEDIA_SERVER_LOG_H


#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <sstream>

using namespace std;

#define FILENAME ((const char *) __FILE__ + SOURCE_PATH_SIZE)

#define LOG(level)      \
    Log(FILENAME, __LINE__, LOG_##level)

static const char *level_str[] = {
    "DEBUG", "INFO", "WARN",
    "ERROR", "FATAL"
};

enum LogLevel {
    LOG_DEBUG = 0,
    LOG_INFO = 1,
    LOG_WARN = 2,
    LOG_ERROR = 3,
    LOG_FATAL = 4
};

static LogLevel min_level = LOG_INFO;

static void set_log_level(LogLevel level)
{
    min_level = level;
}

class Log
{
public:
    Log(const char *file, int line, LogLevel level)
        : level(level)
    {
        if (level >= min_level) {
            os << level_str[level]
               << " " << file << ":" << line
               << " " << getpid() << "] ";
        }
    }

    ~Log()
    {
        if (level >= min_level) {
            os << endl;
            cout << os.str();

            flush(cout);
        }
    }

public:
    template<typename T>
    Log &operator<<(const T &t)
    {
        if (level >= min_level) {
            os << t;
        }

        return *this;
    }

private:
    LogLevel        level;
    ostringstream   os;

};


#endif //FAKE_MEDIA_SERVER_LOG_H
