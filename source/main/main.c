
/* ---------- headers */

#include <stdio.h>

#include <cseries/cseries.h>
#include <cache/cache_files.h>
#include <tag_files/tag_groups.h>

/* ---------- code */

int main()
{
	cache_file *file = cache_file_load("C:\\Halo\\Maps\\H3\\guardian.map");

	puts("strings:");
	
	long string_count = cache_file_get_string_count(file);

	for (long i = 0; i < string_count; i++)
		printf("0x%X - %s\n", i, cache_file_get_string(file, i));
	
	puts("");
	puts("tag instances:");

	long tag_count = cache_file_get_tag_count(file);

	for (long i = 0; i < tag_count; i++)
		printf("0x%X - %s\n", i, cache_file_get_tag_name(file, i));
	
	puts("");

	printf("map: %s, build: %s, tag count: %li, string count: %li\n",
		cache_file_get_name(file),
		cache_file_get_build(file),
		tag_count,
		string_count);
	
	cache_file_dispose(file);

	return 0;
}
