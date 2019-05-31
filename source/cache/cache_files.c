
/* ---------- headers */

#include <cseries/cseries.h>
#include <cache/cache_files.h>

#include <stdio.h>
#include <stdlib.h>

/* ---------- types */

typedef struct cache_file_functions
{
    cache_file *(*load)(char const *path);
    void(*dispose)(cache_file *file);
} cache_file_functions;

typedef struct cache_header_functions
{
    long(*get_file_length)(cache_header *header);
    long(*get_tag_header_offset)(cache_header *header);
    long(*get_tag_buffer_size)(cache_header *header);
    char const *(*get_name)(cache_header *header);
    char const *(*get_build)(cache_header *header);
    cache_type(*get_type)(cache_header *header);
} cache_header_functions;

typedef struct cache_tag_header_functions
{
    long(*get_tag_count)(cache_tag_header *header);
    dword(*get_tags_offset)(cache_tag_header *header);
} cache_tag_header_functions;

typedef struct cache_tag_instance_functions
{
    tag(*get_group_tag)(cache_tag_instance *instance);
    long(*get_index)(cache_tag_instance *instance);
    char const *(*get_name)(cache_tag_instance *instance);
    dword(*get_offset)(cache_tag_instance *instance);
} cache_tag_instance_functions;

typedef struct cache_strings_functions
{
    char const *(*get_string)(cache_strings *strings, long index);
} cache_strings_functions;

/* ---------- prototypes */

cache_file_functions *cache_file_functions_get(cache_version version);
cache_header_functions *cache_header_functions_get(cache_version version);
cache_tag_header_functions *cache_tag_header_functions_get(cache_version version);
cache_tag_instance_functions *cache_tag_instance_functions_get(cache_version version);
cache_strings_functions *cache_strings_functions_get(cache_version version);

/* ---------- public code */

cache_file *cache_file_load(
    char const *path)
{
    cache_version version = _cache_version_unknown;
    
    FILE *stream = fopen(path, "rb");
    fseek(stream, 4, SEEK_SET);
    fread(&version, sizeof(long), 1, stream);
    fclose(stream);

    cache_file_functions *functions = cache_file_functions_get(version);

    if (functions && functions->load)
        return functions->load(path);
    
    return 0;
}

void cache_file_dispose(
    cache_file *file)
{
    cache_version version = cache_file_get_version(file);
    cache_file_functions *functions = cache_file_functions_get(version);

    if (functions && functions->dispose)
        functions->dispose(file);
}

cache_version cache_file_get_version(
    cache_file *file)
{
    return *(cache_version *)(file + 4);
}

long cache_file_get_file_length(
    cache_file *file)
{
    cache_version version = cache_file_get_version(file);
    cache_header_functions *functions = cache_header_functions_get(version);
    
    if (functions && functions->get_file_length)
        return functions->get_file_length(file);
    
    return NONE;
}

long cache_file_get_tag_header_offset(
    cache_file *file)
{
    cache_version version = cache_file_get_version(file);
    cache_header_functions *functions = cache_header_functions_get(version);
    
    if (functions && functions->get_tag_header_offset)
        return functions->get_tag_header_offset(file);
    
    return NONE;
}

long cache_file_get_tag_buffer_size(
    cache_file *file)
{
    cache_version version = cache_file_get_version(file);
    cache_header_functions *functions = cache_header_functions_get(version);
    
    if (functions && functions->get_tag_buffer_size)
        return functions->get_tag_buffer_size(file);
    
    return NONE;
}

char const *cache_file_get_name(
    cache_file *file)
{
    cache_version version = cache_file_get_version(file);
    cache_header_functions *functions = cache_header_functions_get(version);
    
    if (functions && functions->get_name)
        return functions->get_name(file);
    
    return 0;
}

char const *cache_file_get_build(
    cache_file *file)
{
    cache_version version = cache_file_get_version(file);
    cache_header_functions *functions = cache_header_functions_get(version);
    
    if (functions && functions->get_build)
        return functions->get_build(file);
    
    return 0;
}

cache_type cache_file_get_type(
    cache_file *file)
{
    cache_version version = cache_file_get_version(file);
    cache_header_functions *functions = cache_header_functions_get(version);
    
    if (functions && functions->get_type)
        return functions->get_type(file);
    
    return _cache_type_none;
}

long cache_file_get_tag_count(
    cache_file *file)
{
    cache_version version = cache_file_get_version(file);
    cache_tag_header_functions *functions = cache_tag_header_functions_get(version);
    
    if (functions && functions->get_tag_count)
        return functions->get_tag_count(file);
    
    return 0;
}

dword cache_file_get_tags_offset(
    cache_file *file)
{
    cache_version version = cache_file_get_version(file);
    cache_tag_header_functions *functions = cache_tag_header_functions_get(version);
    
    if (functions && functions->get_tags_offset)
        return functions->get_tags_offset(file);
    
    return 0;
}

cache_tag_instance *cache_file_get_tag_instance(
    cache_file *file,
    long index)
{
    // TODO
    return 0;
}

char const *cache_file_get_string(
    cache_file *file,
    long index)
{
    cache_version version = cache_file_get_version(file);
    cache_strings_functions *functions = cache_strings_functions_get(version);
    
    if (functions && functions->get_string)
        return functions->get_string(file, index);
    
    return 0;
}

/* ---------- private code */

cache_header_functions *cache_header_functions_get(
    cache_version version)
{
    switch (version)
    {
    default:
        return 0;
    }
}

cache_tag_header_functions *cache_tag_header_functions_get(
    cache_version version)
{
    switch (version)
    {
    default:
        return 0;
    }
}

cache_tag_instance_functions *cache_tag_instance_functions_get(
    cache_version version)
{
    switch (version)
    {
    default:
        return 0;
    }
}

cache_strings_functions *cache_strings_functions_get(
    cache_version version)
{
    switch (version)
    {
    default:
        return 0;
    }
}

cache_file_functions *cache_file_functions_get(
    cache_version version)
{
    switch (version)
    {
    default:
        return 0;
    }
}
