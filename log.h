/* 
 * File:   log.h
 * Author: Thiago Massari Guedes
 *
 * Created on 17 de Maio de 2015, 23:38
 */

#pragma once

#include <functional>
#include <sstream>
#include <tuple>

#include <map>
#include <string>

#define _NOEXCEPT noexcept
#define _TP std::make_tuple 

/************
* StreamPolicy
* a classe de StreamPolicy precisa de:
* operador StreamPolicy& operator << T
* std::string str() 
************/

namespace BTL {
	namespace Log {
		
template<typename T, typename Stream>
struct LogFormatter {
	LogFormatter(Stream &st, T &t) {
		st << t;
	}
};

template<typename Stream>
struct LogFormatter<int, Stream> {
	LogFormatter(Stream &st, int &t) {
		st << t << " ";
	}
};

template<typename T, typename U, typename Stream>
struct LogFormatter< std::tuple<T, U>, Stream> {
	LogFormatter(std::stringstream &ss, std::tuple<T, U> &t) {
		const auto &v = std::get<0>(t);
		auto f = std::get<1>(t);
		ss << f(v);
	}
};

enum class Level : int {
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

struct LogLevelFmt {

	static const char *getLevel(int value) {
		union IntToLevel {
			Level level;
			int value;
		};
		IntToLevel conv;
		conv.value = value;
		return getLevel(conv.level);
	}

	static const char *getLevel(Level level) {
		// _levelLabel.insert( item((int)Level::OFF,    "OFF ") );
		// _levelLabel.insert( item((int)Level::ERROR,  "ERRO") );
		// _levelLabel.insert( item((int)Level::AUDIT,  "AUDT") );
		// _levelLabel.insert( item((int)Level::ADMIN,  "ADMN") );
		// _levelLabel.insert( item((int)Level::WARNING,"WARN") );
		// _levelLabel.insert( item((int)Level::INFO,   "INFO") );
		// _levelLabel.insert( item((int)Level::TRACE,  "TRCE") );
		// _levelLabel.insert( item((int)Level::DEBUG,  "DEBG") );
		// _levelLabel.insert( item((int)Level::ALL,    "ALL ") );
		switch(level) {
		case Level::OFF:     return _inst.off;
		case Level::ERROR:   return _inst.erro;
		case Level::AUDIT:   return _inst.audt;
		case Level::ADMIN:   return _inst.admn;
		case Level::WARNING: return _inst.warn;
		case Level::INFO:    return _inst.info;
		case Level::TRACE:   return _inst.trce;
		case Level::DEBUG:   return _inst.debg;
		case Level::ALL:     return _inst.all;
		}
	}

private:

	static LogLevelFmt _inst;

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

template<typename StreamPolicy>
class LogCallback {
public:

	using cbFunction = std::function <void(int level, const std::string &text)>;

	LogCallback(Level lvl, cbFunction f) {
		_logCallback = f;
		_logLevel = lvl;
	}

	//Checks for log level -----------------------------
	bool isTraceEnabled() {
		return Level::TRACE > _logLevel;
	}
	bool isInfoEnabled() {
		return Level::INFO > _logLevel;
	}
	bool isErrorEnabled() {
		return Level::ERROR > _logLevel;
	}
	bool isAuditEnabled() {
		return Level::AUDIT > _logLevel;
	}
	bool isWarningEnabled() {
		return Level::WARNING > _logLevel;
	}
	bool isDebugEnabled() {
		return Level::DEBUG > _logLevel;
	}
	bool isVerboseEnabled() {
		return Level::VERBOSE > _logLevel;
	}
	bool isLevelEnabled(Level level) {
		return level >= _logLevel;
	}
	//==================================================
	
	template <typename T>
	void _log(StreamPolicy &st, T &t) {
		LogFormatter<T, StreamPolicy>(st, t);
	}

	template <typename T, typename ...U>
	void _log(StreamPolicy &ss, T &t, U&&... p) {
		LogFormatter<T, StreamPolicy>(ss, t);
		_log(ss, p...);
	}
	
	template <typename ...T>
	void log(Level level, T&&... p) {
		if( !isLevelEnabled(level) )
			return;
		StreamPolicy ss;
		_log(ss, p...);
		_logCallback((int)level, ss.str());
	} 

	template <typename ...T>
	static void trace(T&&... p) {
		if (get() != nullptr && get()->isTraceEnabled())
			get()->log(Level::TRACE, p...);
	}

	template <typename ...T>
	static void info(T&&... p) {
		if (get() != nullptr && get()->isInfoEnabled())
			get()->log(Level::INFO, p...);
	}

	template <typename ...T>
	static void warn(T&&... p) {
		if (get() != nullptr && get()->isWarningEnabled())
			get()->log(Level::WARNING, p...);
	}

	template <typename ...T>
	static void error(T&&... p) {
		if (get() != nullptr && get()->isErrorEnabled())
			get()->log(Level::ERROR, p...);
	}

	template <typename ...T>
	static void audit(T&&... p) {
		if (get() != nullptr && get()->isErrorEnabled())
			get()->log(Level::AUDIT, p...);
	}
	
	template <typename ...T>
	static void debug(T&&... p) {
		if (get() != nullptr && get()->isDebugEnabled())
			get()->log(Level::DEBUG, p...);
	}

	template <typename ...T>
	static void verbose(T&&... p) {
		if (get() != nullptr && get()->isVerboseEnabled())
			get()->log(Level::VERBOSE, p...);
	}

	static void init(Level logLevel, cbFunction f) {
		_inst = new LogCallback(logLevel,f);
	}

	static LogCallback *get() {
		if (_inst == nullptr) {

			struct LogException : public std::exception {
				std::string reason;
				virtual const char *what() const _NOEXCEPT {
					return reason.c_str();
				}
			};

			LogException e;
			e.reason = "Non initialized class. Log callback not defined";
			throw e;
		}
		else {
			return _inst;
		}
	}

private:
	cbFunction _logCallback;
	Level _logLevel;
	static LogCallback *_inst;
};

	}
}
