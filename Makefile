#
# Makefile
#
CROSS_COMPILE   = $(HOME)/Tina-Linux/prebuilt/gcc/linux-x86/arm/toolchain-sunxi-musl/toolchain/bin/arm-openwrt-linux-
CC              = $(CROSS_COMPILE)gcc
CXX             = $(CROSS_COMPILE)g++
# TARGET_USR_DIR  = $(HOME)/Tina-Linux/out/t113-bingpi_m2/staging_dir/target/usr
# TARGET_LIB_DIR  = $(TARGET_USR_DIR)/lib
# TARGET_INC_DIR  = $(TARGET_USR_DIR)/include/freetype2

LVGL_DIR_NAME   ?= lvgl
LVGL_DIR        ?= .
STAGING_DIR     = ${HOME}/Tina-Linux/out/t113-bingpi_m2/staging_dir/target
export STAGING_DIR

WARNINGS        := -Wall -Wshadow -Wundef -Wmissing-prototypes -Wno-discarded-qualifiers -Wextra -Wno-unused-function -Wno-error=strict-prototypes -Wpointer-arith \
                   -fno-strict-aliasing -Wno-error=cpp -Wuninitialized -Wmaybe-uninitialized -Wno-unused-parameter -Wno-missing-field-initializers -Wtype-limits \
                   -Wsizeof-pointer-memaccess -Wno-format-nonliteral -Wno-cast-qual -Wunreachable-code -Wno-switch-default -Wreturn-type -Wmultichar -Wformat-security \
                   -Wno-ignored-qualifiers -Wno-error=pedantic -Wno-sign-compare -Wno-error=missing-prototypes -Wdouble-promotion -Wclobbered -Wdeprecated -Wempty-body \
                   -Wshift-negative-value -Wstack-usage=2048 -Wno-unused-value -std=gnu99
CFLAGS          ?= -O3 -g0 -I$(LVGL_DIR)/ -I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/libs $(WARNINGS) \
				   -I$(LVGL_DIR)/src/middle_ware \
				   -I${STAGING_DIR}/usr/include \
				   -I${STAGING_DIR}/usr/include/allwinner \
				   -I${STAGING_DIR}/usr/include/allwinner/include \
				   -DCEDARC_DEBUG=0
LDFLAGS         ?= -lm -L${STAGING_DIR}/usr/lib \
				   -ltrecorder -ltplayer -lxplayer -lvdecoder -ladecoder -lsubdecoder \
				   -lcdc_base -lVE -lMemAdapter -lcdx_parser  -lcdx_playback -lcdx_stream \
				   -lcdx_base -lawrecorder -lvencoder -laencoder -lcdx_muxer -ljpegdecode \
				   -ltmetadataretriever -lcdx_common -luapi -lz -lssl -lasound -lsbm \
				   -laftertreatment -lscaledown -lfbm -lvideoengine -lcrypto -lawrecorder \
				   -lvenc_h264 -lvenc_h265 -lvenc_common -lvenc_jpeg -lvenc_base #-lfreetype \
				   #-lbz2

BIN             = ezkit
BUILD_DIR       = ./build
BUILD_OBJ_DIR   = $(BUILD_DIR)/obj
BUILD_BIN_DIR   = $(BUILD_DIR)/bin

prefix          ?= /usr
bindir          ?= $(prefix)/bin

# Collect source files recursively
CSRCS           := $(shell find src -type f -name '*.c')
CXXSRCS         := $(shell find src -type f -name '*.cpp')

# Include LVGL sources
include $(LVGL_DIR)/lvgl/lvgl.mk
# include $(LVGL_DIR)/lv_freetype/lv_freetype.mk
include $(LVGL_DIR)/page/page.mk
include $(LVGL_DIR)/picture/picture.mk
include $(LVGL_DIR)/font/font.mk

#Do not compile the example
EXCSRCS			+= $(shell find -L $(LVGL_DIR)/$(LVGL_DIR_NAME)/examples -name \*.c)
CSRCS			:= $(filter-out $(EXCSRCS),$(CSRCS))

OBJEXT          ?= .o

COBJS           = $(CSRCS:.c=$(OBJEXT))
CXXOBJS         = $(CXXSRCS:.cpp=$(OBJEXT))
AOBJS           = $(ASRCS:.S=$(OBJEXT))

SRCS            = $(ASRCS) $(CSRCS) $(CXXSRCS)
OBJS            = $(AOBJS) $(COBJS) $(CXXOBJS)
TARGET          = $(addprefix $(BUILD_OBJ_DIR)/, $(patsubst ./%, %, $(OBJS)))

all: default

$(BUILD_OBJ_DIR)/%.o: %.c lv_conf.h
	@mkdir -p $(dir $@)
	@$(CC)  $(CFLAGS) -c $< -o $@
	@echo "CC  $<"

$(BUILD_OBJ_DIR)/%.o: %.cpp lv_conf.h
	@mkdir -p $(dir $@)
	@$(CXX)  $(CFLAGS) -c $< -o $@
	@echo "CXX $<"

$(BUILD_OBJ_DIR)/%.o: %.S lv_conf.h
	@mkdir -p $(dir $@)
	@$(CC)  $(CFLAGS) -c $< -o $@
	@echo "AS  $<"

default: $(TARGET)
	@mkdir -p $(dir $(BUILD_BIN_DIR)/)
	$(CXX) -o $(BUILD_BIN_DIR)/$(BIN) $(TARGET) $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)

install:
	install -d $(DESTDIR)$(bindir)
	install $(BUILD_BIN_DIR)/$(BIN) $(DESTDIR)$(bindir)

uninstall:
	$(RM) -r $(addprefix $(DESTDIR)$(bindir)/,$(BIN))

