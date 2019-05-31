
/* ---------- headers */

#include <cseries/cseries.h>
#include <cache/cache_files.h>

#include <stdio.h>
#include <stdlib.h>

/* ---------- types */

typedef struct cache_header_gen1
{
    tag header_signature;
    cache_version version;
    long file_length;
    long compressed_file_length;
    long offset_to_index;
    long tag_memory_size;
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

typedef struct cache_tag_instance_gen1
{
    tag group_tags[3];
    long index;
    char const *name;
    dword offset;
    boolean external;
    long : 32;
} cache_tag_instance_gen1;

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

/* ---------- prototypes */

cache_file *cache_file_gen1_load(char const *path);
void cache_file_gen1_dispose(cache_file *file);

long cache_header_gen1_get_file_length(cache_header_gen1 *header);
long cache_header_gen1_get_tag_header_offset(cache_header_gen1 *header);
long cache_header_gen1_get_tag_buffer_size(cache_header_gen1 *header);
char const *cache_header_gen1_get_name(cache_header_gen1 *header);
char const *cache_header_gen1_get_build(cache_header_gen1 *header);
cache_type cache_header_gen1_get_type(cache_header_gen1 *header);

/* ---------- globals */

cache_file_functions cache_file_gen1_functions =
{
    cache_file_gen1_load,
    cache_file_gen1_dispose
};

cache_header_functions cache_header_gen1_functions =
{
    cache_header_gen1_get_file_length,
    cache_header_gen1_get_tag_header_offset,
    cache_header_gen1_get_tag_buffer_size,
    cache_header_gen1_get_name,
    cache_header_gen1_get_build,
    cache_header_gen1_get_type
};

/* ---------- code */

cache_file *cache_file_gen1_load(
    char const *path)
{
    FILE *stream = fopen(path, "rb");
    fseek(stream, 0, SEEK_END);
    long size = ftell(stream);
    cache_file *result = malloc(size);
    fseek(stream, 0, SEEK_SET);
    fread(result, size, 1, stream);
    return result;
}

void cache_file_gen1_dispose(
    cache_file *file)
{
    free(file);
}

long cache_header_gen1_get_file_length(
    cache_header_gen1 *header)
{
    return header->file_length;
}

long cache_header_gen1_get_tag_header_offset(
    cache_header_gen1 *header)
{
    return header->offset_to_index;
}

long cache_header_gen1_get_tag_buffer_size(
    cache_header_gen1 *header)
{
    return header->tag_memory_size;
}

char const *cache_header_gen1_get_name(
    cache_header_gen1 *header)
{
    return header->name;
}

char const *cache_header_gen1_get_build(
    cache_header_gen1 *header)
{
    return header->build;
}

cache_type cache_header_gen1_get_type(
    cache_header_gen1 *header)
{
    return header->type;
}
