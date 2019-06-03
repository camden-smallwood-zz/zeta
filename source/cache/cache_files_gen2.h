#pragma once

/* ---------- types */

typedef struct cache_header_gen2
{
	tag header_signature;
	cache_version version;
	dword file_length;
	dword compressed_file_length;
	dword offset_to_index;
	dword index_stream_size;
	dword tag_buffer_size;
	dword total_stream_size;
	/*H2V:
	dword virtual_base_address;
	dword tag_dependency_graph_offset;
	dword tag_dependency_graph_size;*/
	long_string source_file;
	tag_string build;
	cache_type type;
	dword crc;
	char : 8;
	bool tracked_build;
	char : 8;
	char : 8;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	dword string_id_buffer_aligned_offset;
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
	/*H2V:
	dword language_pack_offset;
	dword language_pack_size;
	long secondary_sound_gestalt_index;
	dword fast_geometry_cache_block_offset;
	dword fast_geometry_cache_block_size;*/
	dword checksum;
	/*H2V:
	dword mopp_code_checksum;
	char unknown4[1284];*/
	char unknown4[1320];
	tag footer_signature;
} cache_header_gen2;

typedef struct cache_tag_header_gen2
{
	dword group_tags_offset;
	long group_tags_count;
	dword tags_offset;
	long scenario_index;
	long globals_index;
	dword checksum;
	long count;
	tag signature;
} cache_tag_header_gen2;

typedef struct cache_tag_instance_gen2
{
	tag group_tag;
	long index;
	dword offset;
	dword size;
} cache_tag_instance_gen2;

/* ---------- prototypes */

cache_file *cache_file_gen2_load(char const *path);
void cache_file_gen2_dispose(cache_file *file);
dword cache_file_gen2_get_base_address();
cache_tag_instance_gen2 *cache_file_gen2_get_tag_instance(cache_file *file, long index);

long cache_header_gen2_get_file_length(cache_header_gen2 *header);
long cache_header_gen2_get_tag_header_offset(cache_header_gen2 *header);
long cache_header_gen2_get_tag_buffer_size(cache_header_gen2 *header);
char const *cache_header_gen2_get_name(cache_header_gen2 *header);
char const *cache_header_gen2_get_build(cache_header_gen2 *header);
cache_type cache_header_gen2_get_type(cache_header_gen2 *header);
cache_shared_type cache_header_gen2_get_shared_type(cache_header_gen2 *header);

long cache_tag_header_gen2_get_tag_count(cache_tag_header_gen2 *header);
dword cache_tag_header_gen2_get_tags_offset(cache_tag_header_gen2 *header);

tag cache_tag_instance_gen2_get_group_tag(cache_file *file, cache_tag_instance_gen2 *instance);
long cache_tag_instance_gen2_get_index(cache_file *file, cache_tag_instance_gen2 *instance);
dword cache_tag_instance_gen2_get_name_offset(cache_file *file, cache_tag_instance_gen2 *instance);
dword cache_tag_instance_gen2_get_offset(cache_file *file, cache_tag_instance_gen2 *instance);

dword cache_strings_gen2_get_string_count(cache_file *file);
dword cache_strings_gen2_get_string_offset(cache_file *file, long index);
