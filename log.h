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

#define _NOEXCEPT noexcept
#define _TP std::make_tuple 

template<typename T>
struct LogFormatter {
	LogFormatter(std::stringstream &ss, T &t) {
		ss << t;
	}
};

template<>
struct LogFormatter<int> {
	LogFormatter(std::stringstream &ss, int &t) {
		ss << t << " ";
	}
};

template<typename T, typename U>
struct LogFormatter< std::tuple<T, U> > {
	LogFormatter(std::stringstream &ss, std::tuple<T, U> &t) {
		const auto &v = std::get<0>(t);
		auto f = std::get<1>(t);
		ss << f(v);
	}
};

class LogCallback {
public:

	enum class Level : int {
		OFF_LEVEL = 9,
		ERROR_LEVEL = 8,
		AUDIT_LEVEL = 7,
		ADMIN_LEVEL = 6,
		WARNING_LEVEL = 5,
		INFO_LEVEL = 4,
		TRACE_LEVEL = 3,
		DEBUG_LEVEL = 2,
		VERBOSE_LEVEL = 1,
		ALL_LEVEL = 0
	};

	using cbFunction = std::function <void(int level, const std::string &text)>;

	LogCallback(Level lvl, cbFunction f) {
		_logCallback = f;
		_logLevel = lvl;
	}

	//Checks for log level -----------------------------
	bool isTraceEnabled() {
		return Level::TRACE_LEVEL > _logLevel;
	}
	bool isInfoEnabled() {
		return Level::INFO_LEVEL > _logLevel;
	}
	bool isErrorEnabled() {
		return Level::ERROR_LEVEL > _logLevel;
	}
	bool isWarningEnabled() {
		return Level::WARNING_LEVEL > _logLevel;
	}
	bool isDebugEnabled() {
		return Level::DEBUG_LEVEL > _logLevel;
	}
	bool isVerboseEnabled() {
		return Level::VERBOSE_LEVEL > _logLevel;
	}
	bool isLevelEnabled(Level level) {
		return level >= _logLevel;
	}
	//==================================================
	
	template <typename T>
	void _log(std::stringstream &ss, T &t) {
		LogFormatter<T>(ss, t);
	}

	template <typename T, typename ...U>
	void _log(std::stringstream &ss, T &t, U&&... p) {
		LogFormatter<T>(ss, t);
		_log(ss, p...);
	}
	
	template <typename ...T>
	void log(Level level, T&&... p) {
		if( !isLevelEnabled(level) )
			return;
		std::stringstream ss;
		_log(ss, p...);
		_logCallback((int)level, ss.str());
	} 

	template <typename ...T>
	static void trace(T&&... p) {
		if (get() != nullptr && get()->isTraceEnabled())
			get()->log(Level::TRACE_LEVEL, p...);
	}

	template <typename ...T>
	static void info(T&&... p) {
		if (get() != nullptr && get()->isInfoEnabled())
			get()->log(Level::INFO_LEVEL, p...);
	}

	template <typename ...T>
	static void warn(T&&... p) {
		if (get() != nullptr && get()->isWarningEnabled())
			get()->log(Level::WARNING_LEVEL, p...);
	}

	template <typename ...T>
	static void error(T&&... p) {
		if (get() != nullptr && get()->isErrorEnabled())
			get()->log(Level::ERROR_LEVEL, p...);
	}

	template <typename ...T>
	static void debug(T&&... p) {
		if (get() != nullptr && get()->isDebugEnabled())
			get()->log(Level::DEBUG_LEVEL, p...);
	}

	template <typename ...T>
	static void verbose(T&&... p) {
		if (get() != nullptr && get()->isVerboseEnabled())
			get()->log(Level::VERBOSE_LEVEL, p...);
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
