
/* ---------- headers */

#include <cseries/cseries.h>
#include <cache/cache_files.h>

#include <stdio.h>

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

dword cache_file_get_base_address(
    cache_file *file)
{
    cache_version version = cache_file_get_version(file);
    cache_file_functions *functions = cache_file_functions_get(version);

    if (functions && functions->get_base_address)
        return functions->get_base_address(file);
    
    return 0;
}

cache_tag_instance *cache_file_get_tag_instance(
    cache_file *file,
    long index)
{
    cache_version version = cache_file_get_version(file);
    cache_file_functions *functions = cache_file_functions_get(version);

    if (functions && functions->get_tag_instance)
        return functions->get_tag_instance(file, index);
    
    return 0;
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
    cache_tag_header *header = file + cache_file_get_tag_header_offset(file);
    cache_tag_header_functions *functions = cache_tag_header_functions_get(version);
    
    if (functions && functions->get_tag_count)
        return functions->get_tag_count(header);
    
    return 0;
}

dword cache_file_get_tags_offset(
    cache_file *file)
{
    cache_version version = cache_file_get_version(file);
    cache_tag_header *header = file + cache_file_get_tag_header_offset(file);
    cache_tag_header_functions *functions = cache_tag_header_functions_get(version);
    
    if (functions && functions->get_tags_offset)
        return functions->get_tags_offset(header);
    
    return 0;
}

tag cache_file_get_tag_group_tag(cache_file *instance);
char const *cache_file_get_tag_name(cache_file *instance);
dword cache_file_get_tag_offset(cache_file *instance);

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

cache_file_functions *cache_file_functions_get(
    cache_version version)
{
    extern cache_file_functions cache_file_gen1_functions;

    switch (version)
    {
    case _cache_version_gen1:
        return &cache_file_gen1_functions;
    default:
        return 0;
    }
}

cache_header_functions *cache_header_functions_get(
    cache_version version)
{
    extern cache_header_functions cache_header_gen1_functions;

    switch (version)
    {
    case _cache_version_gen1:
        return &cache_header_gen1_functions;
    default:
        return 0;
    }
}

cache_tag_header_functions *cache_tag_header_functions_get(
    cache_version version)
{
    extern cache_tag_header_functions cache_tag_header_gen1_functions;

    switch (version)
    {
    case _cache_version_gen1:
        return &cache_tag_header_gen1_functions;
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
