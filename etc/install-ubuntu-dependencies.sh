#!/usr/bin/env bash
# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

set -o errexit

#
# The following xorg/system dependencies are required when opencv has to be built from source
# and cannot be loaded from conan-center in a pre-built version (non-std-compiler/config).
# They are required for highgui features. You can install them manually or
# set tools.system.package_manager:mode=install and tools.system.package_manager:sudo=True
# in conan config to get them installed via conan install as shown further below in "Setup conan" step.
# But the conan config breaks with dependency caching mechanism from github actions so
# we prefer an explicit installation here.
#
sudo apt-get install --no-install-recommends -y \
    libx11-xcb-dev libfontenc-dev libxaw7-dev libxmu-dev libxmuu-dev libxpm-dev libxres-dev \
    libxss-dev libxtst-dev libxv-dev libxvmc-dev libxxf86vm-dev libxcb-render-util0-dev \
    libxcb-xkb-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-randr0-dev \
    libxcb-shape0-dev libxcb-sync-dev libxcb-xfixes0-dev libxcb-xinerama0-dev libxcb-dri3-dev libxcb-cursor-dev \
    libxcb-util-dev libxcb-util0-dev libxcb-glx0-dev libxcb-dri2-0-dev libxcb-present-dev libxcb-composite0-dev \
    libxcb-ewmh-dev libxcb-res0-dev libxkbfile-dev
