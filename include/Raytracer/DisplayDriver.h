//!=============================================================================
//! 
//! @file     DisplayDriver.h
//! @brief    Establishes an abstracted interface to the SDL3 display
//! 
//! @details  
//! 
//! @note
//! 
//!=============================================================================
#pragma once

#include "Frame.h"
#include "Util/Log.h"
#include "InputMgr.h"
#include <SDL3/SDL.h>
#include <string>
#include <memory>



namespace Raytracer {

	//! @class  DisplayDriver
	//! @brief  Establishes an abstracted interface to the SDL3 display
	//! 
	class DisplayDriver {
	private:
		// Properties
		const std::string mWindowTitle;	//! Title displayed on the window
		int mWindowWidth;				//! Width of the window, pixels
		int mWindowHeight;				//! Height of the window, pixels

		// SDL objects
		SDL_Window* mWindow;
		SDL_Renderer* mRenderer;
		SDL_Texture* mTexture;

		// General
		bool mIsInitialized;					//! Initialization status
		std::unique_ptr<InputMgr> mInputMgr;	//! User input manager

	public:
		DisplayDriver(std::string windowTitle, int windowWidth, int windowHeight);
		~DisplayDriver();

		bool Initialize();

		void PollEvents();
		void DisplayFrame(const Frame& frame);

		bool IsInitialized() const;
		int GetWindowWidth() const;
		int GetWindowHeight() const;
	};

} // namespace Raytracer
