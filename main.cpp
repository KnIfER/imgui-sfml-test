#include <imgui-SFML.h>
#include <imgui.h>

#include <vlc/vlc.h>
#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Window/Event.hpp>

struct ctx {
  sf::Uint8 *surface{nullptr};
};

static void *lock(void *data, void **pixels) {
  struct ctx *ctx = (struct ctx *)data;
  *pixels = ctx;
  return nullptr;
}

static void unlock(void *data, void *id, void *const *pixels) {
  struct ctx *ctx = (struct ctx *)data;
}

static void display(void *data, void *id) { (void)data; }

void render() {
  ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiSetCond_FirstUseEver);
  ImGui::Begin("Window");
  ImGui::Text("Hello, world!\r\ndfgdfg\r\ndfgdg");
  ImGui::End();
}

int main() {
	std::cout <<"asd" << std::endl;
  //if (argc != 2) {
 //   std::cout << "Usage: ./imgui-vlc filename" << std::endl;
 //   return 0;
 // }
  const int width{640};
  const int height{360};

  sf::VideoMode vmode(width, height, 8);
  sf::RenderWindow window(vmode, "Imgui VLC", sf::Style::Close);

  window.setVerticalSyncEnabled(true);
  ImGui::SFML::Init(window);

  libvlc_instance_t *libvlc = nullptr;
  libvlc_media_t *media = nullptr;
  libvlc_media_player_t *media_player = nullptr;

  char const *vlc_arg[] = {"--no-video-title-show"};
  int count_arg = sizeof(vlc_arg) / sizeof(*vlc_arg);
  libvlc = libvlc_new(count_arg, vlc_arg);
  media = libvlc_media_new_path(libvlc, "C:\\b.mp4");//argv[1]
  media_player = libvlc_media_player_new_from_media(media);
  //libvlc_video_set_callbacks
  //libvlc_media_release(media);

  //sf::Uint8 frame[width * height * 4];
  sf::Uint8 *frame = new sf::Uint8[width * height * 4];

  libvlc_video_set_callbacks(media_player, lock, unlock, display, frame);
  libvlc_video_set_format(media_player, "RGBA", width, height, width * 4);
  std::cout << libvlc_media_player_play(media_player) << std::endl;
  


  sf::Texture texture;
  texture.create(width, height);
  sf::Sprite sprite;
  sprite.setTexture(texture);
  
  sf::Clock delta_clock; 
  //while (libvlc_media_player_get_state(media_player) != libvlc_state_t::libvlc_Playing) { std::cout << "libvlc_Playing_NOT" << std::endl; }
  while (window.isOpen()) {
	  std::cout << "true" << std::endl;
	  sf::Event event;
	  while (window.pollEvent(event)) {
	    ImGui::SFML::ProcessEvent(event);
  	  
	    switch (event.type) {
	      case sf::Event::Closed:
	        window.close();
	        break;
	      default:
	        break;
	    }
	  }
	  ImGui::SFML::Update(window, delta_clock.restart());
  	  
	  window.clear();
	  texture.update(frame);
	  window.draw(sprite);
  	  
	  render();
	  ImGui::SFML::Render(window);
  	  
	  window.display();
      sf::sleep(sf::milliseconds(10));
  }
  std::cout << "false" << std::endl;
  libvlc_media_player_stop(media_player);
  libvlc_media_player_release(media_player);
  libvlc_release(libvlc);
  
  //ImGui::SFML::Shutdown();

  return 0;
}
