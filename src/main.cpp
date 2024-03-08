#include "main.hpp"

bool main::shutdown = false;

void main::shutdown_app()
{
    shutoff_rumble();
    stop_gx();

	std::string console_log = "";
	for(auto line : console_menu::console_out)
	{
		console_log += format::va("%s\n", line.c_str());
	}
	fs::write_file("sd://WiiEat/console.log", console_log);

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

	//Streamline this better
	if(fs::file_exists("sd://WiiEat/refresh_token"))
	{
		auto refresh_token = fs::read_file("sd://WiiEat/refresh_token");
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
