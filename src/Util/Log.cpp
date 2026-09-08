//!=============================================================================
//! 
//! @file     Log.cpp
//! 
//!=============================================================================

#include "Util/Log.h"



namespace Raytracer::Util {

	//! @fn     Trace
	//! @brief  Logs the provided message with the [TRACE] tag
	//!
	void Log::Trace(std::string_view message) {
		std::cout << "[TRACE]" << message << '\n';
	}
	
	//! @fn     Debug
	//! @brief  Logs the provided message with the [DEBUG] tag
	//!
	void Log::Debug(std::string_view message) {
		std::cout << "[DEBUG]" << message << '\n';
	}
	
	//! @fn     Info
	//! @brief  Logs the provided message with the [INFO] tag
	//!
	void Log::Info(std::string_view message) {
		std::cout << "[INFO]" << message << '\n';
	}
	
	//! @fn     Warning
	//! @brief  Logs the provided message with the [WARN] tag
	//!
	void Log::Warning(std::string_view message) {
		std::cout << "[WARN]" << message << '\n';
	}
	
	//! @fn     Error
	//! @brief  Logs the provided message with the [ERROR] tag
	//!
	void Log::Error(std::string_view message) {
		std::cout << "[ERROR]" << message << '\n';
	}
	
	//! @fn     Fatal
	//! @brief  Logs the provided message with the [FATAL] tag
	//!
	void Log::Fatal(std::string_view message) {
		std::cout << "[FATAL]" << message << '\n';
	}

} // namespace Raytracer::Util