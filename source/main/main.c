
/* ---------- headers */

#include <cseries/cseries.h>
#include <cache/cache_files.h>

#include <stdio.h>

/* ---------- code */

int main()
{
    cache_file *file = cache_file_load("C:\\Halo\\Games\\Halo Combat Evolved [PC]\\gephyrophobia.map");
    printf("%s\n", cache_file_get_build(file));
    cache_file_dispose(file);

    return 0;
}
