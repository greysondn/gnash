// 
//   Copyright (C) 2008, 2009 Free Software Foundation, Inc.
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

#ifndef __HANDLER_H__
#define __HANDLER_H__ 1

#ifdef HAVE_CONFIG_H
#include "gnashconfig.h"
#endif

#include <boost/cstdint.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
//#include <boost/thread/condition.hpp>

#include <vector>
#include <string>
#include <deque>

#ifdef HAVE_POLL
# include <sys/poll.h>
#else 
# ifdef HAVE_EPOLL
#  include <sys/epoll.h>
# endif
#endif

#include "log.h"
#include "network.h"
#include "buffer.h"
#include "element.h"
#include "cque.h"
#include "network.h"
#include "dsodefs.h" //For DSOEXPORT.
#include "proc.h"

#include "diskstream.h"
#include "sharedlib.h"
#include "extension.h"

#include "rtmp.h"
#include "rtmp_msg.h"
#include "rtmp_server.h"
#include "network.h"

// _definst_ is the default instance name
namespace cygnal
{

class Cygnal;

class Handler : public gnash::Extension
{
public:
    /// \enum admin_cmd_e
    ///		The Admin command sent by the client
    typedef enum {
	UNKNOWN,
	STATUS,
	POLL,
	HELP,
	INTERVAL,
	QUIT,
    } admin_cmd_e;
    /// This enum contains the possible values for streaming video
    /// types.
    typedef enum {
	RECORD,
	LIVE,
	APPEND
    } pub_stream_e;
    /// This typedef is only used for the io function that must be
    /// supported by the plugin.
    typedef size_t (*cygnal_io_t)(boost::uint8_t *data, size_t size);
    typedef boost::shared_ptr<amf::Buffer> (*cygnal_io_read_t)();
    typedef struct {
	const char *version;
	const char *description;
  	cygnal_io_read_t read_func;
  	cygnal_io_t write_func;
	gnash::Network::protocols_supported_e protocol;
    } cygnal_init_t;
    /// This typedef is only used for the init function optionally
    /// supported by the plugin.
    typedef boost::shared_ptr<cygnal_init_t>(*cygnal_io_init_t)(boost::shared_ptr<gnash::RTMPMsg> &msg);

    DSOEXPORT Handler();
    ~Handler();

    /// \var sync
    ///     Send the onSync message to all connected clients
    bool sync() { return sync(_in_fd); };
    bool sync(int in_fd);

    // Access the name field
    void setName(const std::string &x) { _name = x; };
    std::string &getName() { return _name; }

    void addSOL(boost::shared_ptr<amf::Element> x) {
	_sol.push_back(x);
    };

    /// \method addClient
    ///     Add a client to the list for output messages.
    size_t addClient(int x, gnash::Network::protocols_supported_e proto);
    /// \method removeClient
    ///     Remove a client from the list for messages.
    void removeClient(int x);
    /// \var getClients
    ///     Get the vector of file descriptors for this handler.
    std::vector<int> &getClients() { return _clients; };
    gnash::Network::protocols_supported_e getProtocol(int x) { return _protocol[x]; };
    void setProtocol(int fd, gnash::Network::protocols_supported_e x) { _protocol[fd] = x; };
    
    /// \method addRemote
    ///     Add a remote machine to the list for input messages.
    size_t addRemote(int x) { _remote.push_back(x); return _remote.size(); };

    void setPlugin(boost::shared_ptr<Handler::cygnal_init_t> &init);
    void setPlugin(Handler::cygnal_io_t read_ptr, Handler::cygnal_io_t write_ptr );

    /// Initialize the named module within Cygnal
    //
    boost::shared_ptr<cygnal_init_t> initModule(const std::string& module);

    /// \method initialized
    ///     See if any of the cgi-bins has been loaded.
    bool initialized();

    boost::shared_ptr<amf::Buffer> readFromPlugin();

    size_t writeToPlugin(amf::Buffer &buf) {
	return writeToPlugin(buf.begin(), buf.allocated()); };
    size_t writeToPlugin(boost::uint8_t *data, size_t size);

    // These methods handle control of the file streaming 

    /// \fn     int createStream()
    int createStream();
    /// \overload int createStream(const std::string &filespec)
    /// @param filespec The spec of the file to stream
    int createStream(const std::string &filespec);

    /// \fn playStream
    ///    Play the specified file as a stream
    int playStream();
    /// \overload int playStream(const std::string &filespec)
    int playStream(const std::string &filespec);

    // Publish a live RTMP stream
    int publishStream();
    int publishStream(const std::string &filespec, pub_stream_e op);

    // Seek within the RTMP stream
    int seekStream();
    int seekStream(int offset);

    // Pause the RTMP stream
    int pauseStream();
    // Pause the RTMP stream
    int togglePause();

    // Resume the paused RTMP stream
    int resumeStream();

    // Close the RTMP stream
    int closeStream();

    void setFCSubscribe(const std::string &x) { _fcsubscribe = x; };
    std::string &getFCSubscribe() { return _fcsubscribe; }

    void setNetConnection(gnash::RTMPMsg *msg) { _netconnect.reset(msg); };
    void setNetConnection(boost::shared_ptr<gnash::RTMPMsg> msg) { _netconnect = msg; };
    boost::shared_ptr<gnash::RTMPMsg> getNetConnection() { return _netconnect;};
    
    // Dump internal data.
    void dump();
    
protected:
    /// \var _streams
    ///    This is a counter of how many streams have been allocated
    ///    by the server.
    int _streams;
    /// \var _name
    ///	    The name of the path this handler is supporting.
    std::string				_name;
    ///	    Each incoming request has one of 4 states the server has
    ///     to handle to send a response.

    /// \var _protocol
    ///    this is the map of which protocol is being used by which
    ///    file descriptor.
    std::map<int, gnash::Network::protocols_supported_e> _protocol;
    /// \var _clients
    ///	    is the array of all clients connected to this server for
    ///     this application. This is where all the output goes.
    std::vector<int>			_clients;
    /// \var _remote
    ///	    This is network connections to other processes,
    ///	    on other computers.
    std::vector<int>			_remote;

    /// \var _local
    ///    These are local process we're responsible for
    ///    starting and stopping.
    boost::shared_ptr<cygnal::Proc>	_local;
    /// \var _plugins
    ///	    is for the dynamically loaded applications
    boost::shared_ptr<cygnal_init_t>	_plugin;
    /// \var _file
    ///	    is for disk based files
    std::vector<boost::shared_ptr<gnash::DiskStream> > _files;
    /// \var _sol
    ///	    is for remote SharedObjects
    std::vector<boost::shared_ptr<amf::Element> > _sol;
    ///var _bodysize;
    ///     is to store the body size of the previous packet for this
    ///     channel. 4 and 1 byte heades don't use the length field,
    ///     they just use the previous vaue for this field.
    std::map<int, size_t>		_bodysize;
    /// \var _in_fd
    ///	    The file descriptor of the incoming data for an
    ///     Invoke message.
    int					_in_fd;

    /// \var _fcssubscribe
    ///    This is a string sometimes sent by the client with what
    ///    appears to be a unique ID number.
    std::string				_fcsubscribe;

    /// \var _netconnect
    ///    This store the data from the NetConnection ActionScript
    ///    object we get as the final part of the handshake process
    ///    that is used to set up the connection. This has all the
    ///    file paths and other information needed by the server.
    boost::shared_ptr<gnash::RTMPMsg>	_netconnect;
private:    
    boost::mutex			_mutex;
    
// Remote Shared Objects. References are an index into this vector.
//    std::map<std::string, boost::shared_ptr<handler_t> > _handlers;
};

} // end of gnash namespace

#endif // end of __HANDLER_H__

// local Variables:
// mode: C++
// indent-tabs-mode: t
// End: