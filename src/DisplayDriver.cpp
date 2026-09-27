//!=============================================================================
//! 
//! @file     DisplayDriver.cpp
//! 
//!=============================================================================

#include "DisplayDriver.h"



namespace Raytracer {

	//! @fn     DisplayDriver
	//! @brief  Constructor
	//! 
	DisplayDriver::DisplayDriver(std::string windowTitle, int initialWindowWidth, int initialWindowHeight)
		: mWindowTitle(std::move(windowTitle))
		, mWindowWidth(initialWindowWidth)
		, mWindowHeight(initialWindowHeight)
		, mWindow(nullptr)
		, mRenderer(nullptr)
		, mTexture(nullptr)
		, mIsInitialized(false)
		, mInputMgr(std::make_unique<InputMgr>())
	{}

	//! @fn     ~DisplayDriver
	//! @brief  Destructor
	//! 
	DisplayDriver::~DisplayDriver() {
		if (mTexture) {
			SDL_DestroyTexture(mTexture);
		}

		if (mRenderer) {
			SDL_DestroyRenderer(mRenderer);
		}

		if (mWindow) {
			SDL_DestroyWindow(mWindow);
		}

		SDL_Quit();
	}

	//! @fn     Initialize
	//! @brief  Initialization function. Returns true on success
	//! 
	bool DisplayDriver::Initialize() {
		if (IsInitialized()) {
			Util::Log::Warning("DisplayDriver: Attempted to initialize when in an already initialized state");
			return false;
		}

		// Initialize SDL
		if (!SDL_Init(SDL_INIT_VIDEO)) {
			Util::Log::Error("DisplayDriver: Failed to initialize SDL");
			return false;
		}

		// Initialize window
		mWindow = SDL_CreateWindow(mWindowTitle.c_str(), mWindowWidth, mWindowHeight, NULL);
		if (mWindow == nullptr) {
			Util::Log::Error("DisplayDriver: Failed to create window");
			return false;
		}

		// Initialize renderer
		mRenderer = SDL_CreateRenderer(mWindow, nullptr);
		if (mRenderer == nullptr) {
			Util::Log::Error("DisplayDriver: Failed to create renderer");
			return false;
		}

		mTexture = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, mWindowWidth, mWindowHeight);
		if (mTexture == nullptr) {
			Util::Log::Error("DisplayDriver: Failed to create texture");
			return false;
		}

		// Additional settings
		SDL_CaptureMouse(true);
		SDL_SetWindowRelativeMouseMode(mWindow, true);

		Util::Log::Debug("DisplayDriver: Successfully initialized");
		mIsInitialized = true;
		return true;
	}

	//! @fn     PollEvents
	//! @brief  Processes all pending events
	//! 
	void DisplayDriver::PollEvents() {
		if (!IsInitialized()) {
			Util::Log::Warning("DisplayDriver: Attempted to poll events when uninitialized");
			return;
		}

		SDL_Event mEvent; //! Event polling result
		while (SDL_PollEvent(&mEvent)) {
			if (mEvent.type == SDL_EVENT_QUIT ||
				(mEvent.type == SDL_EVENT_KEY_DOWN && mEvent.key.key == SDLK_ESCAPE)) {
				mIsInitialized = false;
			}
			else {
				mInputMgr->HandleEvent(mEvent); //! Handle key events within InputManager
			}
		}
	}

	//! @fn     DisplayFrame
	//! @brief  Displays the provided frame in the window
	//! 
	void DisplayDriver::DisplayFrame(const Frame& frame) {
		if (!IsInitialized()) {
			Util::Log::Warning("DisplayDriver: Attempted to display frame when uninitialized");
			return;
		}

		// Set texture
		void* pixels;
		int pitch;	//! Number of bytes per row of the pixel buffer

		SDL_LockTexture(mTexture, NULL, &pixels, &pitch);
		memcpy(pixels, frame.GetBuffer(), static_cast<size_t>(pitch) * mWindowHeight);
		SDL_UnlockTexture(mTexture);

		// Render texture
		SDL_RenderClear(mRenderer);
		SDL_RenderTexture(mRenderer, mTexture, NULL, NULL);
		SDL_RenderPresent(mRenderer);
	}

	//! @fn     IsInitialized
	//! @brief  Returns whether the display is initialized.
	//!			This function returns false if the window has been closed
	//! 
	bool DisplayDriver::IsInitialized() const {
		return mIsInitialized;
	}

	//! @fn     GetWindowWidth
	//! @brief  Returns the current width of the window
	//! 
	int DisplayDriver::GetWindowWidth() const {
		return mWindowWidth;
	}

	//! @fn     GetWindowHeight
	//! @brief  Returns the current height of the window
	//! 
	int DisplayDriver::GetWindowHeight() const {
		return mWindowHeight;
	}

} // namespace Raytracer
