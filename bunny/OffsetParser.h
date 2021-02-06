//  Then include this file, and then do
//
//     Offsets data = nlohmann::json::parse(jsonString);

#pragma once

#include "thirdparty/json.hpp"
using json = nlohmann::json;

namespace OffsettParser {
    using nlohmann::json;

    inline json get_untyped(const json &j, const char *property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json &j, std::string property) {
        return get_untyped(j, property.data());
    }

    struct Netvars {
        unsigned long cs_gamerules_data;
        unsigned long m_armor_value;
        unsigned long m_collision;
        unsigned long m_collision_group;
        unsigned long m_local;
        unsigned long m_move_type;
        unsigned long m_original_owner_xuid_high;
        unsigned long m_original_owner_xuid_low;
        unsigned long m_survival_game_rule_decision_types;
        unsigned long m_survival_rules;
        unsigned long m_aim_punch_angle;
        unsigned long m_aim_punch_angle_vel;
        unsigned long m_ang_eye_angles_x;
        unsigned long m_ang_eye_angles_y;
        unsigned long m_b_bomb_planted;
        unsigned long m_b_freeze_period;
        unsigned long m_b_gun_game_immunity;
        unsigned long m_b_has_defuser;
        unsigned long m_b_has_helmet;
        unsigned long m_b_in_reload;
        unsigned long m_b_is_defusing;
        unsigned long m_b_is_queued_matchmaking;
        unsigned long m_b_is_scoped;
        unsigned long m_b_is_valve_ds;
        unsigned long m_b_spotted;
        unsigned long m_b_spotted_by_mask;
        unsigned long m_b_started_arming;
        unsigned long m_clr_render;
        unsigned long m_dw_bone_matrix;
        unsigned long m_f_accuracy_penalty;
        unsigned long m_f_flags;
        unsigned long m_fl_c4_blow;
        unsigned long m_fl_defuse_count_down;
        unsigned long m_fl_defuse_length;
        unsigned long m_fl_fallback_wear;
        unsigned long m_fl_flash_duration;
        unsigned long m_fl_flash_max_alpha;
        unsigned long m_fl_last_bone_setup_time;
        unsigned long m_fl_lower_body_yaw_target;
        unsigned long m_fl_next_attack;
        unsigned long m_fl_next_primary_attack;
        unsigned long m_fl_simulation_time;
        unsigned long m_fl_timer_length;
        unsigned long m_h_active_weapon;
        unsigned long m_h_my_weapons;
        unsigned long m_h_observer_target;
        unsigned long m_h_owner;
        unsigned long m_h_owner_entity;
        unsigned long m_i_account_id;
        unsigned long m_i_clip1;
        unsigned long m_i_competitive_ranking;
        unsigned long m_i_competitive_wins;
        unsigned long m_i_crosshair_id;
        unsigned long m_i_entity_quality;
        unsigned long m_i_fov_start;
        unsigned long m_i_glow_index;
        unsigned long m_i_health;
        unsigned long m_i_item_definition_index;
        unsigned long m_i_item_id_high;
        unsigned long m_i_most_recent_model_bone_counter;
        unsigned long m_i_observer_mode;
        unsigned long m_i_shots_fired;
        unsigned long m_i_state;
        unsigned long m_i_team_num;
        unsigned long m_life_state;
        unsigned long m_n_fallback_paint_kit;
        unsigned long m_n_fallback_seed;
        unsigned long m_n_fallback_stat_trak;
        unsigned long m_n_force_bone;
        unsigned long m_n_tick_base;
        unsigned long m_rgfl_coordinate_frame;
        unsigned long m_sz_custom_name;
        unsigned long m_sz_last_place_name;
        unsigned long m_third_person_view_angles;
        unsigned long m_vec_origin;
        unsigned long m_vec_velocity;
        unsigned long m_vec_view_offset;
        unsigned long m_view_punch_angle;
    };

    struct Signatures {
        unsigned long clientstate_choked_commands;
        unsigned long clientstate_delta_ticks;
        unsigned long clientstate_last_outgoing_command;
        unsigned long clientstate_net_channel;
        unsigned long convar_name_hash_table;
        unsigned long dw_client_state;
        unsigned long dw_client_state_get_local_player;
        unsigned long dw_client_state_is_hltv;
        unsigned long dw_client_state_map;
        unsigned long dw_client_state_map_directory;
        unsigned long dw_client_state_max_player;
        unsigned long dw_client_state_player_info;
        unsigned long dw_client_state_state;
        unsigned long dw_client_state_view_angles;
        unsigned long dw_entity_list;
        unsigned long dw_force_attack;
        unsigned long dw_force_attack2;
        unsigned long dw_force_backward;
        unsigned long dw_force_forward;
        unsigned long dw_force_jump;
        unsigned long dw_force_left;
        unsigned long dw_force_right;
        unsigned long dw_game_dir;
        unsigned long dw_game_rules_proxy;
        unsigned long dw_get_all_classes;
        unsigned long dw_global_vars;
        unsigned long dw_glow_object_manager;
        unsigned long dw_input;
        unsigned long dw_interface_link_list;
        unsigned long dw_local_player;
        unsigned long dw_mouse_enable;
        unsigned long dw_mouse_enable_ptr;
        unsigned long dw_player_resource;
        unsigned long dw_radar_base;
        unsigned long dw_sensitivity;
        unsigned long dw_sensitivity_ptr;
        unsigned long dw_set_clan_tag;
        unsigned long dw_view_matrix;
        unsigned long dw_weapon_table;
        unsigned long dw_weapon_table_index;
        unsigned long dw_yaw_ptr;
        unsigned long dw_zoom_sensitivity_ratio_ptr;
        unsigned long dwb_send_packets;
        unsigned long dwpp_direct3_d_device9;
        unsigned long force_update_spectator_glow;
        unsigned long interface_engine_cvar;
        unsigned long is_c4_owner;
        unsigned long m_b_dormant;
        unsigned long m_p_studio_hdr;
        unsigned long m_pitch_class_ptr;
        unsigned long m_yaw_class_ptr;
        unsigned long model_ambient_min;
        unsigned long set_abs_angles;
        unsigned long set_abs_origin;
    };

    struct Offsets {
        unsigned long timestamp;
        Signatures signatures;
        Netvars netvars;
    };

        void from_json(const json &j, OffsettParser::Netvars &x);

        void to_json(json &j, const OffsettParser::Netvars &x);

        void from_json(const json &j, OffsettParser::Signatures &x);

        void to_json(json &j, const OffsettParser::Signatures &x);

        void from_json(const json &j, OffsettParser::Offsets &x);

        void to_json(json &j, const OffsettParser::Offsets &x);

        inline void from_json(const json &j, OffsettParser::Netvars &x) {

            x.cs_gamerules_data = j.at("cs_gamerules_data").get<long int>();
            x.m_armor_value = j.at("m_ArmorValue").get<long int>();
            x.m_collision = j.at("m_Collision").get<long int>();
            x.m_collision_group = j.at("m_CollisionGroup").get<long int>();
            x.m_local = j.at("m_Local").get<long int>();
            x.m_move_type = j.at("m_MoveType").get<long int>();
            x.m_original_owner_xuid_high = j.at("m_OriginalOwnerXuidHigh").get<long int>();
            x.m_original_owner_xuid_low = j.at("m_OriginalOwnerXuidLow").get<long int>();
            x.m_survival_game_rule_decision_types = j.at("m_SurvivalGameRuleDecisionTypes").get<long int>();
            x.m_survival_rules = j.at("m_SurvivalRules").get<long int>();
            x.m_aim_punch_angle = j.at("m_aimPunchAngle").get<long int>();
            x.m_aim_punch_angle_vel = j.at("m_aimPunchAngleVel").get<long int>();
            x.m_ang_eye_angles_x = j.at("m_angEyeAnglesX").get<long int>();
            x.m_ang_eye_angles_y = j.at("m_angEyeAnglesY").get<long int>();
            x.m_b_bomb_planted = j.at("m_bBombPlanted").get<long int>();
            x.m_b_freeze_period = j.at("m_bFreezePeriod").get<long int>();
            x.m_b_gun_game_immunity = j.at("m_bGunGameImmunity").get<long int>();
            x.m_b_has_defuser = j.at("m_bHasDefuser").get<long int>();
            x.m_b_has_helmet = j.at("m_bHasHelmet").get<long int>();
            x.m_b_in_reload = j.at("m_bInReload").get<long int>();
            x.m_b_is_defusing = j.at("m_bIsDefusing").get<long int>();
            x.m_b_is_queued_matchmaking = j.at("m_bIsQueuedMatchmaking").get<long int>();
            x.m_b_is_scoped = j.at("m_bIsScoped").get<long int>();
            x.m_b_is_valve_ds = j.at("m_bIsValveDS").get<long int>();
            x.m_b_spotted = j.at("m_bSpotted").get<long int>();
            x.m_b_spotted_by_mask = j.at("m_bSpottedByMask").get<long int>();
            x.m_b_started_arming = j.at("m_bStartedArming").get<long int>();
            x.m_clr_render = j.at("m_clrRender").get<long int>();
            x.m_dw_bone_matrix = j.at("m_dwBoneMatrix").get<long int>();
            x.m_f_accuracy_penalty = j.at("m_fAccuracyPenalty").get<long int>();
            x.m_f_flags = j.at("m_fFlags").get<long int>();
            x.m_fl_c4_blow = j.at("m_flC4Blow").get<long int>();
            x.m_fl_defuse_count_down = j.at("m_flDefuseCountDown").get<long int>();
            x.m_fl_defuse_length = j.at("m_flDefuseLength").get<long int>();
            x.m_fl_fallback_wear = j.at("m_flFallbackWear").get<long int>();
            x.m_fl_flash_duration = j.at("m_flFlashDuration").get<long int>();
            x.m_fl_flash_max_alpha = j.at("m_flFlashMaxAlpha").get<long int>();
            x.m_fl_last_bone_setup_time = j.at("m_flLastBoneSetupTime").get<long int>();
            x.m_fl_lower_body_yaw_target = j.at("m_flLowerBodyYawTarget").get<long int>();
            x.m_fl_next_attack = j.at("m_flNextAttack").get<long int>();
            x.m_fl_next_primary_attack = j.at("m_flNextPrimaryAttack").get<long int>();
            x.m_fl_simulation_time = j.at("m_flSimulationTime").get<long int>();
            x.m_fl_timer_length = j.at("m_flTimerLength").get<long int>();
            x.m_h_active_weapon = j.at("m_hActiveWeapon").get<long int>();
            x.m_h_my_weapons = j.at("m_hMyWeapons").get<long int>();
            x.m_h_observer_target = j.at("m_hObserverTarget").get<long int>();
            x.m_h_owner = j.at("m_hOwner").get<long int>();
            x.m_h_owner_entity = j.at("m_hOwnerEntity").get<long int>();
            x.m_i_account_id = j.at("m_iAccountID").get<long int>();
            x.m_i_clip1 = j.at("m_iClip1").get<long int>();
            x.m_i_competitive_ranking = j.at("m_iCompetitiveRanking").get<long int>();
            x.m_i_competitive_wins = j.at("m_iCompetitiveWins").get<long int>();
            x.m_i_crosshair_id = j.at("m_iCrosshairId").get<long int>();
            x.m_i_entity_quality = j.at("m_iEntityQuality").get<long int>();

            x.m_i_fov_start = j.at("m_iFOVStart").get<long int>();
            x.m_i_glow_index = j.at("m_iGlowIndex").get<long int>();
            x.m_i_health = j.at("m_iHealth").get<long int>();
            x.m_i_item_definition_index = j.at("m_iItemDefinitionIndex").get<long int>();
            x.m_i_item_id_high = j.at("m_iItemIDHigh").get<long int>();
            x.m_i_most_recent_model_bone_counter = j.at("m_iMostRecentModelBoneCounter").get<long int>();
            x.m_i_observer_mode = j.at("m_iObserverMode").get<long int>();
            x.m_i_shots_fired = j.at("m_iShotsFired").get<long int>();
            x.m_i_state = j.at("m_iState").get<long int>();
            x.m_i_team_num = j.at("m_iTeamNum").get<long int>();
            x.m_life_state = j.at("m_lifeState").get<long int>();
            x.m_n_fallback_paint_kit = j.at("m_nFallbackPaintKit").get<long int>();
            x.m_n_fallback_seed = j.at("m_nFallbackSeed").get<long int>();
            x.m_n_fallback_stat_trak = j.at("m_nFallbackStatTrak").get<long int>();
            x.m_n_force_bone = j.at("m_nForceBone").get<long int>();
            x.m_n_tick_base = j.at("m_nTickBase").get<long int>();
            x.m_rgfl_coordinate_frame = j.at("m_rgflCoordinateFrame").get<long int>();
            x.m_sz_custom_name = j.at("m_szCustomName").get<long int>();
            x.m_sz_last_place_name = j.at("m_szLastPlaceName").get<long int>();
            x.m_third_person_view_angles = j.at("m_thirdPersonViewAngles").get<long int>();
            x.m_vec_origin = j.at("m_vecOrigin").get<long int>();
            x.m_vec_velocity = j.at("m_vecVelocity").get<long int>();
            x.m_vec_view_offset = j.at("m_vecViewOffset").get<long int>();
            x.m_view_punch_angle = j.at("m_viewPunchAngle").get<long int>();
        }

        inline void to_json(json &j, const OffsettParser::Netvars &x) {
            j = json::object();
            j["cs_gamerules_data"] = x.cs_gamerules_data;
            j["m_ArmorValue"] = x.m_armor_value;
            j["m_Collision"] = x.m_collision;
            j["m_CollisionGroup"] = x.m_collision_group;
            j["m_Local"] = x.m_local;
            j["m_MoveType"] = x.m_move_type;
            j["m_OriginalOwnerXuidHigh"] = x.m_original_owner_xuid_high;
            j["m_OriginalOwnerXuidLow"] = x.m_original_owner_xuid_low;
            j["m_SurvivalGameRuleDecisionTypes"] = x.m_survival_game_rule_decision_types;
            j["m_SurvivalRules"] = x.m_survival_rules;
            j["m_aimPunchAngle"] = x.m_aim_punch_angle;
            j["m_aimPunchAngleVel"] = x.m_aim_punch_angle_vel;
            j["m_angEyeAnglesX"] = x.m_ang_eye_angles_x;
            j["m_angEyeAnglesY"] = x.m_ang_eye_angles_y;
            j["m_bBombPlanted"] = x.m_b_bomb_planted;
            j["m_bFreezePeriod"] = x.m_b_freeze_period;
            j["m_bGunGameImmunity"] = x.m_b_gun_game_immunity;
            j["m_bHasDefuser"] = x.m_b_has_defuser;
            j["m_bHasHelmet"] = x.m_b_has_helmet;
            j["m_bInReload"] = x.m_b_in_reload;
            j["m_bIsDefusing"] = x.m_b_is_defusing;
            j["m_bIsQueuedMatchmaking"] = x.m_b_is_queued_matchmaking;
            j["m_bIsScoped"] = x.m_b_is_scoped;
            j["m_bIsValveDS"] = x.m_b_is_valve_ds;
            j["m_bSpotted"] = x.m_b_spotted;
            j["m_bSpottedByMask"] = x.m_b_spotted_by_mask;
            j["m_bStartedArming"] = x.m_b_started_arming;
            j["m_clrRender"] = x.m_clr_render;
            j["m_dwBoneMatrix"] = x.m_dw_bone_matrix;
            j["m_fAccuracyPenalty"] = x.m_f_accuracy_penalty;
            j["m_fFlags"] = x.m_f_flags;
            j["m_flC4Blow"] = x.m_fl_c4_blow;
            j["m_flDefuseCountDown"] = x.m_fl_defuse_count_down;
            j["m_flDefuseLength"] = x.m_fl_defuse_length;
            j["m_flFallbackWear"] = x.m_fl_fallback_wear;
            j["m_flFlashDuration"] = x.m_fl_flash_duration;
            j["m_flFlashMaxAlpha"] = x.m_fl_flash_max_alpha;
            j["m_flLastBoneSetupTime"] = x.m_fl_last_bone_setup_time;
            j["m_flLowerBodyYawTarget"] = x.m_fl_lower_body_yaw_target;
            j["m_flNextAttack"] = x.m_fl_next_attack;
            j["m_flNextPrimaryAttack"] = x.m_fl_next_primary_attack;
            j["m_flSimulationTime"] = x.m_fl_simulation_time;
            j["m_flTimerLength"] = x.m_fl_timer_length;
            j["m_hActiveWeapon"] = x.m_h_active_weapon;
            j["m_hMyWeapons"] = x.m_h_my_weapons;
            j["m_hObserverTarget"] = x.m_h_observer_target;
            j["m_hOwner"] = x.m_h_owner;
            j["m_hOwnerEntity"] = x.m_h_owner_entity;
            j["m_iAccountID"] = x.m_i_account_id;
            j["m_iClip1"] = x.m_i_clip1;
            j["m_iCompetitiveRanking"] = x.m_i_competitive_ranking;
            j["m_iCompetitiveWins"] = x.m_i_competitive_wins;
            j["m_iCrosshairId"] = x.m_i_crosshair_id;
            j["m_iEntityQuality"] = x.m_i_entity_quality;
            j["m_iFOVStart"] = x.m_i_fov_start;
            j["m_iGlowIndex"] = x.m_i_glow_index;
            j["m_iHealth"] = x.m_i_health;
            j["m_iItemDefinitionIndex"] = x.m_i_item_definition_index;
            j["m_iItemIDHigh"] = x.m_i_item_id_high;
            j["m_iMostRecentModelBoneCounter"] = x.m_i_most_recent_model_bone_counter;
            j["m_iObserverMode"] = x.m_i_observer_mode;
            j["m_iShotsFired"] = x.m_i_shots_fired;
            j["m_iState"] = x.m_i_state;
            j["m_iTeamNum"] = x.m_i_team_num;
            j["m_lifeState"] = x.m_life_state;
            j["m_nFallbackPaintKit"] = x.m_n_fallback_paint_kit;
            j["m_nFallbackSeed"] = x.m_n_fallback_seed;
            j["m_nFallbackStatTrak"] = x.m_n_fallback_stat_trak;
            j["m_nForceBone"] = x.m_n_force_bone;
            j["m_nTickBase"] = x.m_n_tick_base;
            j["m_rgflCoordinateFrame"] = x.m_rgfl_coordinate_frame;
            j["m_szCustomName"] = x.m_sz_custom_name;
            j["m_szLastPlaceName"] = x.m_sz_last_place_name;
            j["m_thirdPersonViewAngles"] = x.m_third_person_view_angles;
            j["m_vecOrigin"] = x.m_vec_origin;
            j["m_vecVelocity"] = x.m_vec_velocity;
            j["m_vecViewOffset"] = x.m_vec_view_offset;
            j["m_viewPunchAngle"] = x.m_view_punch_angle;
        }

        inline void from_json(const json &j, OffsettParser::Signatures &x) {
            x.clientstate_choked_commands = j.at("clientstate_choked_commands").get<unsigned long>();
            x.clientstate_delta_ticks = j.at("clientstate_delta_ticks").get<unsigned long>();
            x.clientstate_last_outgoing_command = j.at("clientstate_last_outgoing_command").get<unsigned long>();
            x.clientstate_net_channel = j.at("clientstate_net_channel").get<unsigned long>();
            x.convar_name_hash_table = j.at("convar_name_hash_table").get<unsigned long>();
            x.dw_client_state = j.at("dwClientState").get<unsigned long>();
            x.dw_client_state_get_local_player = j.at("dwClientState_GetLocalPlayer").get<unsigned long>();
            x.dw_client_state_is_hltv = j.at("dwClientState_IsHLTV").get<unsigned long>();
            x.dw_client_state_map = j.at("dwClientState_Map").get<unsigned long>();
            x.dw_client_state_map_directory = j.at("dwClientState_MapDirectory").get<unsigned long>();
            x.dw_client_state_max_player = j.at("dwClientState_MaxPlayer").get<unsigned long>();
            x.dw_client_state_player_info = j.at("dwClientState_PlayerInfo").get<unsigned long>();
            x.dw_client_state_state = j.at("dwClientState_State").get<unsigned long>();
            x.dw_client_state_view_angles = j.at("dwClientState_ViewAngles").get<unsigned long>();
            x.dw_entity_list = j.at("dwEntityList").get<unsigned long>();
            x.dw_force_attack = j.at("dwForceAttack").get<unsigned long>();
            x.dw_force_attack2 = j.at("dwForceAttack2").get<unsigned long>();
            x.dw_force_backward = j.at("dwForceBackward").get<unsigned long>();
            x.dw_force_forward = j.at("dwForceForward").get<unsigned long>();
            x.dw_force_jump = j.at("dwForceJump").get<unsigned long>();
            x.dw_force_left = j.at("dwForceLeft").get<unsigned long>();
            x.dw_force_right = j.at("dwForceRight").get<unsigned long>();
            x.dw_game_dir = j.at("dwGameDir").get<unsigned long>();
            x.dw_game_rules_proxy = j.at("dwGameRulesProxy").get<unsigned long>();
            x.dw_get_all_classes = j.at("dwGetAllClasses").get<unsigned long>();
            x.dw_global_vars = j.at("dwGlobalVars").get<unsigned long>();
            x.dw_glow_object_manager = j.at("dwGlowObjectManager").get<unsigned long>();
            x.dw_input = j.at("dwInput").get<unsigned long>();
            x.dw_interface_link_list = j.at("dwInterfaceLinkList").get<unsigned long>();
            x.dw_local_player = j.at("dwLocalPlayer").get<unsigned long>();
            x.dw_mouse_enable = j.at("dwMouseEnable").get<unsigned long>();
            x.dw_mouse_enable_ptr = j.at("dwMouseEnablePtr").get<unsigned long>();
            x.dw_player_resource = j.at("dwPlayerResource").get<unsigned long>();
            x.dw_radar_base = j.at("dwRadarBase").get<unsigned long>();
            x.dw_sensitivity = j.at("dwSensitivity").get<unsigned long>();
            x.dw_sensitivity_ptr = j.at("dwSensitivityPtr").get<unsigned long>();
            x.dw_set_clan_tag = j.at("dwSetClanTag").get<unsigned long>();
            x.dw_view_matrix = j.at("dwViewMatrix").get<unsigned long>();
            x.dw_weapon_table = j.at("dwWeaponTable").get<unsigned long>();
            x.dw_weapon_table_index = j.at("dwWeaponTableIndex").get<unsigned long>();
            x.dw_yaw_ptr = j.at("dwYawPtr").get<unsigned long>();
            x.dw_zoom_sensitivity_ratio_ptr = j.at("dwZoomSensitivityRatioPtr").get<unsigned long>();
            x.dwb_send_packets = j.at("dwbSendPackets").get<unsigned long>();
            x.dwpp_direct3_d_device9 = j.at("dwppDirect3DDevice9").get<unsigned long>();
            x.force_update_spectator_glow = j.at("force_update_spectator_glow").get<unsigned long>();
            x.interface_engine_cvar = j.at("interface_engine_cvar").get<unsigned long>();
            x.is_c4_owner = j.at("is_c4_owner").get<unsigned long>();
            x.m_b_dormant = j.at("m_bDormant").get<unsigned long>();
            x.m_p_studio_hdr = j.at("m_pStudioHdr").get<unsigned long>();
            x.m_pitch_class_ptr = j.at("m_pitchClassPtr").get<unsigned long>();
            x.m_yaw_class_ptr = j.at("m_yawClassPtr").get<unsigned long>();
            x.model_ambient_min = j.at("model_ambient_min").get<unsigned long>();
            x.set_abs_angles = j.at("set_abs_angles").get<unsigned long>();
            x.set_abs_origin = j.at("set_abs_origin").get<unsigned long>();
        }

        inline void to_json(json &j, const OffsettParser::Signatures &x) {
            j = json::object();
            j["clientstate_choked_commands"] = x.clientstate_choked_commands;
            j["clientstate_delta_ticks"] = x.clientstate_delta_ticks;
            j["clientstate_last_outgoing_command"] = x.clientstate_last_outgoing_command;
            j["clientstate_net_channel"] = x.clientstate_net_channel;
            j["convar_name_hash_table"] = x.convar_name_hash_table;
            j["dwClientState"] = x.dw_client_state;
            j["dwClientState_GetLocalPlayer"] = x.dw_client_state_get_local_player;
            j["dwClientState_IsHLTV"] = x.dw_client_state_is_hltv;
            j["dwClientState_Map"] = x.dw_client_state_map;
            j["dwClientState_MapDirectory"] = x.dw_client_state_map_directory;
            j["dwClientState_MaxPlayer"] = x.dw_client_state_max_player;
            j["dwClientState_PlayerInfo"] = x.dw_client_state_player_info;
            j["dwClientState_State"] = x.dw_client_state_state;
            j["dwClientState_ViewAngles"] = x.dw_client_state_view_angles;
            j["dwEntityList"] = x.dw_entity_list;
            j["dwForceAttack"] = x.dw_force_attack;
            j["dwForceAttack2"] = x.dw_force_attack2;
            j["dwForceBackward"] = x.dw_force_backward;
            j["dwForceForward"] = x.dw_force_forward;
            j["dwForceJump"] = x.dw_force_jump;
            j["dwForceLeft"] = x.dw_force_left;
            j["dwForceRight"] = x.dw_force_right;
            j["dwGameDir"] = x.dw_game_dir;
            j["dwGameRulesProxy"] = x.dw_game_rules_proxy;
            j["dwGetAllClasses"] = x.dw_get_all_classes;
            j["dwGlobalVars"] = x.dw_global_vars;
            j["dwGlowObjectManager"] = x.dw_glow_object_manager;
            j["dwInput"] = x.dw_input;
            j["dwInterfaceLinkList"] = x.dw_interface_link_list;
            j["dwLocalPlayer"] = x.dw_local_player;
            j["dwMouseEnable"] = x.dw_mouse_enable;
            j["dwMouseEnablePtr"] = x.dw_mouse_enable_ptr;
            j["dwPlayerResource"] = x.dw_player_resource;
            j["dwRadarBase"] = x.dw_radar_base;
            j["dwSensitivity"] = x.dw_sensitivity;
            j["dwSensitivityPtr"] = x.dw_sensitivity_ptr;
            j["dwSetClanTag"] = x.dw_set_clan_tag;
            j["dwViewMatrix"] = x.dw_view_matrix;
            j["dwWeaponTable"] = x.dw_weapon_table;
            j["dwWeaponTableIndex"] = x.dw_weapon_table_index;
            j["dwYawPtr"] = x.dw_yaw_ptr;
            j["dwZoomSensitivityRatioPtr"] = x.dw_zoom_sensitivity_ratio_ptr;
            j["dwbSendPackets"] = x.dwb_send_packets;
            j["dwppDirect3DDevice9"] = x.dwpp_direct3_d_device9;
            j["force_update_spectator_glow"] = x.force_update_spectator_glow;
            j["interface_engine_cvar"] = x.interface_engine_cvar;
            j["is_c4_owner"] = x.is_c4_owner;
            j["m_bDormant"] = x.m_b_dormant;
            j["m_pStudioHdr"] = x.m_p_studio_hdr;
            j["m_pitchClassPtr"] = x.m_pitch_class_ptr;
            j["m_yawClassPtr"] = x.m_yaw_class_ptr;
            j["model_ambient_min"] = x.model_ambient_min;
            j["set_abs_angles"] = x.set_abs_angles;
            j["set_abs_origin"] = x.set_abs_origin;
        }

        inline void from_json(const json &j, OffsettParser::Offsets &x) {
            x.timestamp = j.at("timestamp").get<unsigned long>();
            x.signatures = j.at("signatures").get<Signatures>();
            x.netvars = j.at("netvars").get<Netvars>();
        }

	inline void to_json(json& j, const OffsettParser::Offsets& x) {
		j = json::object();
		j["timestamp"] = x.timestamp;
		j["signatures"] = x.signatures;
		j["netvars"] = x.netvars;
	}
}
