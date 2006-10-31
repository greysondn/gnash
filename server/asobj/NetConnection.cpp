// 
//   Copyright (C) 2005, 2006 Free Software Foundation, Inc.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

// 
//
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <string>
#include <new>
#include "NetConnection.h"
#include "fn_call.h"
#include "rtmp.h"
#include "log.h"

using namespace std;
using namespace amf;

namespace gnash {

/// \class NetConnection
/// \brief Opens a local connection through which you can play
/// back video (FLV) files from an HTTP address or from the local file
/// system.

/// First introduced for swf v7

/// protocol:[//host][:port]/appname/[instanceName]

/// For protocol, specify either rtmp or rtmpt. If rtmp is specified,
/// Flash Player will create a persistent Communication Server. If
/// rtmpt is specified, Flash Player will create an HTTP "tunneling"
/// connection to the server. For more information on RTMP and RTMPT,
/// see the description section below.
//
/// You can omit the host parameter if the Flash movie is served from
/// the same host where Flash Communication Server is installed.
/// If the instanceName parameter is omitted, Flash Player connects to
/// the application's default instance (_definst_).
/// By default, RTMP connections use port 1935, and RTMPT connections
/// use port 80.
NetConnection::NetConnection() {
}

NetConnection::~NetConnection() {
}

/// \brief Open a connection to stream FLV files.
/// \param the URL
/// \return nothing
/// \note Older Flash movies can only take a NULL value as
/// the parameter, which therefor only connects to the localhost using
/// RTMP. Newer Flash movies have a parameter to connect which is a
/// URL string like rtmp://foobar.com/videos/bar.flv
bool
NetConnection::connect(const char *arg)
{
    GNASH_REPORT_FUNCTION;
    
    string::size_type first_colon;
    string::size_type second_colon;
    string::size_type single_slash;
    string::size_type double_slash;

    if (arg != 0) {
        if (strcmp(arg, "null") == 0) {
            log_warning("No URL specified!\n");
            return false;
        }
        _url = arg;
        // protocol:[//host][:port]/appname/[instanceName]
        first_colon = _url.find(':', 0);
        second_colon = _url.find(':', first_colon + 1);
        double_slash = _url.find("//", 0) + 2;
        single_slash = _url.find("/", double_slash);
        _protocol = _url.substr(0, first_colon);
        if (second_colon != string::npos) {
            _host = _url.substr(double_slash, second_colon - double_slash);
            _portstr = _url.substr(second_colon + 1, single_slash - second_colon - 1);
            _port = (short)strtol(_portstr.c_str(), NULL, 0);
        } else {
            _host = _url.substr(double_slash, single_slash - double_slash);
            if (_protocol == "rtmp") {
                _port = RTMP;
            }
            if (_protocol == "http") {
                _port = RTMPT;
            }
        }
        _path = _url.substr(single_slash, _url.size());

        
        if (_portstr.size() == 0) {
            log_msg("Loading FLV file from: %s://%s%s\n",
                    _protocol.c_str(), _host.c_str(), _path.c_str());
        } else {
            log_msg("Loading FLV file from: %s://%s:%s%s\n",
                    _protocol.c_str(), _host.c_str(),
                    _portstr.c_str(), _path.c_str());
        }
    } else {
        log_msg("Connecting to localhost\n");
    }

    RTMPproto proto;

    if (proto.createClient(_host.c_str(), _port)) {
        proto.handShakeRequest();
        proto.clientFinish();
    } else {
        dbglogfile << "ERROR: Couldn't connect to server!" << endl;
        return false;
    }

    return true;
}

/// \brief callback to instantiate a new NetConnection object.
/// \param fn the parameters from the Flash movie
/// \return nothing from the function call.
/// \note The return value is returned through the fn.result member.
void
netconnection_new(const fn_call& fn)
{
    GNASH_REPORT_FUNCTION;
//    log_msg("%s: %d args\n", __PRETTY_FUNCTION__, fn.nargs);
        
    netconnection_as_object *netconnection_obj = new netconnection_as_object;

    netconnection_obj->set_member("connect", &netconnection_connect);
#ifdef ENABLE_TESTING
    netconnection_obj->set_member("geturl",  &netconnection_geturl);
    netconnection_obj->set_member("getprotocol",  &netconnection_getprotocol);
    netconnection_obj->set_member("gethost", &netconnection_gethost);
    netconnection_obj->set_member("getport", &netconnection_getport);
    netconnection_obj->set_member("getpath", &netconnection_getpath);
#endif
    fn.result->set_as_object(netconnection_obj);
}

void netconnection_connect(const fn_call& fn)
{
    GNASH_REPORT_FUNCTION;
//    log_msg("%s: %d args\n", __PRETTY_FUNCTION__, fn.nargs);
    
    string filespec;
    netconnection_as_object *ptr = (netconnection_as_object*)fn.this_ptr;
    
    assert(ptr);
    if (fn.nargs != 0) {
        filespec = fn.env->bottom(fn.first_arg_bottom_index).to_string();
        ptr->obj.connect(filespec.c_str());
    } else {
        ptr->obj.connect(0);
    }    
}

#ifdef ENABLE_TESTING
// These are the callbacks used to define custom methods for our AS
// classes. This way we can examine the private dats after calling a
// method to see if it worked correctly.
void netconnection_geturl(const fn_call& fn)
{

    netconnection_as_object *ptr = (netconnection_as_object*)fn.this_ptr;
    assert(ptr);
    fn.result->set_tu_string(ptr->obj.getURL().c_str());
}

void netconnection_getprotocol(const fn_call& fn)
{
    netconnection_as_object *ptr = (netconnection_as_object*)fn.this_ptr;
    assert(ptr);
    fn.result->set_tu_string(ptr->obj.getProtocol().c_str());
}

void netconnection_gethost(const fn_call& fn)
{
    netconnection_as_object *ptr = (netconnection_as_object*)fn.this_ptr;
    assert(ptr);
    fn.result->set_tu_string(ptr->obj.getHost().c_str());
}

void netconnection_getport(const fn_call& fn)
{
    netconnection_as_object *ptr = (netconnection_as_object*)fn.this_ptr;
    assert(ptr);
    fn.result->set_tu_string(ptr->obj.getPortStr().c_str());
}

void netconnection_getpath(const fn_call& fn)
{
    netconnection_as_object *ptr = (netconnection_as_object*)fn.this_ptr;
    assert(ptr);
    fn.result->set_tu_string(ptr->obj.getPath().c_str());
}

void netconnection_connected(const fn_call& fn)
{
    netconnection_as_object *ptr = (netconnection_as_object*)fn.this_ptr;
    assert(ptr);
    fn.result->set_bool(ptr->obj.connected());
}
void netconnection_getfilefd(const fn_call& fn)
{
    netconnection_as_object *ptr = (netconnection_as_object*)fn.this_ptr;
    assert(ptr);
    fn.result->set_int(ptr->obj.getFileFd());
}
void netconnection_getlistenfd(const fn_call& fn)
{
    netconnection_as_object *ptr = (netconnection_as_object*)fn.this_ptr;
    assert(ptr);
    fn.result->set_int(ptr->obj.getListenFd());
}
#endif

} // end of gnash namespace

