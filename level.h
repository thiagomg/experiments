#pragma once

namespace BTL {
	namespace Log {
		
		enum class log_level : int {
			OFF = 9,
			ERROR = 8,
			AUDIT = 7,
			ADMIN = 6,
			WARNING = 5,
			INFO = 4,
			TRACE = 3,
			DEBUG = 2,
			VERBOSE = 1,
			ALL = 0
		};
		
		struct level_format {

			static const char *get_level(int value) {
				union int_to_level {
					log_level level;
					int value;
				};
				int_to_level conv;
				conv.value = value;
				return get_level(conv.level);
			}

			static const char *get_level(log_level level) {
				switch(level) {
				case log_level::OFF:     return _inst.off;
				case log_level::ERROR:   return _inst.erro;
				case log_level::AUDIT:   return _inst.audt;
				case log_level::ADMIN:   return _inst.admn;
				case log_level::WARNING: return _inst.warn;
				case log_level::INFO:    return _inst.info;
				case log_level::TRACE:   return _inst.trce;
				case log_level::DEBUG:   return _inst.debg;
				case log_level::ALL:     return _inst.all;
				}
			}

		private:

			static level_format _inst;

			const char *off  = "OFF ";
			const char *erro = "ERRO";
			const char *audt = "AUDT";
			const char *admn = "ADMN";
			const char *warn = "WARN";
			const char *info = "INFO";
			const char *trce = "TRCE";
			const char *debg = "DEBG";
			const char *all  = "ALL ";
			
		};

	}
}