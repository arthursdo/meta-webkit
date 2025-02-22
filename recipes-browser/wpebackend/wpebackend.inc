SUMMARY = "General-purpose library specifically developed for the WPE-flavored port of WebKit."
HOMEPAGE = "https://github.com/WebPlatformForEmbedded/WPEBackend"
BUGTRACKER = "https://github.com/WebPlatformForEmbedded/WPEBackend/issues"

LICENSE = "BSD-2-Clause"
LIC_FILES_CHKSUM = "file://COPYING;md5=6ae4db0d4b812334e1539cd5aa6e2f46"
DEPENDS:append = " virtual/egl libxkbcommon"

PROVIDES += "virtual/libwpe"
RPROVIDES:${PN} += "virtual/libwpe"

# Workaround build issue with RPi userland EGL libraries.
CFLAGS:append:rpi = " ${@bb.utils.contains('MACHINE_FEATURES', 'vc4graphics', '', '-D_GNU_SOURCE', d)}"

inherit cmake
