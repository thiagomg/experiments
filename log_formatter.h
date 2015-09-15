#pragma once

#include <chrono>
#include <ctime>

namespace BTL {
	namespace Log {

		template<typename T, typename Stream>
		struct log_formatter {
			log_formatter(Stream &st, T &t) {
				st << t;
			}
		};

		template<typename Stream>
		struct log_formatter<int, Stream> {
			log_formatter(Stream &st, int &t) {
				st << t << " ";
			}
		};

		template<typename T, typename U, typename Stream>
		struct log_formatter< std::tuple<T, U>, Stream> {
			log_formatter(std::stringstream &ss, std::tuple<T, U> &t) {
				const auto &v = std::get<0>(t);
				auto f = std::get<1>(t);
				ss << f(v);
			}
		};
		
		template <typename TM, typename Duration>
		void print_timed(const TM &t, Duration fraction, const char *level, const std::string &text) {
			
			using namespace std::chrono;
			
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
		
		void print_timed_now(const char *level, const std::string &text) {
	
			using namespace std::chrono;

			system_clock::time_point now = system_clock::now();
			system_clock::duration tp = now.time_since_epoch();

			tp -= duration_cast<seconds>(tp);
			time_t tt = system_clock::to_time_t(now);

			std::tm cur_time = *localtime(&tt);
			print_timed<std::tm, system_clock::duration>(cur_time, tp, level, text);

		}
		
	}
}