
/* ---------- headers */

#include <cseries/cseries.h>
#include <cache/cache_files.h>

#include <stdio.h>

/* ---------- code */

int main()
{
	cache_file *file = cache_file_load("C:\\Halo\\Maps\\H3\\guardian.map");
	
	long tag_count = cache_file_get_tag_count(file);

	for (long i = 0; i < tag_count; i++)
		printf("0x%X - %s\n", i, cache_file_get_tag_name(file, i));

	printf("map: %s, build: %s, tag count: %li\n",
		cache_file_get_name(file),
		cache_file_get_build(file),
		tag_count);
	
	cache_file_dispose(file);

	return 0;
}
