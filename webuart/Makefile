#
# This software is licensed under the Public Domain.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=webuart
PKG_VERSION:=0.0.1
PKG_RELEASE:=$(AUTORELEASE)

PKG_MAINTAINER:=AVP <anstep78@gmail.com>
PKG_LICENSE:=CC0-1.0
MODE=RELEASE
include $(INCLUDE_DIR)/package.mk

define Package/webuart
	SECTION:=utils
	DEPENDS:=+libsqlite3
	CATEGORY:=Utilities
	TITLE:=WebUart
	URL:=https://smart.internet-gorodok.ru
endef

define Package/readavr/description
	Websocket to uart
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -c -o $(PKG_BUILD_DIR)/main.o  $(PKG_BUILD_DIR)/main.c $(TARGET_LDFLAGS)
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/webuart $(PKG_BUILD_DIR)/main.o -lsqlite3
endef

define Package/webuart/install
	mkdir -p $(1)/www/web/
	mkdir -p $(1)/mnt/usb/.data
	$(CP) ./files/www/* $(1)/www
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/etc/init.d/webuart $(1)/etc/init.d/webuart
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) ./files/etc/config/webuart $(1)/etc/config/webuart
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/webuart $(1)/usr/bin/
endef

$(eval $(call BuildPackage,webuart))
