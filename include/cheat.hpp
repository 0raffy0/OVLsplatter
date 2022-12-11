/**
 * Copyright (C) 2019 - 2020 WerWolv
 * 
 * This file is part of EdiZon.
 * 
 * EdiZon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * EdiZon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with EdiZon.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once


#include <string>
#include <vector>
#include <switch.h>
#include "cheat_engine_types.hpp"
#include "results.hpp"


static Service g_dmntchtSrv;





namespace edz::cheat {

    class CheatManager {
    public:
		static bool isCheatServiceAvailable();
	
        static u64 getBuildID();

        static types::Region getBaseRegion();
        static types::Region getHeapRegion();
        static types::Region getMainRegion();
        static types::Region getAliasRegion();
		
        static Result writeMemory(u64 address, const void *buffer, size_t bufferSize);
		
	private:
        static inline DmntCheatProcessMetadata s_processMetadata;
    };

}