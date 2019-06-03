#pragma once

/* ---------- types */

typedef struct cache_header_gen1
{
	tag header_signature;
	cache_version version;
	long file_length;
	long compressed_file_length;
	long offset_to_index;
	long tag_buffer_size;
	long : 32;
	long : 32;
	tag_string name;
	tag_string build;
	cache_type type;
	dword crc;
	long : 32;
	char unused[1936];
	tag footer_signature;
} cache_header_gen1;

typedef struct cache_tag_header_gen1
{
	dword tags_offset;
	long scenario_index;
	dword checksum;
	long count;
	long vertices_offset;
	long vertex_count;
	long indices_offset;
	long index_count;
	long model_data_size;
	tag signature;
} cache_tag_header_gen1;

typedef struct cache_tag_instance_gen1
{
	tag group_tags[3];
	long index;
	dword name_offset;
	dword offset;
	bool external;
	char : 8;
	short : 16;
	long : 32;
} cache_tag_instance_gen1;

/* ---------- prototypes */

cache_file *cache_file_gen1_load(char const *path);
void cache_file_gen1_dispose(cache_file *file);
dword cache_file_gen1_get_base_address();
cache_tag_instance_gen1 *cache_file_gen1_get_tag_instance(cache_file *file, long index);

long cache_header_gen1_get_file_length(cache_header_gen1 *header);
long cache_header_gen1_get_tag_header_offset(cache_header_gen1 *header);
long cache_header_gen1_get_tag_buffer_size(cache_header_gen1 *header);
char const *cache_header_gen1_get_name(cache_header_gen1 *header);
char const *cache_header_gen1_get_build(cache_header_gen1 *header);
cache_type cache_header_gen1_get_type(cache_header_gen1 *header);
cache_shared_type cache_header_gen1_get_shared_type(cache_header_gen1 *header);

long cache_tag_header_gen1_get_tag_count(cache_tag_header_gen1 *header);
dword cache_tag_header_gen1_get_tags_offset(cache_tag_header_gen1 *header);

tag cache_tag_instance_gen1_get_group_tag(cache_file *file, cache_tag_instance_gen1 *instance);
long cache_tag_instance_gen1_get_index(cache_file *file, cache_tag_instance_gen1 *instance);
dword cache_tag_instance_gen1_get_name_offset(cache_file *file, cache_tag_instance_gen1 *instance);
dword cache_tag_instance_gen1_get_offset(cache_file *file, cache_tag_instance_gen1 *instance);
