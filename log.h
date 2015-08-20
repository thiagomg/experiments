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

#include "log_formatter.h"
#include "level.h"

namespace BTL {
	namespace Log {
		
template<typename StreamPolicy>
class log_callback {
public:

	using cbFunction = std::function <void(int level, const std::string &text)>;

	log_callback(log_level lvl, cbFunction f) {
		_log_callback = f;
		_loglog_level = lvl;
	}
	
	//Internal log implementation ----------------------
	template <typename T>
	void _log(StreamPolicy &st, T &t) {
		log_formatter<T, StreamPolicy>(st, t);
	}

	template <typename T, typename ...U>
	void _log(StreamPolicy &ss, T &t, U&&... p) {
		log_formatter<T, StreamPolicy>(ss, t);
		_log(ss, p...);
	}
	
	template <typename ...T>
	void log(log_level level, T&&... p) {
		if( !is_level_enabled(level) )
			return;
		StreamPolicy ss;
		_log(ss, p...);
		_log_callback((int)level, ss.str());
	} 

	static log_callback *get() {
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
	//==================================================
	
	//Checks for log level -----------------------------
	bool is_trace_enabled() {
		return log_level::TRACE > _loglog_level;
	}
	bool is_info_enabled() {
		return log_level::INFO > _loglog_level;
	}
	bool is_error_enabled() {
		return log_level::ERROR > _loglog_level;
	}
	bool is_audit_enabled() {
		return log_level::AUDIT > _loglog_level;
	}
	bool is_warning_enabled() {
		return log_level::WARNING > _loglog_level;
	}
	bool is_debug_enabled() {
		return log_level::DEBUG > _loglog_level;
	}
	bool is_verbose_enabled() {
		return log_level::VERBOSE > _loglog_level;
	}
	bool is_level_enabled(log_level level) {
		return level >= _loglog_level;
	}
	//==================================================

	//Log external interface ---------------------------
	template <typename ...T>
	static void trace(T&&... p) {
		if (get() != nullptr && get()->is_trace_enabled())
			get()->log(log_level::TRACE, p...);
	}

	template <typename ...T>
	static void info(T&&... p) {
		if (get() != nullptr && get()->is_info_enabled())
			get()->log(log_level::INFO, p...);
	}

	template <typename ...T>
	static void warn(T&&... p) {
		if (get() != nullptr && get()->is_warning_enabled())
			get()->log(log_level::WARNING, p...);
	}

	template <typename ...T>
	static void error(T&&... p) {
		if (get() != nullptr && get()->is_error_enabled())
			get()->log(log_level::ERROR, p...);
	}

	template <typename ...T>
	static void audit(T&&... p) {
		if (get() != nullptr && get()->is_error_enabled())
			get()->log(log_level::AUDIT, p...);
	}
	
	template <typename ...T>
	static void debug(T&&... p) {
		if (get() != nullptr && get()->is_debug_enabled())
			get()->log(log_level::DEBUG, p...);
	}

	template <typename ...T>
	static void verbose(T&&... p) {
		if (get() != nullptr && get()->is_verbose_enabled())
			get()->log(log_level::VERBOSE, p...);
	}

	static void init(log_level loglog_level, cbFunction f) {
		_inst = new log_callback(loglog_level,f);
	}
	//==================================================

private:
	cbFunction _log_callback;
	log_level _loglog_level;
	static log_callback *_inst;
};

	}
}
