/**************************************************************************/
/*  steam_tracker.cpp                                                     */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#if defined(STEAMAPI_ENABLED)

#include "steam_tracker.h"

// https://partner.steamgames.com/doc/sdk/api#initialization_and_shutdown

SteamTracker::SteamTracker() {
	String path = get_steam_library_path();
	if (path.is_empty()) {
		return;
	}

	if (!load_steam_library(path)) {
		return;
	}

	initialize_steam_api();
}

String SteamTracker::get_steam_library_path() {
	String path;
	String base_dir = OS::get_singleton()->get_executable_path().get_base_dir();

	if (OS::get_singleton()->has_feature("linuxbsd")) {
		path = find_existing_path({ base_dir.path_join("libsteam_api.so"),
			base_dir.path_join("../lib/libsteam_api.so") });
	} else if (OS::get_singleton()->has_feature("windows")) {
		path = find_existing_path({ base_dir.path_join(OS::get_singleton()->has_feature("64") ? "steam_api64.dll" : "steam_api.dll") });
	} else if (OS::get_singleton()->has_feature("macos")) {
		path = find_existing_path({ base_dir.path_join("libsteam_api.dylib"),
			base_dir.path_join("../Frameworks/libsteam_api.dylib") });
	}

	return path;
}

String SteamTracker::find_existing_path(const Vector<String> &paths) {
	for (const String &p : paths) {
		if (FileAccess::exists(p)) {
			return p;
		}
	}
	return String(); // Return empty if no valid path found
}

bool SteamTracker::load_steam_library(const String &path) {
	Error err = OS::get_singleton()->open_dynamic_library(path, steam_library_handle);
	if (err != OK) {
		steam_library_handle = nullptr;
		return false;
	}
	print_verbose("Loaded SteamAPI library");
	return true;
}

void SteamTracker::initialize_steam_api() {
	void *symbol_handle = nullptr;

	if (OS::get_singleton()->get_dynamic_library_symbol_handle(steam_library_handle, "SteamAPI_InitFlat", symbol_handle, true) == OK) {
		steam_init_flat_function = (SteamAPI_InitFlatFunction)symbol_handle;
	} else if (OS::get_singleton()->get_dynamic_library_symbol_handle(steam_library_handle, "SteamAPI_Init", symbol_handle) == OK) {
		steam_init_function = (SteamAPI_InitFunction)symbol_handle;
	}

	if (OS::get_singleton()->get_dynamic_library_symbol_handle(steam_library_handle, "SteamAPI_Shutdown", symbol_handle) == OK) {
		steam_shutdown_function = (SteamAPI_ShutdownFunction)symbol_handle;
	}

	if (steam_init_flat_function) {
		char err_msg[1024] = {};
		steam_initialized = (steam_init_flat_function(&err_msg[0]) == SteamAPIInitResult_OK);
	} else if (steam_init_function) {
		steam_initialized = steam_init_function();
	}
}


#endif // STEAMAPI_ENABLED
