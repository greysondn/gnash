// LoaderInfo_as.hx:  ActionScript 3 "LoaderInfo" class, for Gnash.
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
// DejaGnu.hx header file for the testing framework support.

#if flash9
import flash.display.DisplayObject;
import flash.display.Loader;
import flash.display.LoaderInfo;
import flash.display.MovieClip;
import flash.display.SWFVersion;
import flash.events.Event;
import flash.events.EventDispatcher;
import flash.events.IOErrorEvent;
import flash.net.URLRequest;
import flash.system.ApplicationDomain;
import flash.utils.ByteArray;
#else
import flash.MovieClip;
#end
import flash.Lib;
import Type;
import Std;

// import our testing API
import DejaGnu;

// Class must be named with the _as suffix, as that's the same name as the file.
class LoaderInfo_as {
    static function main() {
	
		#if flash9
		
				DejaGnu.note("\nIf test does not run properly -- ie: error of some" +
					" sort, then check line212 and make sure there is a" +
					" valid url path for a swf\n");
					
		var loader:Loader = new Loader();
		//var loader = new flash.display.Loader();
		var x1 = loader.contentLoaderInfo;
		loader.contentLoaderInfo.addEventListener(IOErrorEvent.IO_ERROR, function(e) {
			trace(e);
		});

		loader.contentLoaderInfo.addEventListener(Event.COMPLETE, function(e) {

		// Make sure we actually get a valid class        
		if (Std.is(x1, LoaderInfo)) {
 		    DejaGnu.pass("LoaderInfo class exists");
        } else {
                    DejaGnu.fail("LoaderInfo lass doesn't exist");
        }

		// Tests to see if all the properties exist. All these do is test for
		// existance of a property, and don't test the functionality at all. This
		// is primarily useful only to test completeness of the API implementation.
		if (Std.is(x1.actionScriptVersion, Int)) {
	 	    DejaGnu.pass("LoaderInfo::actionScriptVersion property exists");
	 	} else {
	 	    DejaGnu.fail("LoaderInfo::actionScriptVersion property doesn't exist");
	 	}
	 	if (Std.is(x1.applicationDomain, ApplicationDomain)) {
	 	    DejaGnu.pass("LoaderInfo::applicationDomain property exists");
	 	} else {
	 	    DejaGnu.fail("LoaderInfo::applicationDomain property doesn't exist");
	 	}
		// Cannot test
	 	/*if (Std.is(x1.bytes, Array)) {
	 	    DejaGnu.pass("LoaderInfo::bytes property exists");
	 	} else {
	 	    DejaGnu.fail("LoaderInfo::bytes property doesn't exist");
	 	}*/
		if (Std.is(x1.bytesLoaded, Int)) {
		    DejaGnu.pass("LoaderInfo::bytesLoaded property exists");
		} else {
		    DejaGnu.fail("LoaderInfo::bytesLoaded property doesn't exist");
		}
		if (Std.is(x1.bytesTotal, Int)) {
		    DejaGnu.pass("LoaderInfo::bytesTotal property exists");
		} else {
		    DejaGnu.fail("LoaderInfo::bytesTotal property doesn't exist");
		}
		if (Std.is(x1.childAllowsParent, Bool)) {
		    DejaGnu.pass("LoaderInfo::childAllowsParent property exists");
		} else {
		    DejaGnu.fail("LoaderInfo::childAllowsParent property doesn't exist");
		}
		if (Std.is(x1.content, DisplayObject)) {
	 	    DejaGnu.pass("LoaderInfo::content property exists");
	 	} else {
	 	    DejaGnu.fail("LoaderInfo::content property doesn't exist");
	 	}
		if (Type.typeof(x1.contentType)!=ValueType.TNull) {
		    DejaGnu.pass("LoaderInfo::contentType property exists");
		} else {
		    DejaGnu.fail("LoaderInfo::contentType property doesn't exist");
		}
		if (Std.is(x1.frameRate, Int)) {
		    DejaGnu.pass("LoaderInfo::frameRate property exists");
		} else {
		    DejaGnu.fail("LoaderInfo::frameRate property doesn't exist");
		}
		if (Std.is(x1.height, Int)) {
		    DejaGnu.pass("LoaderInfo::height property exists");
		} else {
		    DejaGnu.fail("LoaderInfo::height property doesn't exist");
		}
	 	if (Std.is(x1.loader, Loader)) {
	 	    DejaGnu.pass("LoaderInfo::loader property exists");
	 	} else {
	 	    DejaGnu.fail("LoaderInfo::loader property doesn't exist");
	 	}
		if (Type.typeof(x1.loaderURL)!=ValueType.TNull) {
		    DejaGnu.pass("LoaderInfo::loaderURL property exists");
		} else {
		    DejaGnu.fail("LoaderInfo::loaderURL property doesn't exist");
		}
	 	if (Std.is(x1.parameters, Dynamic)) {
	 	    DejaGnu.pass("LoaderInfo::parameters property exists");
	 	} else {
	 	    DejaGnu.fail("LoaderInfo::parameters property doesn't exist");
	 	}
		if (Std.is(x1.parentAllowsChild, Bool)) {
		    DejaGnu.pass("LoaderInfo::parentAllowsChild property exists");
		} else {
		    DejaGnu.fail("LoaderInfo::parentAllowsChild property doesn't exist");
		}
		if (Std.is(x1.sameDomain, Bool)) {
		    DejaGnu.pass("LoaderInfo::sameDomain property exists");
		} else {
		    DejaGnu.fail("LoaderInfo::sameDomain property doesn't exist");
		}
	 	if (Std.is(x1.sharedEvents, EventDispatcher)) {
	 	    DejaGnu.pass("LoaderInfo::sharedEvents property exists");
	 	} else {
	 	    DejaGnu.fail("LoaderInfo::sharedEvents property doesn't exist");
	 	}
		
		// For older versions (FP<9), the swfVersion returned type int.  However
		// for new versions (FP>=9) it returned the class SWFVersion.  This test
		// to see which version the swf is.  
		if (Type.typeof(x1.swfVersion)==ValueType.TInt) { 
			if (Std.is(x1.swfVersion, Int)) {
			    DejaGnu.pass("LoaderInfo::swfVersion property exists");
			} else {
			    DejaGnu.fail("LoaderInfo::swfVersion property doesn't exist");
			}
		} else {
			if (Std.is(x1.swfVersion, SWFVersion)) {
			    DejaGnu.pass("LoaderInfo::swfVersion property exists");
			} else {
			    DejaGnu.fail("LoaderInfo::swfVersion property doesn't exist");
			}
		}
		if (Type.typeof(x1.url)!=ValueType.TNull) {
		    DejaGnu.pass("LoaderInfo::url property exists");
		} else {
		    DejaGnu.fail("LoaderInfo::url property doesn't exist");
		}
		if (Std.is(x1.width, Int)) {
		    DejaGnu.pass("LoaderInfo::width property exists");
		} else {
		    DejaGnu.fail("LoaderInfo::width property doesn't exist");
		}
		// childSandboxBridge property only available in the AIR runtime. Not tested
	 	/*if (x1.childSandboxBridge == Object) {
	 	    DejaGnu.pass("LoaderInfo::childSandboxBridge property exists");
	 	} else {
	 	    DejaGnu.fail("LoaderInfo::childSandboxBridge property doesn't exist");
	 	}*/
		// parentSandboxBridge property only available in the AIR runtime. Not tested
	 	/*if (x1.parentSandboxBridge == Object) {
	 	    DejaGnu.pass("LoaderInfo::parentSandboxBridge property exists");
	 	} else {
	 	    DejaGnu.fail("LoaderInfo::parentSandboxBridge property doesn't exist");
	 	}*/

		// Tests to see if all the methods exist. All these do is test for
		// existance of a method, and don't test the functionality at all. This
		// is primarily useful only to test completeness of the API implementation.

		// FIXME: method doesn't exist (maybe haXe problem)	
		/*if (Type.typeof(LoaderInfo.getLoaderInfoByDefinition)==TFunction) {
	 	    DejaGnu.pass("LoaderInfo::getLoaderInfoByDefinition() method exists");
	 	} else {
	 	    DejaGnu.fail("LoaderInfo::getLoaderInfoByDefinition() method doesn't exist");
	 	}*/

#else
    DejaGnu.note("LoaderInfo did not exist in versions prior to SWF9");
#end
		DejaGnu.done();
	
	// Specify file where .swf resides -- user can simply replace their username in the
	// appropriate spot below
	loader.load(new URLRequest("/home/user_name/gnash/trunk/testsuite/car_smash.swf"));
	
    }
}

// local Variables:
// mode: C++
// indent-tabs-mode: t
// End:

