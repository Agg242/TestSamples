#include "chunkutil.h"
#include "dbg.h"


#define ID_LEN 4


BOOL check_id(char *chunk_id, FILE *fptr)
{
    BOOL res = FALSE;
    size_t bytes;
    char id[ID_LEN + 1];
    
    check(fptr != NULL, "Null file pointer");

    bytes = fread(id, 1, ID_LEN, fptr);
    check(bytes == ID_LEN, "Unable to read %s id", chunk_id);
    id[ID_LEN] = '\0';
    if(strcmp(id, chunk_id) == 0)
    {
        res = TRUE;
    }

on_error:
    return res;
}


ULONG get_chunk_size(char *chunk_id, FILE *fptr)
{
    ULONG size = 0;
    size_t bytes;

    while((!feof(fptr)) && (!check_id(chunk_id, fptr)))
    {
        // jump over chunk
        bytes = fread(&size, sizeof(ULONG), 1, fptr);
        check(bytes == 1, "Unable to read chunk size");
        fseek(fptr, size, SEEK_CUR);
    }
    
    if(feof(fptr))
    {
        size = 0;
    }
    else
    {
        bytes = fread(&size, sizeof(ULONG), 1, fptr);
        check(bytes == 1, "Unable to read chunk %s size", chunk_id);
    }
    
    return size;
        
on_error:
    return 0L;
}
