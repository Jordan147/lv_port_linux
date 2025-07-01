# Freetype compilation
It is recommand to download latest freetype release and build seperately.
Refer to freetype/docs/INSTALL.CROSS in detail setup/build steps.
# For UNIX
For UNIX systems, it is recommended to use the way of compiling and installing libraries.
* Enter the FreeType source code directory
* make
* sudo make install
* Add include path: /usr/include/freetype2 (for GCC: -I/usr/include/freetype2 -L/usr/local/lib)
* Link library: freetype (for GCC: -L/usr/local/lib -lfreetype)
# For embedded devices
For embedded devices, it is more recommended to use the FreeType configuration file provided by LVGL, which only includes the most commonly used functions, which is very meaningful for saving limited FLASH space.

* Copy the FreeType source code to your project directory.
* Refer to the following Makefile for configuration:
```
# FreeType custom configuration header file
CFLAGS += -DFT2_BUILD_LIBRARY
CFLAGS += -DFT_CONFIG_MODULES_H=<lvgl/src/libs/freetype/ftmodule.h>
CFLAGS += -DFT_CONFIG_OPTIONS_H=<lvgl/src/libs/freetype/ftoption.h>

# FreeType include path
CFLAGS += -Ifreetype/include

# FreeType C source file
FT_CSRCS += freetype/src/base/ftbase.c
FT_CSRCS += freetype/src/base/ftbitmap.c
FT_CSRCS += freetype/src/base/ftdebug.c
FT_CSRCS += freetype/src/base/ftglyph.c
FT_CSRCS += freetype/src/base/ftinit.c
FT_CSRCS += freetype/src/cache/ftcache.c
FT_CSRCS += freetype/src/gzip/ftgzip.c
FT_CSRCS += freetype/src/sfnt/sfnt.c
FT_CSRCS += freetype/src/smooth/smooth.c
FT_CSRCS += freetype/src/truetype/truetype.c
CSRCS += $(FT_CSRCS)
```
## Configure
```
cd freetype
 ./configure --host=arm-openwrt-linux --prefix=/usr/local/arm-openwrt-linux
 make -j
 sudo make install
 cp -r /usr/local/arm-openwrt-linux $(YOUR_PRJ)/freetype
```