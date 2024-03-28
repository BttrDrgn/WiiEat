#include "main.hpp"

bool main::shutdown = false;

void main::shutdown_app()
{
    shutoff_rumble();
    stop_gx();

	io::delete_file("sd://WiiEat/console.log");
	for(auto line : console_menu::console_out)
	{
		io::write_file("sd://WiiEat/console.log", format::va("%s\n", line.c_str()), true);
	}
	
    exit(0);
}

int main(int argc, char *argv[])
{
	init_video(); // Initialize video
	setup_pads(); // Initialize input
	init_audio(); // Initialize audio
	io::init();
	init_free_type((u8*)font_ttf, font_ttf_size); // Initialize font system
	net::initialize();

	//Streamline this better
	if(io::file_exists("sd://WiiEat/refresh_token"))
	{
		auto refresh_token = io::read_file("sd://WiiEat/refresh_token");
		if(api::auth_request(refresh_token.c_str()))
		{
			if(api::load_address())
			{
				if(api::is_address_complete())
				{
					menus::initialize(menus::state::MENU_RESTAURANT);
				}
				else
				{
					menus::initialize(menus::state::MENU_ADDRESS);
				}
			}
			else
			{
				menus::initialize(menus::state::MENU_ADDRESS);
			}
		}
		else
		{
			menus::initialize(menus::state::MENU_MAIN);
		}
	}
	else
	{
		menus::initialize(menus::state::MENU_MAIN);
	}

	return 0;
}
