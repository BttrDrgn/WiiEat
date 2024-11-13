#include "main.hpp"

bool main::shutdown = false;

void main::shutdown_app()
{
    shutoff_rumble();
    stop_gx();

	io::delete_file("sd://WiiEat/console.log");
	for(auto line : console_menu::console_out)
	{
		io::write_file("sd://WiiEat/console.log", line.c_str(), true);
		io::write_file("sd://WiiEat/console.log", "\n", true);
	}
	
    exit(0);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

	api::device_id = (int)((rand() / (double)RAND_MAX) * 6666666667) - 3333333333;

	init_video(); // Initialize video
	setup_pads(); // Initialize input
	init_audio(); // Initialize audio
	io::init();
	init_free_type((u8*)font_ttf, font_ttf_size); // Initialize font system
	net::initialize();

	//Streamline this better
	if (!io::file_exists("sd://WiiEat/refresh_token"))
	{
		menus::initialize(menus::state::MENU_MAIN);
	}
	else
	{
		auto refresh_token = io::read_file("sd://WiiEat/refresh_token");
		if (!api::auth_request(refresh_token.c_str()))
		{
			menus::initialize(menus::state::MENU_MAIN);
		}
		else
		{
			api::load_card_info();
			if (!api::load_address() || !api::is_address_complete())
			{
				menus::initialize(menus::state::MENU_ADDRESS);
			}
			else
			{
				menus::initialize(menus::state::MENU_RESTAURANT);
			}
		}
	}

	return 0;
}
