#include <imgui-SFML.h>
#include <imgui.h>

#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <RenderWindowmy.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Cursor.hpp>


//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
bool isMouseDragging = false;
bool isScalingLeft = false;
bool isScalingRight = false;
bool isScalingTop = false;
bool isScalingBottom = false;
bool isScaling = false;
bool isResized = false;
int lastDownX, lastDownY;
int cursorIndexerY, cursorIndexerX;
//sf::Vector2<int> windowOldPos;
//sf::Vector2u windowOldSize;
//bool isIMGuiIntercepting = false;

int main() {
	std::cout << "asd" << std::endl;

	const int width{ 800 };
	const int height{ 600 };
	const int borderWidth{ 10 };

	ImGui::CreateContext();

	sf::VideoMode vmode(width, height, 8);
	sf::RenderWindow window(vmode, "Imgui-sfml widgets test", 0);
	window.setVerticalSyncEnabled(true);

	ImGui::SFML::Init(window);

	sf::Uint8 *frame = new sf::Uint8[width * height * 4];
	sf::Texture texture;
	texture.create(width, height);
	sf::Sprite sprite;
	sprite.setTexture(texture);

	sf::Cursor cc;

	
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	sf::Clock delta_clock;
	while (window.isOpen()) {
		sf::Event event;
		ImGui::SFML::Update(window, delta_clock.restart());//where to put this code?
		
		while (window.pollEvent(event)) {
			//std::cout << "pollEventwhile" << std::endl;
			//TODO: scale on border ignoring imgui widgets
			if(!isScaling)
				ImGui::SFML::ProcessEvent(event);
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseMoved:
				std::cout << "MouseMoved" << std::endl;
				//缩放
				if (isScalingLeft) {
					window.setPosition(window.getPosition() + sf::Vector2<int>(event.mouseMove.x - lastDownX, 0));
					window.setSize(window.getSize() - sf::Vector2u{ (unsigned int)(event.mouseMove.x - lastDownX ), 0 });
				}else if (isScalingRight) {
					//window.setSize(windowOldSize + sf::Vector2u{ (unsigned int)(event.mouseMove.x - lastDownX ), 0 });
					window.setSize(window.getSize() + sf::Vector2u{ (unsigned int)(event.mouseMove.x - lastDownX), 0 });
					lastDownX = event.mouseMove.x;
				}
				if (isScalingTop) {
					window.setPosition(window.getPosition() + sf::Vector2<int>(0,event.mouseMove.y - lastDownY));
					window.setSize(window.getSize() - sf::Vector2u{0, (unsigned int)(event.mouseMove.y - lastDownY)});
				}else if (isScalingBottom) {
					//window.setSize(windowOldSize + sf::Vector2u{ 0,(unsigned int)(event.mouseMove.y - lastDownY)});
					window.setSize(window.getSize() + sf::Vector2u{ 0, (unsigned int)(event.mouseMove.y - lastDownY) });
					lastDownY = event.mouseMove.y;
				}
				//
				//处理hover
				//border vals:(base 4)
				//    3-----2----4
				//	  |			 |
				//    1          1
				//	  |			 |
				//    4-----2----3
				cursorIndexerX = cursorIndexerY = 0;
				if (event.mouseMove.x <= borderWidth) {
					cursorIndexerX = -1;
				}else if (event.mouseMove.x >= window.getSize().x - borderWidth) {
					cursorIndexerX = 1;
				}
				if (event.mouseMove.y <= borderWidth) {
					cursorIndexerY = 1;
				}else if (event.mouseMove.y >= window.getSize().y - borderWidth) {
					cursorIndexerY = -1;
				}
				//判断hover
				if (cursorIndexerX == 0 && cursorIndexerY!=0) {
					cc.loadFromSystem((sf::Cursor::SizeVertical));
					window.setMouseCursor(cc);
				}else if (cursorIndexerY == 0 && cursorIndexerX!=0) {
					cc.loadFromSystem((sf::Cursor::SizeHorizontal));
					window.setMouseCursor(cc);
				}
				else if (cursorIndexerX*cursorIndexerY==-1) {
					cc.loadFromSystem((sf::Cursor::SizeTopLeftBottomRight));
					window.setMouseCursor(cc);
				}else if (cursorIndexerX*cursorIndexerY==1) {
					cc.loadFromSystem((sf::Cursor::SizeBottomLeftTopRight));
					window.setMouseCursor(cc);
				}else {
					cc.loadFromSystem((sf::Cursor::Type)(0));
					window.setMouseCursor(cc);
				}
				//imgui控件consume event
				if (ImGui::IsAnyItemActive() || ImGui::IsAnyWindowFocused())
					break;
				//拖动窗口
				if (isMouseDragging) {
					window.setPosition(window.getPosition() + sf::Vector2<int>(event.mouseMove.x - lastDownX, event.mouseMove.y - lastDownY));
				}
				break;
			case sf::Event::MouseButtonPressed:
				std::cout << "MouseButtonPressed 1" << std::endl;
				lastDownX = event.mouseButton.x;
				lastDownY = event.mouseButton.y;
				//检测边界缩放 detect scaling by borders
				if (lastDownX <= borderWidth) {
					isScalingLeft = true;
				}else if (lastDownX >= window.getSize().x - borderWidth) {
					isScalingRight = true;
				}
				if (lastDownY <= borderWidth) {
					isScalingTop = true;
				}else if (lastDownY >= window.getSize().y - borderWidth) {
					isScalingBottom = true;
				}
				std::cout << "MouseButtonPressed 2" << std::endl;
				if (isScalingLeft || isScalingRight || isScalingTop || isScalingBottom) {
					isScaling = true;
					break;
				}
				//未检测到，则进入移动模式 entering move mode
				isMouseDragging = true;
				break;
			case sf::Event::MouseButtonReleased:
				isMouseDragging =
				isScaling=
				isScalingBottom=
				isScalingTop=
				isScalingRight=
				isScalingLeft = false;
				break;
			case sf::Event::KeyPressed:
				//std::cout << event.key.code << std::endl;
				break;
			default:
				break;
			}
		}
		
		
		window.clear(clear_color);
		texture.update(frame);
		window.draw(sprite);

		// 1. Show a simple window.
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
		{
			static float f = 0.0f;
			static int counter = 0;
			ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
			{
				counter++;
			}
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
		if (show_demo_window)
		{
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
			ImGui::ShowDemoWindow(&show_demo_window);
		}

		//自定义控件——SeekBar
		ImGui::SetNextWindowSize(ImVec2{ (float)window.getSize().x ,0 });
		ImGui::SetNextWindowPos(ImVec2{ 0 ,(float)window.getSize().y - 100 });
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::Begin("Another Window my", &show_another_window, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

		static float progress = 0.0f, progress_dir = 1.0f;
		if (false)
		{
			progress += progress_dir * 0.4f * ImGui::GetIO().DeltaTime;
			if (progress >= +1.1f) { progress = +1.1f; progress_dir *= -1.0f; }
			if (progress <= -0.1f) { progress = -0.1f; progress_dir *= -1.0f; }
		}
		ImGui::SliderFloat("", &progress, 0.f, 1.0f);
		ImGui::SeekBar(&progress, 0.f, 100.0f);
		ImGui::End();
		ImGui::PopStyleVar();




		ImGui::SFML::Render(window);
		window.display();
		//sf::sleep(sf::milliseconds(10));
	}

	ImGui::DestroyContext();

	return 0;
}
