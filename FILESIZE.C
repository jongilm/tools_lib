#include <io.h>
#include <fcntl.h>

long file_size(char *filename)
{
    int handle;
    long length;

    handle = open( filename, O_BINARY | O_RDONLY );
    length = filelength( handle );
    close( handle );

    return(length);
}
