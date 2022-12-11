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


#include <cstring>
#include "cheat.hpp"


 #define serviceDispatchIn(_s,_rid,_in,...) \
     serviceDispatchImpl((_s),(_rid),&(_in),sizeof(_in),NULL,0,(SfDispatchParams){ __VA_ARGS__ })


Result dmntchtWriteCheatProcessMemory(u64 address, const void *buffer, size_t size);


bool isServiceRunning(const char *serviceName);







namespace edz::cheat {
	
	
	bool CheatManager::isCheatServiceAvailable() {
        static s8 running = -1;
        if (running == -1) 
            running = isServiceRunning("dmnt:cht");

        return !!running; 
    }
	
	

    u64 CheatManager::getBuildID() {
        u64 buildid = 0;

        std::memcpy(&buildid, CheatManager::s_processMetadata.main_nso_build_id, sizeof(u64));
        
        return __builtin_bswap64(buildid);;
    }


    types::Region CheatManager::getBaseRegion() {
        if (!CheatManager::isCheatServiceAvailable())
            return { };

        return types::Region{ CheatManager::s_processMetadata.address_space_extents.base, CheatManager::s_processMetadata.address_space_extents.size };
    }

    types::Region CheatManager::getHeapRegion() {
        if (!CheatManager::isCheatServiceAvailable())
            return { };

        return types::Region{ CheatManager::s_processMetadata.heap_extents.base, CheatManager::s_processMetadata.heap_extents.size };
    }

    types::Region CheatManager::getMainRegion() {
        if (!CheatManager::isCheatServiceAvailable())
            return { };

        return types::Region{ CheatManager::s_processMetadata.main_nso_extents.base, CheatManager::s_processMetadata.main_nso_extents.size };
    }

    types::Region CheatManager::getAliasRegion() {
        if (!CheatManager::isCheatServiceAvailable())
            return { };

        return types::Region{ CheatManager::s_processMetadata.alias_extents.base, CheatManager::s_processMetadata.alias_extents.size };
    }

    Result CheatManager::writeMemory(u64 address, const void *buffer, size_t bufferSize) {
        if (!CheatManager::isCheatServiceAvailable())
            return ResultEdzCheatServiceNotAvailable;

        return dmntchtWriteCheatProcessMemory(address, buffer, bufferSize);
    }
}






Result dmntchtWriteCheatProcessMemory(u64 address, const void *buffer, size_t size) {
    const struct {
        u64 address;
        u64 size;
    } in = { address, size };
    return serviceDispatchIn(&g_dmntchtSrv, 65103, in,
        .buffer_attrs = { SfBufferAttr_In | SfBufferAttr_HipcMapAlias },
        .buffers = { { buffer, size } },
    );
}



bool isServiceRunning(const char *serviceName) {
  Handle handle;
  SmServiceName service_name = smEncodeName(serviceName);
  bool running = R_FAILED(smRegisterService(&handle, service_name, false, 1));

  svcCloseHandle(handle);

  if (!running)
    smUnregisterService(service_name);

  return running;
}
