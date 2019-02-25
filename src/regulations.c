/***************************************************************************
 *                                                                         *
 *          ###########   ###########   ##########    ##########           *
 *         ############  ############  ############  ############          *
 *         ##            ##            ##   ##   ##  ##        ##          *
 *         ##            ##            ##   ##   ##  ##        ##          *
 *         ###########   ####  ######  ##   ##   ##  ##    ######          *
 *          ###########  ####  #       ##   ##   ##  ##    #    #          *
 *                   ##  ##    ######  ##   ##   ##  ##    #    #          *
 *                   ##  ##    #       ##   ##   ##  ##    #    #          *
 *         ############  ##### ######  ##   ##   ##  ##### ######          *
 *         ###########    ###########  ##   ##   ##   ##########           *
 *                                                                         *
 *            S E C U R E   M O B I L E   N E T W O R K I N G              *
 *                                                                         *
 * This file is part of NexMon.                                            *
 *                                                                         *
 * Copyright (c) 2016 NexMon Team                                          *
 *                                                                         *
 * NexMon is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * NexMon is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with NexMon. If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                         *
 **************************************************************************/

#pragma NEXMON targetregion "patch"

#include <firmware_version.h>
#include <wrapper.h>	// wrapper definitions for functions that already exist in the firmware
#include <structs.h>	// structures that are used by the code in the firmware
#include <patcher.h>
#include <helper.h>
#include <channels.h>

// Nop the following call to keep user tx power targets
//    Choose least of user and now combined regulatory/hw targets
//    ppr_compare_min(tx_pwr_target, srom_max_txpwr);
__attribute__((at(0x2357A, "", CHIP_VER_BCM43430a1, FW_VER_7_45_41_26_r640327)))
__attribute__((at(0x239a6, "", CHIP_VER_BCM43430a1, FW_VER_7_45_41_46)))
GenericPatch4(nop_ppr_compare_min, 0);

// Enable all chanspecs, even malformed ones
int
wlc_valid_chanspec_ext_hook(unsigned short chanspec)
{
  return 0;
}

__attribute__((at(0x80506C, "flashpatch", CHIP_VER_BCM43430a1, FW_VER_ALL)))
BPatch(wlc_valid_chanspec_ext_hook, wlc_valid_chanspec_ext_hook);

// This patches the valid chanspecs to be those required by our experiment
int
wlc_valid_chanspec_hook(void *wlc_cm, unsigned short chanspec, int dualband)
{
  if ((chanspec & WL_CHANSPEC_BW_20) || (chanspec & WL_CHANSPEC_BW_40))
    return 1;

  return 0;
}

__attribute__((at(0x84EEA0, "flashpatch", CHIP_VER_BCM43430a1, FW_VER_ALL)))
BPatch(wlc_valid_chanspec_hook, wlc_valid_chanspec_hook);