#include "main.hpp"

bool main::shutdown = false;

void main::shutdown_app()
{
    shutoff_rumble();
    stop_gx();
    exit(0);
}

int main(int argc, char *argv[])
{
	init_video(); // Initialize video
	setup_pads(); // Initialize input
	init_audio(); // Initialize audio
	fs::init();
	init_free_type((u8*)font_ttf, font_ttf_size); // Initialize font system
	net::initialize();
	
	menus::initialize(menus::state::MENU_MAIN);
}
