/**************************************************************************/
/*  test_performance.h                                                    */
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

#ifndef TEST_PERFORMANCE_H
#define TEST_PERFORMANCE_H

#include "core/os/os.h"
#include "main/performance.h"

#include <sstream>



#include "main/main.h"


#include "core/os/os.h"
#include "tests/test_macros.h"
#include "main/steam_tracker.h"

#if defined(STEAMAPI_ENABLED)

namespace TestSteamTracker {

TEST_CASE("[SteamTracker] Valid Steam Library Path is Found") {
    SteamTracker tracker;
    String path = tracker.get_steam_library_path();

    // If the system has Steam, it should return a valid path.
    // Otherwise, it should return an empty string.
    if (OS::get_singleton()->has_feature("steam")) {
        CHECK(!path.is_empty());  // Ensure path is not empty.
        CHECK(FileAccess::exists(path));  // Ensure file exists.
    } else {
        CHECK(path.is_empty());  // If Steam is not available, path should be empty.
    }
}

TEST_CASE("[SteamTracker] Steam API Initializes Correctly") {
    SteamTracker tracker;

    // Check if Steam API was initialized
    if (OS::get_singleton()->has_feature("steam")) {
        CHECK(tracker.is_initialized());  // Ensure Steam API is initialized.
    } else {
        CHECK(!tracker.is_initialized());  // Ensure Steam API is not initialized.
    }
}

} // namespace TestSteamTracker

#endif // STEAMAPI_ENABLED

#endif // TEST_STEAM_TRACKER_H

