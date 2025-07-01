# freetype2 cross compile
本文详细介绍了如何在Ubuntu 20.04 LTS环境下，使用arm-openwrt-linux交叉编译工具链，从源码编译Freetype2，并配置特定的编译选项，如禁用zlib、bzip2、png、harfbuzz等，最终生成动态和静态库。
## 下载地址
https://sourceforge.net/projects/freetype/files/freetype2/

以上链接下，有各个版本的源码下载。

## 编译环境
Ubuntu 20.04 LTS

## 编译配置
假设：

* 交叉编译工具：arm-openwrt-linux-

使用的交叉编译工具版本信息如下：
```
1. Target: arm-openwrt-linux-gnueabi
2. Configured with: --target=arm-openwrt-linux-gnueabi --enable-threads --disable-libmudflap --disable-libssp --disable-libstdcxx-pch --with-gnu-as --with-gnu-ld --enable-languages=c,c++ --enable-shared --enable-lto --enable-symvers=gnu --enable-__cxa_atexit --disable-nls --enable-clocale=gnu --enable-libgomp --disable-libitm --enable-poison-system-directories 
3. Thread model: posix
4. gcc version 6.4.1
```
* 交叉编译结果安装目录：${HOME}/lv_port_linux/lv_freetype
编译配置如下：
```
./configure --prefix=${HOME}/lv_port_linux/lv_freetype --host=arm-openwrt-linux CC=arm-openwrt-linux-gcc --enable-shared --enable-static --with-zlib=no --with-bzip2=no --with-png=no --with-harfbuzz=no
make -j$(nproc)
make install
```
* –prefix：指定安装目录
* –host：指定目标主机类型
* CC：指定交叉编译工具
* –enable-shared：编译生成.so动态库
* –enable-static：编译生成.a静态库
* 不使用zlib、bzip2、png、harfbuzz
# zlib cross compile
本文详细介绍在Ubunt 20.04 LTS环境下，使用arm-openwrt-linux交叉编译工具链，对zlib进行交叉编译的过程。包括下载zlib源码、配置交叉编译参数、修改Makefile以及执行make和make install命令的具体步骤。
## 下载地址
https://www.zlib.net/
## 编译环境
Ubuntu 20.04 LTS
## 编译配置
假设：
* 交叉编译工具：arm-openwrt-linux-

使用的交叉编译工具版本信息如下：
```
1. Target: arm-openwrt-linux
2. Configured with: --target=arm-openwrt-linux --enable-threads --disable-libmudflap --disable-libssp --disable-libstdcxx-pch --with-gnu-as --with-gnu-ld --enable-languages=c,c++ --enable-shared --enable-lto --enable-symvers=gnu --enable-__cxa_atexit --disable-nls --enable-clocale=gnu --enable-libgomp --disable-libitm --enable-poison-system-directories 
3. Thread model: posix
4. gcc version 6.4.1
```
* 交叉编译结果安装目录：${HOME}/lv_port_linux/lv_freetype

配置及编译：

1. 配置编译参数：
```
./configure --prefix=${HOME}/lv_port_linux/lv_freetype --shared --static
```
* –prefix：指定安装目录
* –shared：编译生成.so动态库
* –static：编译生成.a静态库
2. 修改Makefile，如下：
```
CC=arm-openwrt-linux-gcc

CFLAGS=-O3 -D_LARGEFILE64_SOURCE=1 -DHAVE_HIDDEN
#CFLAGS=-O -DMAX_WBITS=14 -DMAX_MEM_LEVEL=7
#CFLAGS=-g -DZLIB_DEBUG
#CFLAGS=-O3 -Wall -Wwrite-strings -Wpointer-arith -Wconversion \
#           -Wstrict-prototypes -Wmissing-prototypes

SFLAGS=-O3 -fPIC -D_LARGEFILE64_SOURCE=1 -DHAVE_HIDDEN
LDFLAGS=
TEST_LDFLAGS=-L. libz.a
LDSHARED=arm-openwrt-linux-gcc -shared -Wl,-soname,libz.so.1,--version-script,zlib.map
CPP=arm-openwrt-linux-gcc -E

STATICLIB=libz.a
SHAREDLIB=libz.so
SHAREDLIBV=libz.so.1.2.11
SHAREDLIBM=libz.so.1
LIBS=$(STATICLIB) $(SHAREDLIBV)

AR=arm-openwrt-linux-ar
ARFLAGS=rc
RANLIB=arm-openwrt-linux-ranlib
LDCONFIG=ldconfig
LDSHAREDLIBC=-lc
TAR=tar
SHELL=/bin/sh
EXE=

prefix =${HOME}/lv_port_linux/lv_freetype
exec_prefix =${prefix}
libdir =${exec_prefix}/lib
sharedlibdir =${libdir}
includedir =${prefix}/include
mandir =${prefix}/share/man
man3dir = ${mandir}/man3
pkgconfigdir = ${libdir}/pkgconfig
SRCDIR=
ZINC=
ZINCOUT=-I.
```
3. 执行make，以及make install
# libpng Cross compile
本文詳細介紹libpng函式庫的交叉編譯過程，包括從sourceforge.net下載libpng原始碼，設定Ubuntu20.04 LTS環境，使用arm-openwrt-linux-gcc交叉編譯工具，以及具體的編譯和安裝步驟。

## 下載地址
https://sourceforge.net/projects/libpng/files/
以上連結下，有各個版本的原始碼下載。

## 編譯環境
Ubuntu 20.04 LTS

## 編譯配置
假設：

* 交叉編譯工具：arm-openwrt-linux-
使用的交叉編譯工具版本資訊如下：
```
1. Target: arm-openwrt-linux
2. Configured with: --target=arm-openwrt-linux --enable-threads --disable-libmudflap --disable-libssp --disable-libstdcxx-pch --with-gnu-as --with-gnu-ld --enable-languages=c,c++ --enable-shared --enable-lto --enable-symvers=gnu --enable-__cxa_atexit --disable-nls --enable-clocale=gnu --enable-libgomp --disable-libitm --enable-poison-system-directories 
3. Thread model: posix
4. gcc version 6.4.1
```
* 交叉編譯結果安裝目錄：${HOME}/lv_port_linux/lv_freetype

編譯配置如下：
```
./configure --prefix=${HOME}/lv_port_linux/lv_freetype --host=arm-openwrt-linux CC=arm-openwrt-linux-gcc --enable-shared --enable-static LDFLAGS=-L${HOME}/lv_port_linux/lv_freetype/lib CPPFLAGS=-I${HOME}/lv_port_linux/lv_freetype/include
make -j$(nproc)
make install
```
* –prefix：指定安裝目錄
* –host：指定目標主機類型
* CC：指定交叉編譯工具
* –enable-shared：編譯產生.so動態函式庫
* –enable-static：編譯產生.a靜態函式庫
* LDFLAGS：指定連結庫目錄。不一定需要，看編譯具體狀況
* CPPFLAGS：指定頭檔目錄。不一定需要，看編譯具體狀況

