/**
 * @file cl-midi.cpp
 * 
 */

#include <wsp/sound/cl-midi.hpp>
#ifndef __WSP_COMMONCORE_COMMON_RESULT_H__
#include <wsp/common/common_result.h>
#endif
#include <wsp/sound/sound_result.h>

extern "C"{
    #include <wsp/sound/fn-midi.h>
}

wsp::sound::Midi::Midi()
    : data_(NULL)
{
}

wsp::sound::Midi::Midi( const wsp::sound::Midi &rhs )
{
    *this=rhs;
}

wsp::sound::Midi::~Midi()
{
    if( data_ )
    {
        ::WSP_ReleaseMidiData( data_ );
    }
}

wsp::Result wsp::sound::Midi::Load( const char *filepath )
{
    if( data_ )
    {
        ::WSP_ReleaseMidiData( data_ );
        data_ = NULL;
    }
    data_ = ::WSP_ReadStandardMidiFile( filepath );
    if( data_==NULL )
    {
        return wsp::sound::ResultReadingMidiFileFailed();
    }
    return wsp::ResultSuccess();
}

wsp::Result wsp::sound::Midi::Save( const char *filepath )
{
    return wsp::ResultNotImplemented();    
}

wsp::sound::Midi& wsp::sound::Midi::operator=( const wsp::sound::Midi &rhs )
{
    if( *this==rhs )
    {
        ::WSP_CopyMidiData( data_, rhs.data() );
    }
    return *this;
}

bool wsp::sound::Midi::operator==( const wsp::sound::Midi &rhs )
{
    return WSP_IsEqualMidiData( data_, rhs.data() );
}
