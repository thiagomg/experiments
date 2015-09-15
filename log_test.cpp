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



using namespace std;
using namespace std::chrono;
using namespace BTL::Log;

//---- must be in log.cpp -----------------------------------------------
using Logger = log_callback<std::stringstream>; //get something better than stringstream

template<typename StreamPolicy>
log_callback<StreamPolicy> *log_callback<StreamPolicy>::_inst = nullptr;

level_format BTL::Log::level_format::_inst;

//=======================================================================

int main() {

	Logger::init(log_level::ALL, [](int level, const std::string &text) {
		const char *l = level_format::get_level(level);
		print_timed_now(l, text);
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
