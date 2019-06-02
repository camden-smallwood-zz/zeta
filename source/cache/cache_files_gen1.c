
/* ---------- headers */

#include <cseries/cseries.h>
#include <cache/cache_files.h>

#include <stdbool.h>
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

/* ---------- globals */

cache_file_definition cache_file_gen1_definition =
{
    cache_file_gen1_load,
    cache_file_gen1_dispose,
    cache_file_gen1_get_base_address,
    cache_file_gen1_get_tag_instance
};

cache_header_definition cache_header_gen1_definition =
{
    cache_header_gen1_get_file_length,
    cache_header_gen1_get_tag_header_offset,
    cache_header_gen1_get_tag_buffer_size,
    cache_header_gen1_get_name,
    cache_header_gen1_get_build,
    cache_header_gen1_get_type,
    cache_header_gen1_get_shared_type
};

cache_tag_header_definition cache_tag_header_gen1_definition =
{
    cache_tag_header_gen1_get_tag_count,
    cache_tag_header_gen1_get_tags_offset
};

cache_tag_instance_definition cache_tag_instance_gen1_definition =
{
    cache_tag_instance_gen1_get_group_tag,
    cache_tag_instance_gen1_get_index,
    cache_tag_instance_gen1_get_name_offset,
    cache_tag_instance_gen1_get_offset
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

    cache_header_gen1 *header = result;
    cache_tag_header_gen1 *tag_header = result + header->offset_to_index;
    
    dword base_address = cache_file_gen1_get_base_address(result);
    dword address_mask = base_address - header->offset_to_index;

    tag_header->tags_offset -= address_mask;
    tag_header->indices_offset += tag_header->vertices_offset;

    for (int i = 0; i < tag_header->count; i++)
    {
        cache_tag_instance_gen1 *instance = cache_file_gen1_get_tag_instance(result, i);

        instance->name_offset -= address_mask;
        instance->offset -= address_mask;
    }

    return result;
}

void cache_file_gen1_dispose(
    cache_file *file)
{
    free(file);
}

dword cache_file_gen1_get_base_address()
{
    return 0x40440000;
}

cache_tag_instance_gen1 *cache_file_gen1_get_tag_instance(
    cache_file *file,
    long index)
{
    cache_header_gen1 *header = file;
    cache_tag_header_gen1 *tag_header = file + header->offset_to_index;
    return &((cache_tag_instance_gen1 *)(file + tag_header->tags_offset))[index];
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
    return header->tag_buffer_size;
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

cache_shared_type cache_header_gen1_get_shared_type(
    cache_header_gen1 *header)
{
    return _cache_shared_type_none;
}

long cache_tag_header_gen1_get_tag_count(
    cache_tag_header_gen1 *header)
{
    return header->count;
}

dword cache_tag_header_gen1_get_tags_offset(
    cache_tag_header_gen1 *header)
{
    return header->tags_offset;
}

tag cache_tag_instance_gen1_get_group_tag(
    cache_file *file,
    cache_tag_instance_gen1 *instance)
{
    return instance->group_tags[0];
}

long cache_tag_instance_gen1_get_index(
    cache_file *file,
    cache_tag_instance_gen1 *instance)
{
    return instance->index;
}

dword cache_tag_instance_gen1_get_name_offset(
    cache_file *file,
    cache_tag_instance_gen1 *instance)
{
    return instance->name_offset;
}

dword cache_tag_instance_gen1_get_offset(
    cache_file *file,
    cache_tag_instance_gen1 *instance)
{
    return instance->offset;
}
