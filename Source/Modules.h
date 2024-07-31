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

#pragma once

#include "Game.h"
#include "Utils.h"

#define DISABLE_IMGUI

#include <Macchiato/Macchiato.h>
#include <Macchiato/Patch/Trampoline.h>
#include <Macchiato/Utils/Bounds.h>
#include <Macchiato/Utils/Filesystem.h>
#include <Macchiato/Utils/Time.h>

#include <sdl-utils/Types.h>

#include <array>
#include <functional>
#include <string>
#include <unordered_set>

namespace SDLGrabber {
    struct MatchData {
        bool logged = false;
    };

    TRAMPOLINE(MainMgrVSGame_FUN_0e74c0a0, void, void* _this) {
        orig_MainMgrVSGame_FUN_0e74c0a0(_this);

        MatchData* state = StateManager::getState<MatchData>();
        if (state->logged)
            return;

        std::vector<Game::Player*> players = Game::getAllPlayers();
        if (players.size() <= 1) {
            return;
        }

        u32 sessionId = Lp::Sys::BossUploadDataMaker_getSessionID();

        std::stringstream log;

        log << "========================================================="
            << "\n";
        log << "SDL Grabber Report"
            << "\n";
        log << "Developed by splatoon1enjoyer in the SDL Foundation "
               "(https://sdlfoundation.com)"
            << "\n";
        log << "Download at: https://github.com/sdlfoundation/sdl-grabber"
            << "\n";
        log << "========================================================="
            << "\n";

        log << "\n";

        log << "Match Date: " << Macchiato::Utils::getCurrentDate() << "\n";
        log << "Match Time: " << Macchiato::Utils::getCurrentTime() << "\n";
        log << "Session ID: " << sessionId << "\n";

        log << "\n";

        std::unordered_set<u32> usedPlayerIndices = {};

        for (const auto& player : players) {
            std::u16string utf16Name;
            utf16Name.reserve(player->info->name.size());

            for (s16 s : player->info->name) {
                if (s == 0)
                    break;

                utf16Name.push_back(static_cast<char16_t>(s));
            }

            std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>
                        convert;
            std::string name = convert.to_bytes(utf16Name);

            log << "P" << std::to_string(player->info->index) << " ";

            log << "PID: " << std::to_string(player->info->pid);
            log << ", Name: " << name;
            log << ", Area: " << std::to_string(player->info->area);
            log << ", Index: " << std::to_string(player->info->index);
            log << ", Gender: " << std::to_string(player->info->gender);
            log << ", Skin Tone: " << std::to_string(player->info->skinTone);
            log << ", Eye Color: " << std::to_string(player->info->eyeColor);
            log << ", Weapon Set ID: "
                << std::to_string(player->info->fullWeaponId);
            log << ", Main Weapon ID: "
                << std::to_string(player->info->weaponId);
            log << ", Sub Weapon ID: "
                << std::to_string(player->info->subWeaponId);
            log << ", Special Weapon ID: "
                << std::to_string(player->info->specialWeaponId);

            log << "\n";
        }

        std::string logStr = log.str();

        Macchiato::Utils::FS::writeProgramFile("log.txt", logStr);
    }

    TRAMPOLINE(result, void, void* _this, s32 param_1) {
        orig_result(_this, param_1);

        MatchData* state = StateManager::getState<MatchData>();
        state->logged    = false;
    }

    MODULE(log) {
        StateManager::useState<MatchData>();

        return Module::create("Log")
            .withPatch(
                Patch::create()
                    .withTrampoline(INSTALL_TRAMPOLINE(
                        MainMgrVSGame_FUN_0e74c0a0, 0x0e74c0a0))
                    .withTrampoline(INSTALL_TRAMPOLINE(result, 0x0e00af90)))
            .withAutoEnabled(true);
    }

    Directory rootDir() {
        return Macchiato::dir("Root", {log});
    }

    DirectoryRoot root() {
        return Macchiato::root({rootDir});
    }
} // namespace SDLGrabber
