// Created with ReClass.NET 1.2 by KN4CK3R
#include "vector_helper.h"

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
	int enity_time_in_air; //0x0048
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
	int entity_last_jump; //0x0067
	float entity_last_jump_height; //0x006B
	char padding1[1]; //0x006F
	char pad_0070[124]; //0x0070
	int entity_health; //0x00EC
	int entity_armor; //0x00F0
	int entity_primary; //0x00F4
	int entity_next_primary; //0x00F8
	int entity_gunselect; //0x00FC
	bool entity_akimbo; //0x0100
	char padding2[2]; //0x0101
	char padding3[1]; //0x0103
	char pad_0104[40]; //0x0104
	int pistol_ammo; //0x012C
	char pad_0130[4]; //0x0130
	int shotgun_ammo; //0x0134
	int sub_gun_ammo; //0x0138
	int sniper_ammo; //0x013C
	int assault_ammo; //0x0140
	int grenade_ammo; //0x0144
	char pad_0148[188]; //0x0148
	bool entity_attack; //0x0204
	char entity_name[16]; //0x0205
	char padding4[3]; //0x0215
	char pad_0218[244]; //0x0218
	int team; //0x030C
	char pad_0310[36]; //0x0310
	int entity_fov; //0x0334 read only
	int entity_sniper_fov; //0x0338 read only
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