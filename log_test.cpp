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

#include <iomanip>
#include <ctime>

using namespace std;
using namespace BTL::Log;

using Logger = LogCallback<std::stringstream>;

template<typename StreamPolicy>
LogCallback<StreamPolicy> *LogCallback<StreamPolicy>::_inst = nullptr;

int main() {

	Logger::init(Level::ALL, [](int level, const std::string &text) {
		time_t t = std::time(nullptr);
		std::tm tm = *std::localtime(&t);
		
		std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S - ") << level << ": " << text << std::endl;
	});

	Logger::trace(1, 2, 3, 4, "asdasdas", 1);
	Logger::trace("thiago","massari ","guedes",1,2,3,"asd");
	
	auto fmt_val = [](int val) -> std::string {
		char buf[64];
		sprintf(buf, "0x%X ", val);
		return buf;
	};

	Logger::trace(1, 2, 3, _TP(4, fmt_val), 5);
	
	return 0;
}
