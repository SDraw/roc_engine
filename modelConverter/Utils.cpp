#include "stdafx.h"

#include "Utils.h"

unsigned int CompressData(const void *p_src, unsigned int p_srcLen, void *p_dest, unsigned int p_destLen)
{
    z_stream zInfo = { 0 };
    zInfo.total_in = zInfo.avail_in = p_srcLen;
    zInfo.total_out = zInfo.avail_out = p_destLen;
    zInfo.next_in = static_cast<unsigned char*>(const_cast<void*>(p_src));
    zInfo.next_out = static_cast<unsigned char*>(p_dest);

    int l_error;
    unsigned int l_ret = 0U;
    l_error = deflateInit(&zInfo, Z_DEFAULT_COMPRESSION);
    if(l_error == Z_OK)
    {
        l_error = deflate(&zInfo, Z_FINISH);
        if(l_error == Z_STREAM_END) l_ret = zInfo.total_out;
    }
    deflateEnd(&zInfo);
    return l_ret;
}

unsigned int GetMaxCompressedLen(unsigned int p_len)
{
    unsigned int l_blocks = (p_len + 16383) / 16384;
    return (p_len + 6 + (l_blocks * 5));
}
