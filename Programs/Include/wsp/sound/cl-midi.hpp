/**
 * @file cl-midi.hpp
 * 
 */

#ifndef _WSP_SOUND_CLASS_MIDI_HPP_
#define _WSP_SOUND_CLASS_MIDI_HPP_

#include <wsp/sound/_define_sound.h>
#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif
#include <wsp/sound/_midi_types.h>

namespace wsp{ namespace sound{
    /** @brief The class which handles accessing to MIDI data */
    class WSP_DLL_EXPORT Midi
    {
    public:
        Midi();
        explicit Midi( const wsp::sound::Midi &rhs );
        ~Midi();

        /* getters */
        inline const WSP_MidiData* data() const{ return data_; }

        /* I/O */
        wsp::Result Load( const char *filepath );
        wsp::Result Save( const char *filepath );

        wsp::sound::Midi& operator=( const wsp::sound::Midi &rhs );
        bool operator==( const wsp::sound::Midi &rhs );

    private:
        WSP_MidiData *data_;
    };

}}

#endif
