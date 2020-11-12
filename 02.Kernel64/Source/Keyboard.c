#include "Types.h"

BOOL kIsOutputBufferFull( void )
{
    if( kInPortByte( 0x64 ) & 0x01 )
    {
        return TRUE;
    }
    return FALSE;
}

BOOL  kIsInputBufferFull( void )
{
    if( kInPortByte( 0x64 ) & 0x02 )
    {
        return TRUE;
    }
    return FALSE;
}

BOOL kActivateKeyboard( void )
{
    int i;
    int j;

    kOutPortByte( 0x64, 0xAE );
    for ( i = 0 ; i < 0xFFFF ; i++ )
    {
        if( kIsInputBufferFull() == FALSE )
        {
            break;
        }
    }
    kOutPortByte( 0x60, 0xF4 );
    for( j = 0 ; i < 100 ; i++ )
    {
        for( i = 0 ; i < 0xFFFF ; i++ )
        {
            if( kIsOutputBufferFull() == TRUE )
            {
                break;
            }
        }

        if( kInPortByte( 0x60 ) == 0xFA )
        {
            return TRUE;
        }
    }
    return FALSE;
}

BYTE kGetKeyboardScanCode( void )
{
    while( kIsOutputBufferFull() == FALSE )
    {
        ;
    }
    return kInPortByte( 0x60 );
}

BOOL kChangeKeyboardLED( BOOL bCapsLockOn, BOOL bNumLockOn, BOOL bScrollLockOn )
{
    int i, j;
    for( i = 0 ; i < 0xFFFF ; i++ )
    {
        if( kIsInputBufferFull() == FALSE )
        {
            break;
        }
    }

    kOutPortByte( 0x60, 0xED );
    for( i = 0 ; i < 0xFFFF ; i++ )
    {
        if( kIsInputBufferFull() == FALSE )
        {
            break;
        }
    }

    for( j = 0 ; j < 100 ; j++ )
    {
        for( i = 0 ; i < 0xFFFF ; i++ )
        {
            if( kIsOutputBufferFull() == TRUE )
            {
                break;
            }
        }

        if( kInPortByte( 0x60 ) == 0xFA )
        {
            break;
        }
    }
    if( j >= 100 )
    {
        return FALSE;
    }

    kOutPortByte( 0x60, ( bCapsLockOn << 2 ) | ( bNumLockOn << 1 ) | bScrollLockOn );
    for( i = 0 ; i < 0xFFFF ; i++ )
    {
        if( kIsInputBufferFull() == FALSE )
        {
            break;
        }
    }

    for( j = 0 ; j < 100 ; j++ )
    {
        for( i = 0 ; i < 0xFFFF ; i++ )
        {
            if( kIsOutputBufferFull() == TRUE )
            {
                break;
            }
        }

        if( kInPortByte( 0x60 ) == 0xFA )
        {
            break;
        }
    }
    if( j >= 100 )
    {
        return FALSE;
    }
    return TRUE;
}

void kEnableA20Gate( void )
{
    BYTE bOutputPortData;
    int i;

    kOutPortByte( 0x64, 0xD0 );
    for( i = 0 ; i < 0xFFFF ; i++ )
    {
        if( kIsOutputBufferFull() == TRUE )
        {
            break;
        }
    }

    bOutputPortData = kInPortByte( 0x60 );

    bOutputPortData |= 0x01;

    for( i = 0 ; i < 0xFFFF ; i++ )
    {
        if( kIsInputBufferFull() == FALSE )
        {
            break;
        }
    }

    kOutPortByte( 0x64, 0xD1 );
    kOutPortByte( 0x60, bOutputPortData );
}