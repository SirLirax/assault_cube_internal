// Created with ReClass.NET 1.2 by KN4CK3R
#include "vector_helper.h"
#include <cstdint>
class entity
{
public:
	char pad_0000[4]; //0x0000
	Vector3 entity_origin; //0x0004
	Vector3 entity_velocity; //0x0010
	Vector3 entity_deltapos; //0x001C
	Vector3 entity_headpos; //0x0028
	Vector3 entity_viewangles; //0x0034
	float entity_pitch_velocity; //0x0040
	float entity_maxspeed; //0x0044 crashes when above 16
	int32_t enity_time_in_air; //0x0048
	float entity_collision_radius; //0x004C set this to negative for noclip
	float entity_eyeheight; //0x0050
	float entity_max_eye_height; //0x0054
	float enity_above_eye; //0x0058
	bool enity_in_water; //0x005C
	bool enity_on_floor; //0x005D
	bool enity_on_ladder; //0x005E
	bool enity_jump_next; //0x005F set this to 1 for bhop
	bool enity_jumpd; //0x0060
	bool enity_crouching; //0x0061
	bool entity_crouched_in_air; //0x0062
	bool enity_try_crouch; //0x0063
	bool enity_cancollide; //0x0064
	bool enity_stuck; //0x0065
	bool enity_scoping; //0x0066
	int32_t entity_last_jump; //0x0067
	float entity_last_jump_height; //0x006B
	char padding[1]; //0x006F
	char N00000282[4]; //0x0070
	char N00000283[4]; //0x0074
	char N00000284[4]; //0x0078
	char N00000285[4]; //0x007C
	char N00000286[4]; //0x0080
	char N00000287[4]; //0x0084
	char N00000288[4]; //0x0088
	char N00000289[4]; //0x008C
	char N0000028A[4]; //0x0090
	char N0000028B[4]; //0x0094
	char N0000028C[4]; //0x0098
	char N0000028D[4]; //0x009C
	char N0000028E[4]; //0x00A0
	char N0000028F[4]; //0x00A4
	char N00000290[4]; //0x00A8
	char N00000291[4]; //0x00AC
	char N00000292[4]; //0x00B0
	char N00000293[4]; //0x00B4
	char N00000294[4]; //0x00B8
	char N00000295[4]; //0x00BC
	char N00000296[4]; //0x00C0
	char N00000297[4]; //0x00C4
	char N00000298[4]; //0x00C8
	char N00000299[4]; //0x00CC
	char N0000029A[4]; //0x00D0
	char N0000029B[4]; //0x00D4
	char N0000029C[4]; //0x00D8
	char N0000029D[4]; //0x00DC
	char N0000029E[4]; //0x00E0
	char N0000029F[4]; //0x00E4
	char N000002A0[3]; //0x00E8
	int32_t entity_health; //0x00EC
	int32_t entity_armor; //0x00F0
	int32_t entity_primary; //0x00F4
	int32_t entity_next_primary; //0x00F8
	int32_t entity_gunselect; //0x00FC
	bool entity_akimbo; //0x0100
	char padding1[2]; //0x0101
	char padding2[1]; //0x0103
	char pad_0104[40]; //0x0104
	int32_t pistol_ammo; //0x012C
	char pad_0130[4]; //0x0130
	int32_t shotgun_ammo; //0x0134
	int32_t sub_gun_ammo; //0x0138
	int32_t sniper_ammo; //0x013C
	int32_t assault_ammo; //0x0140
	int32_t grenade_ammo; //0x0144
	char pad_0148[188]; //0x0148
	bool entity_attack; //0x0204
	char entity_name[16]; //0x0205
	char padding3[3]; //0x0215
	char pad_0218[244]; //0x0218
	int32_t team; //0x030C
	char pad_0310[36]; //0x0310
	int32_t entity_fov; //0x0334 read only
	int32_t entity_sniper_fov; //0x0338 read only
	char pad_033C[44]; //0x033C
	class weapon* current_weapon; //0x0368
}; //Size: 0x036C


class weapon
{
public:
	char pad_0000[8]; //0x0000
	class entity* weapon_owner_pointer; //0x0008
	class weapon_info* weapon_info_pointer; //0x000C
	char pad_0010[4]; //0x0010
	class weapon_ammo* weapon_ammo_pointer; //0x0014
};

class weapon_info
{
public:
	char gun_name[23]; //0x0000
	char gun_title[42]; //0x0017
	short gun_sound; //0x0041
	short gun_reload; //0x0043
	short gun_reloadtime; //0x0045
	short gun_attackdelay; //0x0047
	short gun_damage; //0x0049
	short gun_piercing; //0x004B
	short gun_projspeed; //0x004D
	short gun_part; //0x004F
	short gun_spread; //0x0051
	short gun_recoil; //0x0053
	short gun_magsize; //0x0055
	short gun_mdl_kick_rot; //0x0057
	short gun_mdl_kick_back; //0x0059
	short gun_recoil_increase; //0x005B
	short gun_recoilbase; //0x005D
	short gun_maxrecoil; //0x005F
	short gun_recoilbackfade; //0x0061
	short gun_pushfactor; //0x0063
	char paddingx[1]; //0x0065
	bool is_automatic; //0x0066
}; //Size: 0x0067

class weapon_ammo
{
public:
	int weapon_current_ammo; //0x0000
};