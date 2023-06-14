#pragma once

unsigned int CompressData(const void *p_src, unsigned int p_srcLen, void *p_dest, unsigned int p_destLen);
unsigned int GetMaxCompressedLen(unsigned int p_len);
