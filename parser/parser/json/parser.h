#pragma once
#include "json/json.h"
#include "../offsets.h"
#include <Windows.h>
#include <WinInet.h>
#include <sstream>
#include <string>
#include <functional>
#pragma comment(lib, "wininet.lib")
Json::Value JSON;
std::string JSONLINK = "https://pastebin.com/raw/rdZqXBBZ"; //json format link here

std::string replaceAll(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

std::string DownloadURL(std::string URL) { /* you could replace this with something like cpr or curl */
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL.c_str(), NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[2000];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			std::string p = replaceAll(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	std::string p = replaceAll(rtn, "|n", "\r\n");
	return p;
}

uintptr_t GetAddress(std::string function) { /* Gets Address */
	return JSON["addresses"][function.c_str()]["addr"].asInt(); /* equivalent to addresses.function.addr in json  */
}

/* 

{
   "addresses":{
      "function":{
         "addr":18325792, //gets this right here 18325792
         "ccv":"__fastcall",
         "retcheck":false,
         "isapartoflua":false,
         "aob":"",
         "args":"int a1, int a2",
         "mask":"",
         "typedef":"int"
      }
   }
}

*/

std::string GetCCV(std::string function) { /* Gets CCV */
	return JSON["addresses"][function.c_str()]["ccv"].asString(); /* equivalent to addresses.function.ccv in json  */
}

/* 

{
   "addresses":{
      "function":{
         "addr":18325792, 
         "ccv":"__fastcall", //gets this right here __fastcall
         "retcheck":false,
         "isapartoflua":false,
         "aob":"",
         "args":"int a1, int a2",
         "mask":"",
         "typedef":"int"
      }
   }
}

*/

int GetType(std::string offset) { /* Gets Type */
	return JSON["offsets"]["types"][offset.c_str()].asInt(); /* equivalent to offsets.types.offset in json  */
}

int GetRbxOffset(std::string offset) { /* Gets Roblox Offset */
	return JSON["offsets"]["roblox_offsets"][offset.c_str()].asInt();
}

int GetLuaOffset(std::string offset) { /* Gets Lua State Offsets */
	return JSON["offsets"]["luastate_offsets"][offset.c_str()].asInt();
}

int GetCallOffset(std::string offset) { /* Gets Callinfo Offset */
	return JSON["offsets"]["callinfo_offsets"][offset.c_str()].asInt();
}

int GetGlobalOffset(std::string offset) { /* Gets Global State Offsets */
	return JSON["offsets"]["globalstate_offsets"][offset.c_str()].asInt();
}

int GetClosureOffset(std::string offset) { /* Gets Closure Offsets */
	return JSON["offsets"]["closure_offsets"][offset.c_str()].asInt();
}

int GetTmsEnums(std::string offset) { /* Gets tms enums */
	return JSON["offsets"]["tms_enums"][offset.c_str()].asInt();
}

int GetProtoo(std::string offset) { /* Gets protos */
	return JSON["offsets"]["protos"][offset.c_str()].asInt();
}

std::string GetExtra(std::string offset) { /* Gets extras */
	return JSON["offsets"]["extras"][offset.c_str()].asString();
}

bool NeedRetcheck(std::string function) {
	return JSON["addresses"][function.c_str()]["retcheck"].asBool();
}

bool IsApartOfLua(std::string function) { /* gets isapartoflua */
	return JSON["addresses"][function.c_str()]["isapartoflua"].asBool();  /* equivalent to addresses.function.isapartoflua in json  */
}

/* 

{
   "addresses":{
      "function":{
         "addr":18325792, 
         "ccv":"__fastcall", 
         "retcheck":false,
         "isapartoflua":false, //gets this right here false
         "aob":"",
         "args":"int a1, int a2",
         "mask":"",
         "typedef":"int"
      }
   }
}

*/

std::string GetArgs(std::string function) { /* gets args */
	return JSON["addresses"][function.c_str()]["args"].asString();  /* equivalent to addresses.function.args in json  */
}


/* 

{
   "addresses":{
      "function":{
         "addr":18325792, 
         "ccv":"__fastcall", 
         "retcheck":false,
         "isapartoflua":false,
         "aob":"",
         "args":"int a1, int a2", //gets this right here int a1, int a2
         "mask":"",
         "typedef":"int"
      }
   }
}

*/


void FetchJson() { /* gets json file from link */
	std::unique_ptr<std::string> data(new std::string(DownloadURL(JSONLINK)));
	const auto rawlength = static_cast<int>((*data).length());
	JSONCPP_STRING error;
	Json::CharReaderBuilder builder;
	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	if (!reader->parse((*data).c_str(), (*data).c_str() + rawlength, &JSON, &error))
	{
		FetchJson();
	}
}

void Init() {  /* gets everything ready */
	FetchJson();
	/* Adresses And CCVs */
	getdatamodel1_addr = GetAddress("getdatamodel1");
	getdatamodel1_CCV = GetCCV("getdatamodel1");
	lua_close_addr = GetAddress("lua_close");
	lua_close_CCV = GetCCV("lua_close");
	resize_addr = GetAddress("resize");
	resize_CCV = GetCCV("resize");
	luah_resizearray_addr = GetAddress("luah_resizearray");
	luah_resizearray_CCV = GetCCV("luah_resizearray");
	lua_getinfo_addr = GetAddress("lua_getinfo");
	lua_getinfo_CCV = GetCCV("lua_getinfo");
	fireclickdetector_addr = GetAddress("fireclickdetector");
	fireclickdetector_CCV = GetCCV("fireclickdetector");
	rbx_setsimulationradius_addr = GetAddress("rbx_setsimulationradius");
	rbx_setsimulationradius_CCV = GetCCV("rbx_setsimulationradius");
	rbx_get_property_addr = GetAddress("rbx_get_property");
	rbx_get_property_CCV = GetCCV("rbx_get_property");
	appendarray_addr = GetAddress("appendarray");
	appendarray_CCV = GetCCV("appendarray");
	auxresume_addr = GetAddress("auxresume");
	auxresume_CCV = GetCCV("auxresume");
	retcheck_addr = GetAddress("retcheck");
	retcheck_CCV = GetCCV("retcheck");
	lual_typeerror_addr = GetAddress("lual_typeerror");
	lual_typeerror_CCV = GetCCV("lual_typeerror");
	lual_argerror_addr = GetAddress("lual_argerror");
	lual_argerror_CCV = GetCCV("lual_argerror");
	lua_gc_addr = GetAddress("lua_gc");
	lua_gc_CCV = GetCCV("lua_gc");
	luab_rawget_addr = GetAddress("luab_rawget");
	luab_rawget_CCV = GetCCV("luab_rawget");
	luab_rawset_addr = GetAddress("luab_rawset");
	luab_rawset_CCV = GetCCV("luab_rawset");
	luaf_newlclosure_addr = GetAddress("luaf_newlclosure");
	luaf_newlclosure_CCV = GetCCV("luaf_newlclosure");
	luad_reallocstack_addr = GetAddress("luad_reallocstack");
	luad_reallocstack_CCV = GetCCV("luad_reallocstack");
	addinfo_addr = GetAddress("addinfo");
	addinfo_CCV = GetCCV("addinfo");
	luac_step_addr = GetAddress("luac_step");
	luac_step_CCV = GetCCV("luac_step");
	luah_getn_addr = GetAddress("luah_getn");
	luah_getn_CCV = GetCCV("luah_getn");
	rehash_addr = GetAddress("rehash");
	rehash_CCV = GetCCV("rehash");
	luah_getstr_addr = GetAddress("luah_getstr");
	luah_getstr_CCV = GetCCV("luah_getstr");
	get_lua_state_addr = GetAddress("get_lua_state");
	get_lua_state_CCV = GetCCV("get_lua_state");
	lua_newstate_addr = GetAddress("lua_newstate");
	lua_newstate_CCV = GetCCV("lua_newstate");
	close_state_addr = GetAddress("close_state");
	close_state_CCV = GetCCV("close_state");
	luav_tostring_addr = GetAddress("luav_tostring");
	luav_tostring_CCV = GetCCV("luav_tostring");
	luav_dolen_addr = GetAddress("luav_dolen");
	luav_dolen_CCV = GetCCV("luav_dolen");
	calltmres_addr = GetAddress("calltmres");
	calltmres_CCV = GetCCV("calltmres");
	luav_doairth_addr = GetAddress("luav_doairth");
	luav_doairth_CCV = GetCCV("luav_doairth");
	luag_indexerror_addr = GetAddress("luag_indexerror");
	luag_indexerror_CCV = GetCCV("luag_indexerror");
	call_ordertm_addr = GetAddress("call_ordertm");
	call_ordertm_CCV = GetCCV("call_ordertm");
	luav_lessequal_addr = GetAddress("luav_lessequal");
	luav_lessequal_CCV = GetCCV("luav_lessequal");
	luav_lessthan_addr = GetAddress("luav_lessthan");
	luav_lessthan_CCV = GetCCV("luav_lessthan");
	luat_objtypename_addr = GetAddress("luat_objtypename");
	luat_objtypename_CCV = GetCCV("luat_objtypename");
	luag_ordererror_addr = GetAddress("luag_ordererror");
	luag_ordererror_CCV = GetCCV("luag_ordererror");
	luag_aritherror_addr = GetAddress("luag_aritherror");
	luag_aritherror_CCV = GetCCV("luag_aritherror");
	luab_auxwrap_addr = GetAddress("luab_auxwrap");
	luab_auxwrap_CCV = GetCCV("luab_auxwrap");
	lual_where_addr = GetAddress("lual_where");
	lual_where_CCV = GetCCV("lual_where");
	lua_pushvfstring_addr = GetAddress("lua_pushvfstring");
	lua_pushvfstring_CCV = GetCCV("lua_pushvfstring");
	lua_concat_addr = GetAddress("lua_concat");
	lua_concat_CCV = GetCCV("lua_concat");
	lua_error_addr = GetAddress("lua_error");
	lua_error_CCV = GetCCV("lua_error");
	markroot_addr = GetAddress("markroot");
	markroot_CCV = GetCCV("markroot");
	lua_typename_addr = GetAddress("lua_typename");
	lua_typename_CCV = GetCCV("lua_typename");
	ipairsaux_addr = GetAddress("ipairsaux");
	ipairsaux_CCV = GetCCV("ipairsaux");
	luab_ipairs_addr = GetAddress("luab_ipairs");
	luab_ipairs_CCV = GetCCV("luab_ipairs");
	luab_next_addr = GetAddress("luab_next");
	luab_next_CCV = GetCCV("luab_next");
	luab_pair_addr = GetAddress("luab_pair");
	luab_pair_CCV = GetCCV("luab_pair");
	luaopen_table_addr = GetAddress("luaopen_table");
	luaopen_table_CCV = GetCCV("luaopen_table");
	luaopen_bit32_addr = GetAddress("luaopen_bit32");
	luaopen_bit32_CCV = GetCCV("luaopen_bit32");
	luaopen_base_addr = GetAddress("luaopen_base");
	luaopen_base_CCV = GetCCV("luaopen_base");
	luad_rawrunprotected_addr = GetAddress("luad_rawrunprotected");
	luad_rawrunprotected_CCV = GetCCV("luad_rawrunprotected");
	getnamecallmethod_addr = GetAddress("getnamecallmethod");
	getnamecallmethod_CCV = GetCCV("getnamecallmethod");
	resume_error_addr = GetAddress("resume_error");
	resume_error_CCV = GetCCV("resume_error");
	reallymarkobject_addr = GetAddress("reallymarkobject");
	reallymarkobject_CCV = GetCCV("reallymarkobject");
	propagateall_addr = GetAddress("propagateall");
	propagateall_CCV = GetCCV("propagateall");
	singlestep_addr = GetAddress("singlestep");
	singlestep_CCV = GetCCV("singlestep");
	markmt_addr = GetAddress("markmt");
	markmt_CCV = GetCCV("markmt");
	f_luaopen_addr = GetAddress("f_luaopen");
	f_luaopen_CCV = GetCCV("f_luaopen");
	luac_fullgc_addr = GetAddress("luac_fullgc");
	luac_fullgc_CCV = GetCCV("luac_fullgc");
	resume_addr = GetAddress("resume");
	resume_CCV = GetCCV("resume");
	lua_resume_addr = GetAddress("lua_resume");
	lua_resume_CCV = GetCCV("lua_resume");
	luau_deserialize_addr = GetAddress("luau_deserialize");
	luau_deserialize_CCV = GetCCV("luau_deserialize");
	luad_seterrorobj_addr = GetAddress("luad_seterrorobj");
	luad_seterrorobj_CCV = GetCCV("luad_seterrorobj");
	luav_execute_addr = GetAddress("luav_execute");
	luav_execute_CCV = GetCCV("luav_execute");
	luau_betavm_addr = GetAddress("luau_betavm");
	luau_betavm_CCV = GetCCV("luau_betavm");
	growci_addr = GetAddress("growci");
	growci_CCV = GetCCV("growci");
	sweeplist_addr = GetAddress("sweeplist");
	sweeplist_CCV = GetCCV("sweeplist");
	luam_realloc__addr = GetAddress("luam_realloc_");
	luam_realloc__CCV = GetCCV("luam_realloc_");
	luad_reallocci_addr = GetAddress("luad_reallocci");
	luad_reallocci_CCV = GetCCV("luad_reallocci");
	luas_resize_addr = GetAddress("luas_resize");
	luas_resize_CCV = GetCCV("luas_resize");
	lual_register_addr = GetAddress("lual_register");
	lual_register_CCV = GetCCV("lual_register");
	lual_findtable_addr = GetAddress("lual_findtable");
	lual_findtable_CCV = GetCCV("lual_findtable");
	print_addr = GetAddress("print");
	print_CCV = GetCCV("print");
	index2adr_addr = GetAddress("index2adr");
	index2adr_CCV = GetCCV("index2adr");
	luad_throw_addr = GetAddress("luad_throw");
	luad_throw_CCV = GetCCV("luad_throw");
	luag_runerror_addr = GetAddress("luag_runerror");
	luag_runerror_CCV = GetCCV("luag_runerror");
	luam_toobig_addr = GetAddress("luam_toobig");
	luam_toobig_CCV = GetCCV("luam_toobig");
	rbx_network_replicator_onreceive_addr = GetAddress("rbx_network_replicator_onreceive");
	rbx_network_replicator_onreceive_CCV = GetCCV("rbx_network_replicator_onreceive");
	rbx_network_physicssenders2_sendpacket_addr = GetAddress("rbx_network_physicssenders2_sendpacket");
	rbx_network_physicssenders2_sendpacket_CCV = GetCCV("rbx_network_physicssenders2_sendpacket");
	report_touch_info_addr = GetAddress("report_touch_info");
	report_touch_info_CCV = GetCCV("report_touch_info");
	fireproximityprompt_addr = GetAddress("fireproximityprompt");
	fireproximityprompt_CCV = GetCCV("fireproximityprompt");
	raknet_rakpeer_runupdatecycle_addr = GetAddress("raknet_rakpeer_runupdatecycle");
	raknet_rakpeer_runupdatecycle_CCV = GetCCV("raknet_rakpeer_runupdatecycle");
	rbx_taskscheduler_getlastcycletimestamp_addr = GetAddress("rbx_taskscheduler_getlastcycletimestamp");
	rbx_taskscheduler_getlastcycletimestamp_CCV = GetCCV("rbx_taskscheduler_getlastcycletimestamp");
	taskscheduler_addr = GetAddress("taskscheduler");
	taskscheduler_CCV = GetCCV("taskscheduler");
	rbx_security_impersonator_impersonator_addr = GetAddress("rbx_security_impersonator_impersonator");
	rbx_security_impersonator_impersonator_CCV = GetCCV("rbx_security_impersonator_impersonator");
	rbx_sc_closestate_addr = GetAddress("rbx_sc_closestate");
	rbx_sc_closestate_CCV = GetCCV("rbx_sc_closestate");
	rbx_sc_crash_addr = GetAddress("rbx_sc_crash");
	rbx_sc_crash_CCV = GetCCV("rbx_sc_crash");
	rbx_lua_dumpthreadrefcounts_addr = GetAddress("rbx_lua_dumpthreadrefcounts");
	rbx_lua_dumpthreadrefcounts_CCV = GetCCV("rbx_lua_dumpthreadrefcounts");
	rbx_network_server_registerlegalscript_addr = GetAddress("rbx_network_server_registerlegalscript");
	rbx_network_server_registerlegalscript_CCV = GetCCV("rbx_network_server_registerlegalscript");
	rbx_sc_interrupt_addr = GetAddress("rbx_sc_interrupt");
	rbx_sc_interrupt_CCV = GetCCV("rbx_sc_interrupt");
	rbx_sc_loadstring_addr = GetAddress("rbx_sc_loadstring");
	rbx_sc_loadstring_CCV = GetCCV("rbx_sc_loadstring");
	rbx_sc_scriptcontext_addr = GetAddress("rbx_sc_scriptcontext");
	rbx_sc_scriptcontext_CCV = GetCCV("rbx_sc_scriptcontext");
	rbx_datamodel_dodatamodelsetup_addr = GetAddress("rbx_datamodel_dodatamodelsetup");
	rbx_datamodel_dodatamodelsetup_CCV = GetCCV("rbx_datamodel_dodatamodelsetup");
	rbx_standardout_printwithlink_addr = GetAddress("rbx_standardout_printwithlink");
	rbx_standardout_printwithlink_CCV = GetCCV("rbx_standardout_printwithlink");
	robloxextraspace__robloxextraspace_addr = GetAddress("robloxextraspace__robloxextraspace");
	robloxextraspace__robloxextraspace_CCV = GetCCV("robloxextraspace__robloxextraspace");
	rbx_ws_getrealphysicsfps_addr = GetAddress("rbx_ws_getrealphysicsfps");
	rbx_ws_getrealphysicsfps_CCV = GetCCV("rbx_ws_getrealphysicsfps");
	sandboxthread_addr = GetAddress("sandboxthread");
	sandboxthread_CCV = GetCCV("sandboxthread");
	luav_concat_addr = GetAddress("luav_concat");
	luav_concat_CCV = GetCCV("luav_concat");
	rbxcrash_addr = GetAddress("rbxcrash");
	rbxcrash_CCV = GetCCV("rbxcrash");
	spawn_addr = GetAddress("spawn");
	spawn_CCV = GetCCV("spawn");
	rbx_sc_watchdog_addr = GetAddress("rbx_sc_watchdog");
	rbx_sc_watchdog_CCV = GetCCV("rbx_sc_watchdog");
	rbx_scriptcontext_getcontext_addr = GetAddress("rbx_scriptcontext_getcontext");
	rbx_scriptcontext_getcontext_CCV = GetCCV("rbx_scriptcontext_getcontext");
	rbx_scriptcontext_debuggermanager_addr = GetAddress("rbx_scriptcontext_debuggermanager");
	rbx_scriptcontext_debuggermanager_CCV = GetCCV("rbx_scriptcontext_debuggermanager");
	rbx_scriptcontext_pluginmanager_addr = GetAddress("rbx_scriptcontext_pluginmanager");
	rbx_scriptcontext_pluginmanager_CCV = GetCCV("rbx_scriptcontext_pluginmanager");
	rbx_scriptcontext_settings_addr = GetAddress("rbx_scriptcontext_settings");
	rbx_scriptcontext_settings_CCV = GetCCV("rbx_scriptcontext_settings");
	rbx_scriptcontext_getglobalstate_addr = GetAddress("rbx_scriptcontext_getglobalstate");
	rbx_scriptcontext_getglobalstate_CCV = GetCCV("rbx_scriptcontext_getglobalstate");
	push_captures_addr = GetAddress("push_captures");
	push_captures_CCV = GetCCV("push_captures");
	flog_getvalue_addr = GetAddress("flog_getvalue");
	flog_getvalue_CCV = GetCCV("flog_getvalue");
	flog_setvalue_addr = GetAddress("flog_setvalue");
	flog_setvalue_CCV = GetCCV("flog_setvalue");
	findfirstchild_addr = GetAddress("findfirstchild");
	findfirstchild_CCV = GetCCV("findfirstchild");
	findfirstchildofclass_addr = GetAddress("findfirstchildofclass");
	findfirstchildofclass_CCV = GetCCV("findfirstchildofclass");
	xorconst = GetAddress("xorconst");
	getdatamodel2 = GetAddress("getdatamodel2");
	callcheck = GetAddress("callcheck");
	luao_nilobject = GetAddress("luao_nilobject");
	luat_typenames = GetAddress("luat_typenames");
	scriptinstance = GetAddress("scriptinstance");
	scriptcontext = GetAddress("scriptcontext");
	datamodel = GetAddress("datamodel");
	kname = GetAddress("kname");

	/* Roblox Offsets */
	getnamecallmethod_offset = GetRbxOffset("getnamecallmethod_offset");
	networked_dm = GetRbxOffset("networked_dm");
	Identity1 = GetRbxOffset("Identity1");
	Identity2 = GetRbxOffset("Identity2");

	/* Lua Offsets */
	lua_state_env = GetLuaOffset("lua_state_env");
	lua_state_l_gt = GetLuaOffset("lua_state_l_gt");
	lua_state_base = GetLuaOffset("lua_state_base");
	lua_state_top = GetLuaOffset("lua_state_top");
	lua_state_nccalls = GetLuaOffset("lua_state_nccalls");
	lua_state_baseccalls = GetLuaOffset("lua_state_baseccalls");
	lua_state_status = GetLuaOffset("lua_state_status");
	lua_stack_last = GetLuaOffset("lua_stack_last");
	lua_state_ci = GetLuaOffset("lua_state_ci");
	lua_state_stack = GetLuaOffset("lua_state_stack");
	lua_state_openupval = GetLuaOffset("lua_state_openupval");
	lua_state_baseci = GetLuaOffset("lua_state_baseci");
	lua_state_sizeci = GetLuaOffset("lua_state_sizeci");
	lua_state_savedpc = GetLuaOffset("lua_state_savedpc");

	/* Callinfo Offsets */
	callinfo_func = GetCallOffset("callinfo_func");

	/* Globalstate Offsets */
	global_state_frealloc = GetGlobalOffset("global_state_frealloc");
	global_state_ud = GetGlobalOffset("global_state_ud");
	global_state_currentwhite = GetGlobalOffset("global_state_currentwhite");
	global_state_gcstate = GetGlobalOffset("global_state_gcstate");
	global_state_gcstepmul = GetGlobalOffset("global_state_gcstepmul");
	global_state_gray = GetGlobalOffset("global_state_gray");
	global_state_gcthreshold = GetGlobalOffset("global_state_gcthreshold");
	global_state_totalbytes = GetGlobalOffset("global_state_totalbytes");
	global_state_grayagain = GetGlobalOffset("global_state_grayagain");
	global_state_mainthread = GetGlobalOffset("global_state_mainthread");
	global_state_rootgc = GetGlobalOffset("global_state_rootgc");
	global_state_sweepstrgc = GetGlobalOffset("global_state_sweepstrgc");
	global_state_weak = GetGlobalOffset("global_state_weak");

	/* Closure Offsets */
	closure_env = GetClosureOffset("closure_env");
	closure_isc = GetClosureOffset("closure_isc");
	closure_nupval = GetClosureOffset("closure_nupval");
	closure_nups = GetClosureOffset("closure_nups");

	/* Types */
	R_LUA_TNONE = GetType("R_LUA_TNONE");
	R_LUA_TUSERDATA = GetType("R_LUA_TUSERDATA");
	R_LUA_TFUNCTION = GetType("R_LUA_TFUNCTION");
	R_LUA_TSTRING = GetType("R_LUA_TSTRING");
	R_LUA_TBOOLEAN = GetType("R_LUA_TBOOLEAN");
	R_LUA_TNUMBER = GetType("R_LUA_TNUMBER");
	R_LUA_TTABLE = GetType("R_LUA_TTABLE");
	R_LUA_TNIL = GetType("R_LUA_TNIL");
	R_LUA_TTHREAD = GetType("R_LUA_TTHREAD");
	R_LUA_TVECTOR = GetType("R_LUA_TVECTOR");
	R_LUA_TLIGHTUSERDATA = GetType("R_LUA_TLIGHTUSERDATA");
	R_LUA_TPROTO = GetType("R_LUA_TPROTO");
	R_LUA_TUPVALUE = GetType("R_LUA_TUPVALUE");

	/* Tms Enums */
	tm_index = GetTmsEnums("tm_index");
	tm_newindex = GetTmsEnums("tm_newindex");
	tm_namecall = GetTmsEnums("tm_namecall");
	tm_mode = GetTmsEnums("tm_mode");
	tm_add = GetTmsEnums("tm_add");
	tm_sub = GetTmsEnums("tm_sub");
	tm_mul = GetTmsEnums("tm_mul");
	tm_div = GetTmsEnums("tm_div");
	tm_mod = GetTmsEnums("tm_mod");
	tm_pow = GetTmsEnums("tm_pow");
	tm_unm = GetTmsEnums("tm_unm");
	tm_eq = GetTmsEnums("tm_eq");

	/* Lua TValue Offsets (These Are Always The Same :scream:) */
	lua_tvalue_tt = 12;
	lua_tvalue_size = 16;

	/* Protos */
	proto_k = GetProtoo("proto_k");
	proto_code = GetProtoo("proto_code");
	proto_p = GetProtoo("proto_p");
	proto_lineinfo = GetProtoo("proto_lineinfo");
	proto_locvars = GetProtoo("proto_locvars");
	proto_upvalues = GetProtoo("proto_upvalues");
	proto_source = GetProtoo("proto_source");
	proto_sizeupvalues = GetProtoo("proto_sizeupvalues");
	proto_sizek = GetProtoo("proto_sizek");
	proto_sizecode = GetProtoo("proto_sizecode");
	proto_sizelineinfo = GetProtoo("proto_sizelineinfo");
	proto_lineinfo_opt = GetProtoo("proto_lineinfo_opt");
	proto_nups = GetProtoo("proto_nups");
	proto_numparams = GetProtoo("proto_numparams");
	proto_is_vararg = GetProtoo("proto_is_vararg");
	proto_maxstacksize = GetProtoo("proto_maxstacksize");

	/* Extras */
	gs_symbol = GetExtra("gs_symbol");
	gs_offsets = GetExtra("gs_offset");
	ls_symbol = GetExtra("ls_symbol");
	ls_offsets = GetExtra("ls_offset");
	luastate_offset = std::stoi(ls_offsets); // fuck you mellon
	globalstate_offset = std::stoi(gs_offsets); // fuck you mellon
}

int convertToASCII(std::string s)  /* turns string to ascii */
{
	for (int i = 0; i < s.length(); i++)
	{
		return (int)s[i];
	}
}

std::uintptr_t RBX_GlobalState(uintptr_t i)  /* hacky rbx_globalstate because mellon is a faggot */
{
	switch (convertToASCII(gs_symbol)) {
	case 43: return *reinterpret_cast<const std::uintptr_t*>(i + globalstate_offset) + (i + globalstate_offset);
	case 45: return *reinterpret_cast<const std::uintptr_t*>(i + globalstate_offset) - (i + globalstate_offset);
	case 94: return *reinterpret_cast<const std::uintptr_t*>(i + globalstate_offset) ^ (i + globalstate_offset);
	default: std::cout << "unexpected symbol!" << std::endl; return 0;
	}
}

std::uintptr_t RBX_LuaState(uintptr_t i) /* hacky rbx_luastate because mellon is a faggot */
{
	switch (convertToASCII(gs_symbol)) {
	case 43: return *reinterpret_cast<const std::uintptr_t*>(i + luastate_offset) + (i + luastate_offset);
	case 45: return *reinterpret_cast<const std::uintptr_t*>(i + luastate_offset) - (i + luastate_offset);
	case 94: return *reinterpret_cast<const std::uintptr_t*>(i + luastate_offset) ^ (i + luastate_offset);
	default: std::cout << "unexpected symbol!" << std::endl; return 0;
	}
}


/*

to actually call functions you need to make a function where u have to pass the address, ccv and args
so u have to check if ccv is __cdecl, __fastcall and so on if you dont get what i mean add me on discord and ask me it on there itsuki#6969

*/
