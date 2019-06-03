#pragma once

/* ---------- types */

typedef void cache_file;
typedef void cache_header;
typedef void cache_tag_header;
typedef void cache_tag_instance;
typedef void cache_strings;

typedef enum cache_version
{
	_cache_version_unknown,
	_cache_version_gen1= 7,
	_cache_version_gen1_ce= 609,
	_cache_version_gen2= 8,
	_cache_version_gen3= 11,
	_cache_version_gen4= 12,
	NUMBER_OF_CACHE_VERSIONS= 6
} cache_version;

typedef enum cache_type
{
	_cache_type_none= NONE,
	_cache_type_campaign,
	_cache_type_multiplayer,
	_cache_type_mainmenu,
	_cache_type_shared,
	_cache_type_shared_campaign,
	_cache_type_unused5,
	_cache_type_unused6,
	NUMBER_OF_CACHE_TYPES
} cache_type;

typedef enum cache_shared_type
{
	_cache_shared_type_none= NONE,
	_cache_shared_type_ui,
	_cache_shared_type_shared,
	_cache_shared_type_campaign,
	NUMBER_OF_CACHE_SHARED_TYPES
} cache_shared_type;

typedef struct cache_file_definition
{
	cache_file *(*load)(char const *path);
	void(*dispose)(cache_file *file);
	dword(*get_base_address)();
	cache_tag_instance *(*get_tag_instance)(cache_file *file, long index);
} cache_file_definition;

typedef struct cache_header_definition
{
	long(*get_file_length)(cache_header *header);
	long(*get_tag_header_offset)(cache_header *header);
	long(*get_tag_buffer_size)(cache_header *header);
	char const *(*get_name)(cache_header *header);
	char const *(*get_build)(cache_header *header);
	cache_type(*get_type)(cache_header *header);
	cache_shared_type(*get_shared_type)(cache_header *header);
} cache_header_definition;

typedef struct cache_tag_header_definition
{
	long(*get_tag_count)(cache_tag_header *header);
	dword(*get_tags_offset)(cache_tag_header *header);
} cache_tag_header_definition;

typedef struct cache_tag_instance_definition
{
	tag(*get_group_tag)(cache_file *file, cache_tag_instance *instance);
	long(*get_index)(cache_file *file, cache_tag_instance *instance);
	dword(*get_name_offset)(cache_file *file, cache_tag_instance *instance);
	dword(*get_offset)(cache_file *file, cache_tag_instance *instance);
} cache_tag_instance_definition;

typedef struct cache_strings_definition
{
	char const *(*get_string)(cache_strings *strings, long index);
} cache_strings_definition;

/* ---------- prototypes/CACHE_FILES.C */

cache_file_definition *cache_file_definition_get(cache_version version);
cache_header_definition *cache_header_definition_get(cache_version version);
cache_tag_header_definition *cache_tag_header_definition_get(cache_version version);
cache_tag_instance_definition *cache_tag_instance_definition_get(cache_version version);
cache_strings_definition *cache_strings_definition_get(cache_version version);

cache_file *cache_file_load(char const *path);
void cache_file_dispose(cache_file *file);
dword cache_file_get_base_address(cache_file *file);
cache_tag_instance *cache_file_get_tag_instance(cache_file *file, long index);
cache_version cache_file_get_version(cache_file *file);
long cache_file_get_file_length(cache_file *file);
long cache_file_get_tag_header_offset(cache_file *file);
long cache_file_get_tag_buffer_size(cache_file *file);
char const *cache_file_get_name(cache_file *file);
char const *cache_file_get_build(cache_file *file);
cache_type cache_file_get_type(cache_file *file);
cache_shared_type cache_file_get_shared_type(cache_file *file);
long cache_file_get_tag_count(cache_file *file);
dword cache_file_get_tags_offset(cache_file *file);
tag cache_file_get_tag_group_tag(cache_file *file, long index);
char const *cache_file_get_tag_name(cache_file *file, long index);
dword cache_file_get_tag_offset(cache_file *file, long index);
char const *cache_file_get_string(cache_file *file, long index);
