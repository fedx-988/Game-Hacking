#ifndef OFFSETS_H
#define OFFSETS_H

const DWORD PISTOL_AMMO_OFFSET = 0x12C;       // Offset for pistol ammo
const DWORD HEALTH_OFFSET = 0xEC;             // Offset for health
const DWORD BASE_OFFSET = 0x00195404;         // Base address offset to get the main data structure
const DWORD AR_AMMO_OFFSET = 0x140;           // Offset for AR ammo
const DWORD GRENADE_AMMO_OFFSET = 0x144;      // Offset for grenade ammo
const DWORD DUAL_PISTOL_AMMO_OFFSET = 0x148;  // Offset for dual pistols ammo
const DWORD ARMOUR_OFFSET = 0xF0;             // Offset for armour
const DWORD PLAYER_COUNT_OFFSET = 0x18AC0C;   // Offset for player count (should be added to base address)
const DWORD X_HEAD_COORD_OFFSET = 0x4;        // Offset for X Head Coordinate
const DWORD Y_HEAD_COORD_OFFSET = 0x8;        // Offset for Y Head Coordinate
const DWORD Z_HEAD_COORD_OFFSET = 0xC;        // Offset for Z Head Coordinate
const DWORD LOCAL_PLAYER_OFFSET = BASE_OFFSET;// Local player offset (if needed)
const DWORD ENTITY_LIST_OFFSET = 0x0018AC04;  // Offset for Entity List

#endif
