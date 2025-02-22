SUMMARY = "Cog is a small launcher designed for the WebKit WPE port. \
           But it also allows to use the WebKit GTK port instead. \
           Cog is small: provides no user interface, and is suitable \
           to be used as a Web application container. Cog may be \
           presented fullscreen depending on the WPE backend being used. \
           "
HOMEPAGE = "https://github.com/Igalia/cog"
BUGTRACKER = "https://github.com/Igalia/cog/issues"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYING;md5=bf1229cd7425b302d60cdb641b0ce5fb"

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI = "https://wpewebkit.org/releases/${P}.tar.xz"

# Depend on wpewebkit unless the webkitgtk packageconfig option is selected.
DEPENDS += "\
            ${@bb.utils.contains('PACKAGECONFIG', 'webkitgtk', 'webkitgtk', 'wpewebkit', d)} \
            ${@bb.utils.contains('PACKAGECONFIG', 'fdo', 'wayland', '', d)} \
            ${@bb.utils.contains_any('LAYERSERIES_CORENAMES', 'dunfell gatesgarth hardknott honister', 'libsoup-2.4', 'libsoup', d)} \
            glib-2.0 wayland-native wayland-protocols \
            "

DEPENDS:append:class-target = " \
            ${@bb.utils.contains('PACKAGECONFIG', 'fdo', 'wayland-native', '', d)} \
            "

# At run-time cog package should depend on virtual/wpebackend unless webkitgtk+ is enabled.
RDEPENDS:${PN} += "\
    ${@bb.utils.contains('PACKAGECONFIG', 'webkitgtk', '', 'virtual/wpebackend', d)} \
    xkeyboard-config \
    "

inherit pkgconfig

# Cog can work with any wpebackend.
# But for using wpebackend-fdo this has to be enabled at build time.
PREFERRED_PROVIDER_virtual/wpebackend ?= "wpebackend-fdo"
PACKAGECONFIG ?= " \
    ${@bb.utils.contains('PREFERRED_PROVIDER_virtual/wpebackend', 'wpebackend-fdo', 'drm', '', d)} \
    ${COG_PACKAGECONFIG} \
"

PACKAGECONFIG:class-devupstream ?= " \
    ${@bb.utils.contains('PREFERRED_PROVIDER_virtual/wpebackend', 'wpebackend-fdo', 'drm', '', d)} \
    ${COG_PACKAGECONFIG} \
"

# libcogplatform*.so are plugins that should go on the main package (not on -dev)
# https://github.com/Igalia/cog/commit/758ed08555e8152a2becd2178d1f3a4ce6e67af9
# Also libWPEBackend-default.so should go into the main package.
FILES_SOLIBSDEV = "${libdir}/libcogcore*.so"

FILES:${PN} += " \
    ${libdir}/libcogplatform*.so \
    ${datadir}/dbus-1/system.d/com.igalia.Cog.conf \
"
INSANE_SKIP:${PN} = "dev-so"

# Local user name starting cog with DBus system session enabled. This should be
# set only if the dbus PACKAGECONFIG is enabled.
COG_DBUS_OWN_USER ?= ""

COG_PACKAGECONFIG := " \
    ${@bb.utils.contains('PREFERRED_PROVIDER_virtual/wpebackend', 'wpebackend-fdo', \
                         bb.utils.contains('DISTRO_FEATURES', 'wayland', 'wl', '', d), \
                         '', d)}"

