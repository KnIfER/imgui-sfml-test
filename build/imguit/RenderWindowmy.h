////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2017 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef SFML_RENDERWINDOWMY_HPP
#define SFML_RENDERWINDOWMY_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>
#include <iostream>

namespace sf
{
	class RenderWindowmy : public RenderWindow
	{
	public:
		RenderWindowmy():RenderWindow() {};
		RenderWindowmy(VideoMode mode, const String& title, Uint32 style = Style::Default, const ContextSettings& settings = ContextSettings()) : RenderWindow(mode, title, style, settings) {};

		int activateC = 0;
	protected:
		void onCreate() { std::cout << "onCreate" << std::endl; };
		///void onResize() { std::cout << "onResize" << std::endl; };
		//bool activate(bool active) {
		//	std::cout <<++activateC<< "active:" << active << std::endl;
		//	SetTimer(m_hWnd, 1, 1000, NULL);   //一个1秒触发一次的定时器 
		//	return active;
		//};
	};

} // namespace sf


#endif // SFML_RENDERWINDOWMY_HPP
