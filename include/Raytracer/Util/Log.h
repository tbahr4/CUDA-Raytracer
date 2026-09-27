//!=============================================================================
//! 
//! @file     Log.h
//! @brief    Simple console logger
//! 
//! @details  
//! 
//! @note
//! 
//!=============================================================================
#pragma once

#include <iostream>
#include <string_view>



namespace Raytracer::Util {

	//! @class  Log
	//! @brief  Simple console logger
	//! 
	class Log {
	private:
		//! @class  LogLevel
		//! @brief  Log message severity levels
		//! 
		enum class LogLevel {
			TRACE = 0,		//! Extreme detail
			DEBUG = 1,		//! Developer output
			INFO = 2,		//! General information
			WARNING = 3,	//! Non-fatal warnings, operation can typically continue
			ERROR = 4,		//! Non-fatal error, operation may stop but program may continue
			FATAL = 5		//! Fatal error, program must exit immediately
		};

	public:
		Log() = delete;

		static void Trace(std::string_view message);
		static void Debug(std::string_view message);
		static void Info(std::string_view message);
		static void Warning(std::string_view message);
		static void Error(std::string_view message);
		static void Fatal(std::string_view message);
	};

} // namespace Raytracer::Util
