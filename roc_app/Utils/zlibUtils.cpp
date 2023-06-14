#include "stdafx.h"
#include "Utils/zlibUtils.h"

namespace zlibUtils
{

unsigned int CompressData(const void *p_src, unsigned int p_srcLen, void *p_dest, unsigned int p_destLen)
{
    z_stream zInfo = { 0 };
    zInfo.total_in = zInfo.avail_in = p_srcLen;
    zInfo.total_out = zInfo.avail_out = p_destLen;
    zInfo.next_in = reinterpret_cast<unsigned char*>(const_cast<void*>(p_src));
    zInfo.next_out = reinterpret_cast<unsigned char*>(p_dest);

    unsigned int l_ret = 0U;
    if((deflateInit(&zInfo, Z_DEFAULT_COMPRESSION) == Z_OK) && (deflate(&zInfo, Z_FINISH) == Z_STREAM_END))
        l_ret = zInfo.total_out;

    deflateEnd(&zInfo);
    return l_ret;
}

unsigned int UncompressData(const void *p_src, unsigned int p_srcLen, void *p_dest, unsigned int p_destLen)
{
    z_stream zInfo = { 0 };
    zInfo.total_in = zInfo.avail_in = p_srcLen;
    zInfo.total_out = zInfo.avail_out = p_destLen;
    zInfo.next_in = reinterpret_cast<unsigned char*>(const_cast<void*>(p_src));
    zInfo.next_out = reinterpret_cast<unsigned char*>(p_dest);

    int l_error;
    unsigned int l_ret = 0U;
    l_error = inflateInit(&zInfo);
    if(l_error == Z_OK)
    {
        l_error = inflate(&zInfo, Z_FINISH);
        if(l_error == Z_STREAM_END) l_ret = zInfo.total_out;
    }
    inflateEnd(&zInfo);
    return l_ret;
}

unsigned int GetMaxCompressedLen(unsigned int nLenSrc)
{
    return (nLenSrc + 6U + ((nLenSrc + 16383U) / 16384U * 5U));
}

}
