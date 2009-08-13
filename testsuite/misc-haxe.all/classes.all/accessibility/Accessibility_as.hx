// Accessibility_as.hx:  ActionScript 3 "Accessibility" class, for Gnash.
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

// These tests were in the ming test files, but have not yet been included
// in the haxe cases
// Possible internal tests that need to be written
// check_equals (typeof(Accessibility.__proto__), 'object');
// check_equals (Accessibility.__proto__, Object.prototype);
// sendEvent may be a function that is undocumented in the Adobe specs
// check_equals ( typeof(Accessibility.sendEvent), 'function' );
// check(Accessibility.hasOwnProperty("sendEvent"));

#if flash9
import flash.accessibility.Accessibility;
import flash.display.MovieClip;
#else
import flash.Accessibility;
import flash.MovieClip;
#end
import flash.Lib;
import Type;
import Reflect;
import haxe.PosInfos;

// import our testing API
import DejaGnu;

// Class must be named with the _as suffix, as that's the same name as the file.
class Accessibility_as {
    static function main() {

// Tests to see if all the properties exist. All these do is test for
// existance of a property, and don't test the functionality at all. This
// is primarily useful only to test completeness of the API implementation.
#if flash9
// Should be:
// 	    TBool
// 	    null
// 	    false
// 	    false
// 	    flash.accessibility.Accessibility
// 	    sendEvent
	
	if (Type.typeof(Accessibility.active) == ValueType.TBool) {
	    DejaGnu.pass("[ln:"+here.lineNumber+"]Accessibility::active property exists");
	} else {
	    DejaGnu.fail("[ln:"+here.lineNumber+"]Accessibility::active property doesn't exist");
	}

	// Test for existence of updateProperties method
	// updateProperties throws error if Capabilities.hasAccessability == false
	if ( flash.system.Capabilities.hasAccessibility != false ) {
		if (Accessibility.updateProperties() != null) {
	    	    DejaGnu.pass("[ln:"+here.lineNumber+"]Accessibility::updateProperties() method exists");
		} else {
	    	    DejaGnu.fail("[ln:"+here.lineNumber+"]Accessibility::updateProperties() method doesn't exist");
		}
 	}	
#else


 	if (Type.typeof(Accessibility.isActive) == ValueType.TFunction) {
 	    DejaGnu.pass("[ln:"+here.lineNumber+"]Accessibility::isActive method exists");
 	} else {
 	    DejaGnu.fail("[ln:"+here.lineNumber+"]Accessibility::isActive method doesn't exist");
 	}
	
	// Tests to see if all the methods exist. All these do is test for
	// existance of a method, and don't test the functionality at all. This
	// is primarily useful only to test completeness of the API implementation.
	DejaGnu.note("ME1A "   + Type.typeof(Accessibility.updateProperties()));

	if (Type.typeof(Accessibility.updateProperties) == ValueType.TFunction) {
	    DejaGnu.pass("[ln:"+here.lineNumber+"]Accessibility::updateProperties() method exists");
	} else {
	    DejaGnu.fail("[ln:"+here.lineNumber+"]Accessibility::updateProperties() method doesn't exist");
	}
#end		
//Si
//Add more tests here.

DejaGnu.note("\n The following tests come from the Ming test. \n " );
//      Accessibility has no constructor 
//	var accObj:Accessibility = new Accessibility();

#if flash9
#else
	var accobj = untyped __new__(Accessibility);
	if (Std.string(untyped __typeof__(accobj)) == 'undefined') {
		DejaGnu.pass("[ln:"+here.lineNumber+"]Type of accobj is undefined");
    } else {
        DejaGnu.fail("[ln:"+here.lineNumber+"]Type of accObj is not undefined");
    }
#end

//Check the prototype.
#if flash9
	//Do Nothing!
#else
	if ( Reflect.isObject(untyped Accessibility.__proto__)) {
		DejaGnu.pass("[ln:"+here.lineNumber+"]Accessibility.prototype.__proto_ field exists");
	} else {
		DejaGnu.fail("[ln:"+here.lineNumber+"]Accessibility.prototype.__proto__ field does not exist");
	}
	if ( untyped Accessibility.__proto__ == untyped Object.prototype) {
		DejaGnu.pass("[ln:"+here.lineNumber+"]Accessibility.prototype.__proto__ references Object.prototype");
	} else {
		DejaGnu.fail("[ln:"+here.lineNumber+"]Accessibility.prototype.__proto__ does not reference Object.prototype");
	}
#end

#if flash9
	if (untyped Accessibility.hasOwnProperty("active")) {
		DejaGnu.pass("[ln:"+here.lineNumber+"]Accessibility. 'active' property exists");
	} else {
		DejaGnu.fail("[ln:"+here.lineNumber+"]Accessibility. 'active' property does not exist");
	}
	if (Std.is(untyped Accessibility.active, Bool) ){
	  DejaGnu.pass("[ln:"+here.lineNumber+"]Accessibility.active is a bool");
	} else {
	    DejaGnu.fail("[ln:"+here.lineNumber+"]Accessibility.active is not a bool");
	}	
#else
	if (untyped Accessibility.hasOwnProperty("isActive")) {
		DejaGnu.pass("[ln:"+here.lineNumber+"]Accessibility.prototype. 'isActive' property exists");
	} else {
		DejaGnu.fail("[ln:"+here.lineNumber+"]Accessibility.prototype. 'isActive' property does not exist");
	}
	if (Type.typeof(untyped Accessibility.isActive)== TFunction){
		DejaGnu.pass("[ln:"+here.lineNumber+"]System.security 'isActive' is a function");
	} else {
	    DejaGnu.fail("[ln:"+here.lineNumber+"]System.security 'isActive' is not a function");
	}
    
    // This was set to xfail in the ming, but seems to pass now in both the pp
    // and gnash
    //xcheck_equals ( typeof(Accessibility.isActive()), 'boolean' );
	if ( untyped __typeof__(Reflect.callMethod(Accessibility, 
                                   Reflect.field(Accessibility, 'isActive'),
                                   []) == 'boolean') ){
        DejaGnu.pass("[ln:"+here.lineNumber+"]Accessibility.isActive should be a Bool!!!! WARNING!!!");
	} else {
	    DejaGnu.fail("[ln:"+here.lineNumber+"]Accessibility.isActive is not a Bool.");
	}
#end

	if (untyped Accessibility.hasOwnProperty("updateProperties")) {
	DejaGnu.pass("[ln:"+here.lineNumber+"]Accessibility.prototype. 'updateProperties' property exists");
	} else {
		DejaGnu.fail("[ln:"+here.lineNumber+"]Accessibility.prototype. 'updateProperties' property does not exist");
	}
	if (untyped Accessibility.hasOwnProperty("sendEvent")) {
	DejaGnu.pass("[ln:"+here.lineNumber+"]Accessibility.prototype. 'sendEvent' property exists");
	} else {
		DejaGnu.fail("[ln:"+here.lineNumber+"]Accessibility.prototype. 'sendEvent' property does not exist");
	}
	if (Type.typeof(untyped Accessibility.updateProperties)== TFunction){
		DejaGnu.pass("[ln:"+here.lineNumber+"]System.security 'updateProperties' is a function");
	} else {
	    DejaGnu.fail("[ln:"+here.lineNumber+"]System.security 'updateProperties' is not a function");
	}	if (Type.typeof(untyped Accessibility.sendEvent)== TFunction){
		DejaGnu.pass("[ln:"+here.lineNumber+"]System.security 'sendEvent' is a function");
	} else {
	    DejaGnu.fail("[ln:"+here.lineNumber+"]System.security 'sendEvent' is not a function");
	}	
	//These two methods are only defined in SWF 5. haxe does not compile SWF5!
	// Methods return void (just undefined in SWF 5).
    // Make sure the return type is reight.
    //	DejaGnu.note(""+untyped __typeof__(Accessibility.updateProperties()) );
    //	DejaGnu.note(""+untyped __typeof__(Accessibility.sendEvent()) );

#if flash9
#else
	if (untyped __typeof__(Reflect.callmethod(Accessibility,
                               Reflect.field(Accessibility, 'updateProperties'),
                               [] )) == 'undefined'){
		DejaGnu.pass("[ln:"+here.lineNumber+"]The return type of Accessibility.updateProperties is void (good).");
	} else {
	    DejaGnu.fail("[ln:"+here.lineNumber+"]The rerurn type of Accessibility.updateProperties is somewhat defined (wrong).");
	}

	if (untyped __typeof__(Reflect.callmethod(Accessibility,
                               Reflect.field(Accessibility, 'sendEvent'),
                               [] )) == 'undefined'){
		DejaGnu.pass("[ln:"+here.lineNumber+"]The return type of Accessibility.sendEvent is void (good).");
	} else {
	    DejaGnu.fail("[ln:"+here.lineNumber+"]The rerurn type of Accessibility.sendEvent() is somewhat defined (wrong).");
	}

#end

#if !flash9
    // This test crashes the pp in SWF 9 because Accessibility cannot be
    // instantiated
    
    // Accessibility object can't be instantiated !
    var accObj = untyped __new__(Accessibility);
    //check_equals(accObj, undefined);
    if (untyped __typeof__(accObj == 'undefined')) {
        DejaGnu.pass("[ln:"+here.lineNumber+"]Correctly did not instantiate an Accesibility object");
    } else {
        DejaGnu.fail("[ln:"+here.lineNumber+"]Unexpected return with call to new on Accessibility");
    }
#end

        // Call this after finishing all tests. It prints out the totals.
        DejaGnu.done();
    }
}

// local Variables:
// mode: C++
// indent-tabs-mode: t
// End:

