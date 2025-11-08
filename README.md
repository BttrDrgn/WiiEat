# WiiEat
A hackly thrown together mess of a homebrew with a very slightly thought out api design

Made for educational purposes. Used to fill my stomach.

# Compiling
- devKitPro with Wii development libraries
- [wii-curl](https://github.com/AndrewPiroli/wii-curl)
## Windows
- [devKitPro installer](https://github.com/devkitPro/installer/releases/latest)
- Run `pacman -Syu` in devKitPro's Msys2 (dkp)
- Install the wii-curl libraries one at a time starting from `wii-mbedtls` to `wii-curl` then `libwiisocket` by running `pacman -U *.tar.gz` in dkp
- Install the dkp portlibs `pacman -S ppc-libogg ppc-freetype ppc-libvorbisidec`
- Run `./make.sh` in the WiiEat project folder