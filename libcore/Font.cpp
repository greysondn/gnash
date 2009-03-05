// Font.cpp:  ActionScript Font handling, for Gnash.
// 
//   Copyright (C) 2006, 2007, 2008, 2009 Free Software Foundation, Inc.
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


// Based on the public domain work of Thatcher Ulrich <tu@tulrich.com> 2003

#include "smart_ptr.h" // GNASH_USE_GC
#include "Font.h"
#include "log.h"
#include "shape_character_def.h"
#include "DefineFontTag.h"
#include "FreetypeGlyphsProvider.h"

#include <utility> // for std::make_pair

namespace gnash {


Font::GlyphInfo::GlyphInfo()
	:
	glyph(),
	advance(0)
{}

Font::GlyphInfo::GlyphInfo(boost::intrusive_ptr<shape_character_def> glyph,
        float advance)
	:
	glyph(glyph.get()),
	advance(advance)
{}

Font::GlyphInfo::GlyphInfo(const GlyphInfo& o)
	:
	glyph(o.glyph.get()),
	advance(o.advance)
{}

#ifdef GNASH_USE_GC
void
Font::GlyphInfo::markReachableResources() const
{
	if ( glyph ) glyph->setReachable();
}
#endif

Font::Font(std::auto_ptr<SWF::DefineFontTag> ft)
    :
    _fontTag(ft.release()),
    _name(_fontTag->name()),
    _unicodeChars(_fontTag->unicodeChars()),
    _shiftJISChars(_fontTag->shiftJISChars()),
    _ansiChars(_fontTag->ansiChars()),
    _italic(_fontTag->italic()),
    _bold(_fontTag->bold())
{
    if (_fontTag->hasCodeTable()) _embeddedCodeTable = _fontTag->getCodeTable();
}

Font::Font(const std::string& name, bool bold, bool italic)
    :
    _fontTag(0),
    _name(name),
    _unicodeChars(false),
    _shiftJISChars(false),
    _ansiChars(true),
    _italic(italic),
    _bold(bold)
{
    assert(!_name.empty());
}

Font::~Font()
{
}

shape_character_def*
Font::get_glyph(int index, bool embedded) const
{
    // What to do if embedded is true and this is a
    // device-only font?
    const GlyphInfoRecords& lookup = (embedded && _fontTag) ? 
            _fontTag->glyphTable() : _deviceGlyphTable;

    if (index >= 0 && (size_t)index < lookup.size())
    {
        return lookup[index].glyph.get();
    }
    else
    {
        // TODO: should we log an error here ?
        return NULL;
    }
}

void
Font::addFontNameInfo(const FontNameInfo& fontName)
{
    if (!_displayName.empty() || !_copyrightName.empty())
    {
        IF_VERBOSE_MALFORMED_SWF(
            log_swferror(_("Attempt to set font display or copyright name "
                    "again. This should mean there is more than one "
                    "DefineFontName tag referring to the same Font. Don't "
                    "know what to do in this case, so ignoring."));
        );
        return;
    }

    _displayName = fontName.displayName;
    _copyrightName = fontName.copyrightName;
}


Font::GlyphInfoRecords::size_type
Font::glyphCount() const
{
        assert(_fontTag);
        return _fontTag->glyphTable().size();
}


void
Font::setFlags(boost::uint8_t flags)
{
    _shiftJISChars = flags & (1 << 6);
    _unicodeChars = flags & (1 << 5);
    _ansiChars = flags & (1 << 4);
    _italic = flags & (1 << 1);
    _bold = flags & (1 << 0);
}


void
Font::setCodeTable(std::auto_ptr<CodeTable> table)
{
    if (_embeddedCodeTable)
    {
        IF_VERBOSE_MALFORMED_SWF(
            log_swferror(_("Attempt to add an embedded glyph CodeTable to "
                    "a font that already has one. This should mean there "
                    "are several DefineFontInfo tags, or a DefineFontInfo "
                    "tag refers to a font created by DefineFone2 or "
                    "DefineFont3. Don't know what should happen in this "
                    "case, so ignoring."));
        );
        return;
    }
    _embeddedCodeTable.reset(table.release());
}

    
void
Font::setName(const std::string& name)
{
    _name = name;
}

int
Font::get_glyph_index(boost::uint16_t code, bool embedded) const
{
    const CodeTable& ctable = (embedded && _embeddedCodeTable) ? 
        *_embeddedCodeTable : _deviceCodeTable;

    int glyph_index = -1;
    CodeTable::const_iterator it = ctable.find(code);
    if ( it != ctable.end() )
    {
        glyph_index = it->second;
        return glyph_index;
    }

    // Try adding an os font, if possible
    if ( ! embedded )
    {
        glyph_index = const_cast<Font*>(this)->add_os_glyph(code);
    }
    return glyph_index;
}

float Font::get_advance(int glyph_index, bool embedded) const
{
    // What to do if embedded is true and this is a
    // device-only font?
    const GlyphInfoRecords& lookup = (embedded && _fontTag) ? 
            _fontTag->glyphTable() : _deviceGlyphTable;

    if (glyph_index < 0)
    {
        // Default advance.
        return 512.0f;
    }

    if (static_cast<size_t>(glyph_index) < lookup.size())
    {
        assert(glyph_index >= 0);
        return lookup[glyph_index].advance;
    }
    else
    {
        // Bad glyph index.  Due to bad data file?
        abort();
        return 0;
    }
}


// Return the adjustment in advance between the given two
// characters.  Normally this will be 0; i.e. the 
float	Font::get_kerning_adjustment(int last_code, int code) const
{
    kerning_pair	k;
    k.m_char0 = last_code;
    k.m_char1 = code;
    kernings_table::const_iterator it = m_kerning_pairs.find(k);
    if ( it != m_kerning_pairs.end() )
    {
        float adjustment = it->second;
        return adjustment;
    }
    return 0;
}

unsigned short int Font::unitsPerEM(bool embed) const
{
    // the EM square is 1024 x 1024 for DefineFont up to 2
    // and 20 as much for DefineFont3 up
    if (embed)
    {
        // If this is not an embedded font, what should we do
        // here?
        if ( _fontTag && _fontTag->subpixelFont() ) return 1024 * 20;
        else return 1024;
    }
    else
    {
        if ( ! _ftProvider.get() )
        {
            if ( ! initDeviceFontProvider() )
            {
                log_error("Device font provider was not initialized, "
                        "can't get unitsPerEM");
                return 0; // can't query it..
            }
        }
        return _ftProvider->unitsPerEM();
    }
}

int
Font::add_os_glyph(boost::uint16_t code)
{
    if ( ! _ftProvider.get() )
    {
        if ( ! initDeviceFontProvider() )
        {
            log_error("Device font provider was not initialized, can't "
                    "get unitsPerEM");
            return -1; // can't provide it...
        }
    }

    assert(_deviceCodeTable.find(code) == _deviceCodeTable.end());

    float advance;

    // Get the vectorial glyph
    boost::intrusive_ptr<shape_character_def> sh = 
        _ftProvider->getGlyph(code, advance);

    if ( ! sh )
    {
        log_error("Could not create shape "
                "glyph for character code %u (%c) with "
                "device font %s (%p)", code, code, _name,
                _ftProvider.get());
        return -1;
    }

    // Find new glyph offset
    int newOffset = _deviceGlyphTable.size();

    // Add the new glyph id
    _deviceCodeTable[code] = newOffset;

    _deviceGlyphTable.push_back(GlyphInfo(sh, advance));

    testInvariant();

    return newOffset;
}

bool
Font::initDeviceFontProvider() const
{
    if ( _name.empty() )
    {
        log_error("No name associated with this font, can't use device "
                "fonts (should I use a default one?)");
        return false;
    }

    _ftProvider = FreetypeGlyphsProvider::createFace(_name, _bold, _italic);
    if ( ! _ftProvider.get() )
    {
        log_error("Could not create a freetype face %s", _name);
        return false;
    }
    return true;
}

bool
Font::matches(const std::string& name, bool bold, bool italic) const
{
	return (_bold == bold && _italic == italic && name ==_name);
}

// TODO: what about device fonts?
float
Font::get_leading() const {
    return _fontTag ? _fontTag->leading() : 0.0f;
}

// TODO: what about device fonts?
float
Font::get_descent() const {
    return _fontTag ? _fontTag->descent() : 0.0f;
}
    
// TODO: what about device fonts?
bool
Font::is_subpixel_font() const {
    return _fontTag ? _fontTag->subpixelFont() : false;
}

#ifdef GNASH_USE_GC
/// Mark reachable resources (for the GC)
//
/// Reachable resources are:
///	- shape_character_defs (vector glyphs, devide and embeded)
///
void
Font::markReachableResources() const
{
	// Mark device glyphs (textured and vector)
	for (GlyphInfoRecords::const_iterator i = _deviceGlyphTable.begin(),
            e=_deviceGlyphTable.end(); i != e; ++i)
	{
		i->markReachableResources();
	}

}
#endif // GNASH_USE_GC


}	// end namespace gnash


// Local Variables:
// mode: C++
// indent-tabs-mode: t
// End:
