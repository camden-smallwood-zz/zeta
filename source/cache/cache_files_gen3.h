#pragma once

/* ---------- types */

typedef enum cache_partition_type
{
	_cache_partition_type_resources,
	_cache_partition_type_sound_resources,
	_cache_partition_type_global_tags,
	_cache_partition_type_shared_tags,
	_cache_partition_type_base,
	_cache_partition_type_map_tags,
	NUMBER_OF_CACHE_PARTITIONS
} cache_partition_type;

typedef struct cache_partition
{
	dword base_address;
	long size;
} cache_partition;

typedef enum cache_section_type
{
	_cache_section_type_debug,
	_cache_section_type_resources,
	_cache_section_type_tag,
	_cache_section_type_localization,
	NUMBER_OF_CACHE_SECTIONS
} cache_section_type;

typedef struct cache_section
{
	dword virtual_address;
	long size;
} cache_section;

typedef struct cache_interop
{
	dword resource_base_address;
	long debug_section_size;
	dword runtime_base_address;
	dword unknown_base_address;
	cache_section sections[NUMBER_OF_CACHE_SECTIONS];
} cache_interop;

typedef struct cache_header_gen3
{
	tag header_signature;
	cache_version version;
	dword file_length;
	dword compressed_file_length;
	dword offset_to_index;
	dword index_stream_size;
	dword tag_buffer_size;
	long_string source_file;
	tag_string build;
	cache_type type : 16;
	cache_shared_type shared_type : 16;
	char : 8;
	bool tracked_build;
	char : 8;
	char : 8;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	dword string_id_count;
	dword string_id_buffer_size;
	dword string_id_indices_offset;
	dword string_id_buffer_offset;
	long external_dependencies;
	long high_date_time;
	long low_date_time;
	long ui_high_date_time;
	long ui_low_date_time;
	long shared_high_date_time;
	long shared_low_date_time;
	long campaign_high_date_time;
	long campaign_low_date_time;
	tag_string name;
	long : 32;
	long_string scenario_name;
	dword minor_version;
	dword tag_names_count;
	dword tag_names_buffer_offset;
	dword tag_names_buffer_size;
	dword tag_names_indices_offset;
	dword checksum;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	dword base_address;
	long xdk_version;
	cache_partition partitions[NUMBER_OF_CACHE_PARTITIONS];
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long sha1_a[5];
	long sha1_b[5];
	long sha1_c[5];
	long rsa[64];
	cache_interop interop;
	long guid[4];
	short : 16;
	short : 16;
	long : 32;
	long compression_guid[4];
	byte elements1[0x2300];
	byte elements2[0x708];
	byte elements3[0x12C];
	long : 32;
	tag footer_signature;
} cache_header_gen3;

typedef struct cache_tag_header_gen3
{
	dword group_tags_offset;
	long group_tags_count;
	long count;
	dword tags_offset;
	long dependent_tags_count;
	dword dependent_tags_offset;
	long dependent_tags_count2;
	dword dependent_tags_offset2;
	dword checksum;
	tag signature;
} cache_tag_header_gen3;

typedef struct cache_tag_instance_gen3
{
	word group_index;
	word identifier;
	dword offset;
} cache_tag_instance_gen3;

/* ---------- prototypes */

cache_file *cache_file_gen3_load(char const *path);
void cache_file_gen3_dispose(cache_file *file);
dword cache_file_gen3_get_base_address();
cache_tag_instance_gen3 *cache_file_gen3_get_tag_instance(cache_file *file, long index);

long cache_header_gen3_get_file_length(cache_header_gen3 *header);
long cache_header_gen3_get_tag_header_offset(cache_header_gen3 *header);
long cache_header_gen3_get_tag_buffer_size(cache_header_gen3 *header);
char const *cache_header_gen3_get_name(cache_header_gen3 *header);
char const *cache_header_gen3_get_build(cache_header_gen3 *header);
cache_type cache_header_gen3_get_type(cache_header_gen3 *header);
cache_shared_type cache_header_gen3_get_shared_type(cache_header_gen3 *header);

long cache_tag_header_gen3_get_tag_count(cache_tag_header_gen3 *header);
dword cache_tag_header_gen3_get_tags_offset(cache_tag_header_gen3 *header);

tag cache_tag_instance_gen3_get_group_tag(cache_file *file, cache_tag_instance_gen3 *instance);
long cache_tag_instance_gen3_get_index(cache_file *file, cache_tag_instance_gen3 *instance);
dword cache_tag_instance_gen3_get_name_offset(cache_file *file, cache_tag_instance_gen3 *instance);
dword cache_tag_instance_gen3_get_offset(cache_file *file, cache_tag_instance_gen3 *instance);
