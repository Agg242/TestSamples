#ifndef CHUNKUTIL_H__
#define CHUNKUTIL_H__

#include <stdio.h>
#include <exec/types.h>

#define SWAP_WORD(w) (((w) >> 8) | ((w) << 8))
#define SWAP_LONG(l) (((l) >> 24) | (((l) & 0x00ff0000) >> 8) | (((l) & 0x0000ff00) << 8) | ((l) << 24))

extern BOOL check_id(char *chunk_id, FILE *fptr);
extern ULONG get_chunk_size(char *chunk_id, FILE *fptr);

#endif