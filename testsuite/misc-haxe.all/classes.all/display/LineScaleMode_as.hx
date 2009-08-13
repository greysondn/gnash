// LineScaleMode_as.hx:  ActionScript 3 "LineScaleMode" class, for Gnash.
//
// Generated by gen-as3.sh on: 20090514 by "rob". Remove this
// after any hand editing loosing changes.
//
//   Copyright (C) 2009 Free Software Foundation, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

// This test case must be processed by CPP before compiling to include the
//  DejaGnu.hx header file for the testing framework support.

#if flash9
import flash.display.LineScaleMode;
import flash.display.MovieClip;
#else
import flash.MovieClip;
#end
import flash.Lib;
import Type;

// import our testing API
import DejaGnu;

// Class must be named with the _as suffix, as that's the same name as the file.
class LineScaleMode_as {
    static function main() {

#if flash9
   	// Make sure we actually get a valid class        
	if (Type.typeof(LineScaleMode)==TObject) {
	    DejaGnu.pass("LineScaleMode class exists.");
	} else {
	    DejaGnu.fail("LineScaleMode class doesn't exist");
	}

	// Tests to see if all the constants exist. All these do is test for
	// existance of a constants, and don't test the functionality at all. 
        if (Type.typeof(LineScaleMode.HORIZONTAL)!= TNull) {
            DejaGnu.pass("LineScaleMode.HORIZONTAL constant exists");
        } else {
            DejaGnu.fail("LineScaleMode.HORIZONTAL constant doesn't exist");
        }
        if (Type.typeof(LineScaleMode.NONE) != TNull) {
            DejaGnu.pass("LineScaleMode.NONE constant exists");
        } else {
            DejaGnu.fail("LineScaleMode.NONE constant doesn't exist");
        }
        if (Type.typeof(LineScaleMode.NORMAL) != TNull) {
            DejaGnu.pass("LineScaleMode.NORMAL constant exists");
        } else {
            DejaGnu.fail("LineScaleMode.NORMAL constant doesn't exist");
        }
        if (Type.typeof(LineScaleMode.VERTICAL) != TNull) {
            DejaGnu.pass("LineScaleMode.VERTICAL constant exists");
        } else {
            DejaGnu.fail("LineScaleMode.VERTICAL constant doesn't exist");
        }
#else
        DejaGnu.note("LineScaleMode did not exist in versions prior to SWF9");
#end

        // Call this after finishing all tests. It prints out the totals.
        DejaGnu.done();

    }
}

// local Variables:
// mode: C++
// indent-tabs-mode: t
// End:

