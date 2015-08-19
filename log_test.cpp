/* 
 * File:   log_test.cpp
 * Author: Thiago Massari Guedes
 *
 * Created on 17 de Maio de 2015, 23:38
 */
 
#include "Log.h"

#include <string>
#include <sstream>
#include <utility>

#include <functional>
#include <iostream>

#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace BTL::Log;

using Logger = LogCallback<std::stringstream>;

template<typename StreamPolicy>
LogCallback<StreamPolicy> *LogCallback<StreamPolicy>::_inst = nullptr;

LogLevelFmt BTL::Log::LogLevelFmt::_inst;

template <typename Duration>
void print_time(const tm &t, Duration fraction, const char *level, const std::string &text) {
    
    std::printf("%04u-%02u-%02u %02u:%02u:%02u.%03u - %s: %s\n", t.tm_year + 1900,
                t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec,
                static_cast<unsigned>(fraction / milliseconds(1)),
				level, text.c_str());

    // VS2013's library has a bug which may require you to replace
    // "fraction / milliseconds(1)" with
    // "duration_cast<milliseconds>(fraction).count()"
	
	//Because I'm lazy about writing a print_time myself, I got this one from:
	//http://stackoverflow.com/questions/27136854/c11-actual-system-time-with-milliseconds?rq=1
}

int main() {

	Logger::init(Level::ALL, [](int level, const std::string &text) {
		//std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S - ") << level << ": " << text << std::endl;
		system_clock::time_point now = system_clock::now();
		system_clock::duration tp = now.time_since_epoch();

		tp -= duration_cast<seconds>(tp);
		time_t tt = system_clock::to_time_t(now);

		std::tm cur_time = *localtime(&tt);
		print_time<system_clock::duration>(cur_time, tp, LogLevelFmt::getLevel(level), text);
	});

	Logger::audit(1, 2, 3, 4, "asdasdas", 1);
	Logger::trace("thiago","massari ","guedes",1,2,3,"asd");
	
	auto fmt_val = [](int val) -> std::string {
		char buf[64];
		sprintf(buf, "0x%X ", val);
		return buf;
	};

	Logger::error(1, 2, 3, _TP(4, fmt_val), 5);
	
	return 0;
}
