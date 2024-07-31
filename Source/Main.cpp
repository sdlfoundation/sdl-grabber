/*
 * sdl-grabber - Splatoon match infologger
 * Copyright (C) 2024 splatoon1enjoyer @ SDL Foundation
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "Modules.h"

#include <Macchiato/Macchiato.h>
#include <coreinit/dynload.h>
#include <wups.h>

#define DISABLE_IMGUI

WUPS_PLUGIN_NAME("SDL Grabber");
WUPS_PLUGIN_DESCRIPTION("Splatoon match infologger");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("splatoon1enjoyer - SDL Foundation");
WUPS_PLUGIN_LICENSE("AGPL v3.0");
WUPS_USE_STORAGE("sdl-grabber");

// Filesystem access
WUPS_USE_WUT_DEVOPTAB();

static Macchiato::Macchiato* macchiatoInstance = nullptr;

INITIALIZE_PLUGIN() {
    Macchiato::VersionTriplet triplet = {};

    Macchiato::RepositoryInfo repository = Macchiato::RepositoryInfo::create(
        Macchiato::RepositoryService::GitHub, "sdlfoundation", "sdl-grabber");

    Macchiato::AppInfo info =
        Macchiato::AppInfo::create(triplet, repository, "SDL Grabber");

    Macchiato::Config config;
    config.targetExecutableNames = {"Gambit.rpx"};

    macchiatoInstance =
        Macchiato::Macchiato::createHeap(info, SDLGrabber::root, config);
    if (!macchiatoInstance)
        return;

    macchiatoInstance->registerToWiiUMenu();
}

DEINITIALIZE_PLUGIN() {
    if (macchiatoInstance)
        delete macchiatoInstance;
}

ON_APPLICATION_START() {
    if (macchiatoInstance)
        macchiatoInstance->init();
}

ON_APPLICATION_ENDS() {
    if (macchiatoInstance)
        macchiatoInstance->deinit();
}

ON_APPLICATION_REQUESTS_EXIT() {
}
