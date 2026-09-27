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
#include <unordered_map>



namespace Raytracer {

	//! @class  InputMgr
	//! @brief  User input manager
	//! 
	class InputMgr {
	private:
		//! @class  InputAction
		//! @brief  Types of available input actions
		//! 
		enum class InputAction {
			NONE,	// Default action
			MOVE_FORWARD,
			MOVE_BACKWARD,
			MOVE_RIGHT,
			MOVE_LEFT,
			MOVE_UP,
			MOVE_DOWN,
			ROLL_RIGHT,
			ROLL_LEFT
		};

		//! @name   Keybind list
		//! @brief  Defines mappings from keyboard inputs to an action.
		//!         Overridable via configuration (TODO)
		//! 
		std::unordered_map<SDL_Keycode, InputAction> mActionMappings {
			{SDLK_W, InputAction::MOVE_FORWARD},
			{SDLK_S, InputAction::MOVE_BACKWARD},
			{SDLK_D, InputAction::MOVE_RIGHT},
			{SDLK_A, InputAction::MOVE_LEFT},
			{SDLK_SPACE, InputAction::MOVE_UP},
			{SDLK_LSHIFT, InputAction::MOVE_DOWN},
			{SDLK_E, InputAction::ROLL_RIGHT},
			{SDLK_Q, InputAction::ROLL_LEFT}
		};

		//Player* mPlayer | TODO

	public:
		void HandleEvent(const SDL_Event& event);

	};

} // namespace Raytracer
