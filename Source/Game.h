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

#include <array>
#include <sdl-utils/Types.h>

#include "Macchiato/Utils/Bind.h"

namespace Cmn {
    struct PlayerInfo {
        u32 index;
        s16 unk1;

        std::array<s16, 17> name;

        u32 country;
        u32 area;
        u32 team;
        u32 gender;
        u32 skinTone;
        u32 eyeColor;

        s32 fullWeaponId;
        s32 weaponId;
        s32 subWeaponId;
        s32 specialWeaponId;

        std::array<char, 0x80> unk2;
        s32                    pid;
    };

    static_assert(offsetof(PlayerInfo, index) == 0x0);
    static_assert(offsetof(PlayerInfo, name) == 0x6);
    static_assert(offsetof(PlayerInfo, country) == 0x28);
    static_assert(offsetof(PlayerInfo, area) == 0x2c);
    static_assert(offsetof(PlayerInfo, team) == 0x30);
    static_assert(offsetof(PlayerInfo, gender) == 0x34);
    static_assert(offsetof(PlayerInfo, skinTone) == 0x38);
    static_assert(offsetof(PlayerInfo, eyeColor) == 0x3c);
    static_assert(offsetof(PlayerInfo, pid) == 0xd0);
} // namespace Cmn

namespace Game {
    struct Player {
        std::array<char, 0x78> unk1;
        u32                    weaponId;

        std::array<char, 0x28> unk2;
        Cmn::PlayerInfo*       info;
    };

    static_assert(offsetof(Player, weaponId) == 0x78);
    static_assert(offsetof(Player, info) == 0xA4);

    struct PlayerFactoryIdx {
        u32            maxPlayerCount;
        Game::Player** playerActors;
    };

    static_assert(offsetof(PlayerFactoryIdx, maxPlayerCount) == 0x0);
    static_assert(offsetof(PlayerFactoryIdx, playerActors) == 0x4);

    struct PlayerFactory {
        PlayerFactoryIdx* playerFactoryIdx;
    };

    static_assert(offsetof(PlayerFactory, playerFactoryIdx) == 0x0);

    struct PlayerMgr {
        std::array<char, 0x30> unk1;
        Game::PlayerFactory*   factories;

        std::array<char, 0x2B8> unk3;
        u32                     playerCount;
    };

    static_assert(offsetof(PlayerMgr, factories) == 0x30);
    static_assert(offsetof(PlayerMgr, playerCount) == 0x2EC);

    struct MainMgrBase {
        std::array<char, 0x268> unk1;
        PlayerMgr*              playerMgr;
    };

    BIND_VAR(MainMgrBase_sInstance, 0x106E5814, MainMgrBase*);

    static_assert(offsetof(MainMgrBase, playerMgr) == 0x268);

    std::vector<Game::Player*> getAllPlayers() {
        if (!Game::MainMgrBase_sInstance)
            return {};
        if (!Game::MainMgrBase_sInstance->playerMgr)
            return {};
        if (!Game::MainMgrBase_sInstance->playerMgr->factories)
            return {};

        Game::Player** players =
            Game::MainMgrBase_sInstance->playerMgr->factories->playerFactoryIdx
                ->playerActors;
        u32 playerCount = Game::MainMgrBase_sInstance->playerMgr->factories
                              ->playerFactoryIdx->maxPlayerCount;
        if (playerCount == 0)
            return {};

        std::vector<Game::Player*> result = {};

        for (u32 i = 0; i < playerCount; i++) {
            Game::Player* player = players[i];
            if (!player)
                continue;

            result.push_back(player);
        }

        return result;
    }
} // namespace Game

namespace Lp::Sys {
    BIND_FUNC(BossUploadDataMaker_getSessionID, 0x0eb34750, u32);
}
