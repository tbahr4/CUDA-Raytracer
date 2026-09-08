//!=============================================================================
//! 
//! @file     Frame.h
//! @brief    Modular frame buffer for pixel rendering
//! 
//! @details  
//! 
//! @note
//! 
//!=============================================================================
#pragma once

#include <cstdint>



namespace Raytracer {

	//! @class  Frame
	//! @brief  Modular frame buffer for pixel rendering
	//! 
	class Frame {
	private:

	public:
		const uint32_t* GetBuffer() const;

	};

} // namespace Raytracer