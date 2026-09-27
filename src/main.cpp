//!=============================================================================
//! 
//! @file     main.cpp
//! @brief    Main entry point
//! 
//! @details  
//! 
//! @note
//! 
//!=============================================================================
#pragma once

#include "DisplayDriver.h"



int main() {
	Raytracer::DisplayDriver display("Raytracer", 600, 400);

	if (!display.Initialize()) {
		return false;
	}

	

	return 0;
}
