//!=============================================================================
//! 
//! @file     InputMgr.h
//! @brief    User input manager
//! 
//! @details  
//! 
//! @note
//! 
//!=============================================================================
#pragma once

#include <SDL3/SDL.h>



namespace Raytracer {

	//! @class  InputMgr
	//! @brief  User input manager
	//! 
	class InputMgr {
	private:

	public:
		void HandleEvent(const SDL_Event& event);

	};

} // namespace Raytracer