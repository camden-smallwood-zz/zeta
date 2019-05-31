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

/* ---------- prototypes/CACHE_FILES.C */

cache_file *cache_file_load(char const *path);
void cache_file_dispose(cache_file *file);
cache_version cache_file_get_version(cache_file *file);
long cache_file_get_file_length(cache_file *file);
long cache_file_get_tag_header_offset(cache_file *file);
long cache_file_get_tag_buffer_size(cache_file *file);
char const *cache_file_get_name(cache_file *file);
char const *cache_file_get_build(cache_file *file);
cache_type cache_file_get_type(cache_file *file);
long cache_file_get_tag_count(cache_file *file);
dword cache_file_get_tags_offset(cache_file *file);
cache_tag_instance *cache_file_get_tag_instance(cache_file *file, long index);
char const *cache_file_get_string(cache_file *file, long index);
