//
//   Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010 Free Software
//   Foundation, Inc
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

#ifdef HAVE_CONFIG_H
#include "gnashconfig.h"
#endif

#include <iostream>
#include <cerrno>
#include <exception>
#include <sstream>
#include <csignal>

#include "log.h"
#include "GnashException.h"

#include "RawFBDevice.h"
#include "GnashDevice.h"

namespace gnash {

namespace renderer {

namespace rawfb {
    
// The debug log used by all the gnash libraries.
static LogFile& dbglogfile = LogFile::getDefaultInstance();


RawFBDevice::RawFBDevice()
    : _fd(0),
      _fbmem(0)
{
    GNASH_REPORT_FUNCTION;
}

RawFBDevice::RawFBDevice(int vid)
    : _fd(0),
      _fbmem(0)
{
    GNASH_REPORT_FUNCTION;

    if (!initDevice(0, 0)) {
        log_error("Couldn't initialize RAWFB device!");
    }
}

RawFBDevice::RawFBDevice(int argc, char *argv[])
    : _fd(0),
      _fbmem(0)
{
    GNASH_REPORT_FUNCTION;
    
}

void
RawFBDevice::clear()
{
    GNASH_REPORT_FUNCTION;
    memset(_fbmem, 0, _fixinfo.smem_len);
}

RawFBDevice::~RawFBDevice()
{
    GNASH_REPORT_FUNCTION;

    if (_fbmem) {
        munmap(_fbmem, 0);
        _fbmem = 0;
        if (_fd) {
            close (_fd);
            _fd = -1;
        }
    }
}

bool
RawFBDevice::initDevice(int /* argc */, char **/* argv[] */)
{
    GNASH_REPORT_FUNCTION;

    // Open the framebuffer device
#ifdef ENABLE_FAKE_FRAMEBUFFER
    _fd = open(FAKEFB, O_RDWR);
    log_debug("WARNING: Using %s as a fake framebuffer!", FAKEFB);
#else
    _fd = open("/dev/fb0", O_RDWR);
#endif
    if (_fd < 0) {
        log_error("Could not open framebuffer device: %s", strerror(errno));
        return false;
    }
    
    // Load framebuffer properties
#ifdef ENABLE_FAKE_FRAMEBUFFER
    fakefb_ioctl(_fd, FBIOGET_VSCREENINFO, &_varinfo);
    fakefb_ioctl(_fd, FBIOGET_FSCREENINFO, &_fixinfo);
#else
    ioctl(_fd, FBIOGET_VSCREENINFO, &_varinfo);
    ioctl(_fd, FBIOGET_FSCREENINFO, &_fixinfo);
#endif
    log_debug(_("Framebuffer device uses %d bytes of memory."),
              _fixinfo.smem_len);
    log_debug(_("Video mode: %dx%d with %d bits per pixel."),
              _varinfo.xres, _varinfo.yres,
              _varinfo.bits_per_pixel);    

    // map framebuffer into memory
#if 0
    _fbmem.reset(static_cast<boost::uint8_t *>(mmap(0, _fixinfo.smem_len,
                                              PROT_READ|PROT_WRITE, MAP_SHARED,
                                              _fd, 0)));
#else
    _fbmem = (unsigned char *)mmap(0, _fixinfo.smem_len,
                                   PROT_READ|PROT_WRITE, MAP_SHARED,
                                   _fd, 0);
#endif
    
    if (!_fbmem) {
        log_error("Couldn't mmap() %d bytes of memory!",
                  _fixinfo.smem_len);
        return false;
    }
    
    return true;
}

// Initialize RAWFB Window layer
bool
RawFBDevice::attachWindow(GnashDevice::native_window_t window)
{
    GNASH_REPORT_FUNCTION;
}
    

// Return a string with the error code as text, instead of a numeric value
const char *
RawFBDevice::getErrorString(int error)
{
}

// Create an RAWFB window to render in. This is only used by testing
void
RawFBDevice::createWindow(const char *name, int x, int y, int width, int height)
{
    GNASH_REPORT_FUNCTION;
}

void
RawFBDevice::eventLoop(size_t passes)
{
    GNASH_REPORT_FUNCTION;    
}

#ifdef ENABLE_FAKE_FRAMEBUFFER
// Simulate the ioctls used to get information from the framebuffer
// driver. Since this is an emulator, we have to set these fields
// to a reasonable default.
int
fakefb_ioctl(int /* fd */, int request, void *data)
{
    // GNASH_REPORT_FUNCTION;
    
    switch (request) {
      case FBIOGET_VSCREENINFO:
      {
          struct fb_var_screeninfo *ptr =
              reinterpret_cast<struct fb_var_screeninfo *>(data);
          // If we are using a simulated framebuffer, the default for
          // fbe us 640x480, 8bits. So use that as a sensible
          // default. Note that the fake framebuffer is only used for
          // debugging and development.
          ptr->xres          = 1280; // visible resolution
          ptr->xres_virtual  = 1280; // virtual resolution
          ptr->yres          = 1024; // visible resolution
          ptr->yres_virtual  = 1024; // virtual resolution

          // standard PC framebuffer use a 32 bit 8/8/8 framebuffer
          ptr->bits_per_pixel = 32;
          ptr->red.offset    = 16;
          ptr->red.length    = 8;
          ptr->green.offset  = 8;
          ptr->green.length  = 8;
          ptr->blue.offset   = 0;
          ptr->blue.length   = 8;
          ptr->transp.offset = 0;
          ptr->transp.length = 0;
#if 0
          // Android and fbe use a 16bit 5/6/5 framebuffer
          ptr->bits_per_pixel = 16;
          ptr->red.length    = 5;
          ptr->red.offset    = 11;
          ptr->green.length  = 6;
          ptr->green.offset  = 5;
          ptr->blue.length   = 5;
          ptr->blue.offset   = 0;
          ptr->transp.offset = 0;
          ptr->transp.length = 0;
#endif
          // 8bit framebuffer
          // ptr->bits_per_pixel = 8;
          // ptr->red.length    = 8;
          // ptr->red.offset    = 0;
          // ptr->green.length  = 8;
          // ptr->green.offset  = 0;
          // ptr->blue.length   = 8;
          // ptr->blue.offset   = 0;
          // ptr->transp.offset = 0;
          // ptr->transp.length = 0;
          ptr->grayscale     = 1; // != 0 Graylevels instead of color
          
          break;
      }
      case FBIOGET_FSCREENINFO:
      {
          struct fb_fix_screeninfo *ptr =
              reinterpret_cast<struct fb_fix_screeninfo *>(data);
          // Android and fbe use a 16bit 5/6/5 framebuffer
          ptr->smem_len = 5242880; // size of frame buffer memory
          ptr->type = 0; // see FB_TYPE_*
          ptr->visual = 2; // see FB_VISUAL_*
          ptr->xpanstep = 1;      // zero if no hardware panning
          ptr->ypanstep = 1;      // zero if no hardware panning
          ptr->ywrapstep = 0;     // zero if no hardware panning
          ptr->line_length = 5120; // line length
          ptr->accel = FB_ACCEL_NONE; // Indicate to driver which specific
                                  // chip/card we have
#if 0
          // Android and fbe use a 16bit 5/6/5 framebuffer
          ptr->smem_len = 307200; // Length of frame buffer mem
          ptr->type = FB_TYPE_PACKED_PIXELS; // see FB_TYPE_*
          ptr->visual = FB_VISUAL_PSEUDOCOLOR; // see FB_VISUAL_*
          ptr->xpanstep = 0;      // zero if no hardware panning
          ptr->ypanstep = 0;      // zero if no hardware panning
          ptr->ywrapstep = 0;     // zero if no hardware panning
          ptr->accel = FB_ACCEL_NONE; // Indicate to driver which specific
                                  // chip/card we have
#endif
          break;
      }
      case FBIOPUTCMAP:
      {
          // Fbe uses this name for the fake framebuffer, so in this
          // case assume we're using fbe, so write to the known fbe
          // cmap file.
          std::string str = FAKEFB;
          if (str == "/tmp/fbe_buffer") {
              int fd = open("/tmp/fbe_cmap", O_WRONLY);
              if (fd) {
                  write(fd, data, sizeof(struct fb_cmap));
                  close(fd);
              } else {
                  gnash::log_error("Couldn't write to the fake cmap!");
                  return -1;
              }
          } else {
              gnash::log_error("Couldn't write to the fake cmap, unknown type!");
              return -1;
          }
          // If we send a SIGUSR1 signal to fbe, it'll reload the
          // color map.
          int fd = open("/tmp/fbe.pid", O_RDONLY);
          char buf[10];
          if (fd) {
              if (read(fd, buf, 10) == 0) {
                  close(fd);
                  return -1;
              } else {
                  pid_t pid = strtol(buf, 0, NULL);
                  kill(pid, SIGUSR1);
                  gnash::log_debug("Signaled fbe to reload it's colormap.");
              }
              close(fd);
          }
          break;
      }
      default:
          gnash::log_unimpl("fakefb_ioctl(%d)", request);
          break;
    }

    return 0;
}
#endif  // ENABLE_FAKE_FRAMEBUFFER

} // namespace rawfb
} // namespace renderer
} // namespace gnash

// local Variables:
// mode: C++
// indent-tabs-mode: nil
// End:
