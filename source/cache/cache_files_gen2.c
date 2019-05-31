
/* ---------- headers */

#include <cseries/cseries.h>
#include <cache/cache_files.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
_Static_assert(sizeof(cache_header_gen2) == 0x800, "invalid cache_header_gen2 size");

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

/* ---------- globals */

cache_file_functions cache_file_gen2_functions =
{
    cache_file_gen2_load,
    cache_file_gen2_dispose,
    cache_file_gen2_get_base_address,
    cache_file_gen2_get_tag_instance
};

cache_header_functions cache_header_gen2_functions =
{
    cache_header_gen2_get_file_length,
    cache_header_gen2_get_tag_header_offset,
    cache_header_gen2_get_tag_buffer_size,
    cache_header_gen2_get_name,
    cache_header_gen2_get_build,
    cache_header_gen2_get_type,
    cache_header_gen2_get_shared_type
};

cache_tag_header_functions cache_tag_header_gen2_functions =
{
    cache_tag_header_gen2_get_tag_count,
    cache_tag_header_gen2_get_tags_offset
};

cache_tag_instance_functions cache_tag_instance_gen2_functions =
{
    cache_tag_instance_gen2_get_group_tag,
    cache_tag_instance_gen2_get_index,
    cache_tag_instance_gen2_get_name_offset,
    cache_tag_instance_gen2_get_offset
};

/* ---------- code */

cache_file *cache_file_gen2_load(
    char const *path)
{
    FILE *stream = fopen(path, "rb");
    
    fseek(stream, 0, SEEK_END);
    long size = ftell(stream);

    cache_file *result = malloc(size);
    
    fseek(stream, 0, SEEK_SET);
    fread(result, size, 1, stream);

    cache_header_gen2 *header = result;
    cache_tag_header_gen2 *tag_header = result + header->offset_to_index;
    
    dword base_address = cache_file_gen2_get_base_address(result);
    dword address_mask = base_address - header->offset_to_index;

    tag_header->group_tags_offset -= address_mask;
    tag_header->tags_offset -= address_mask;

    cache_tag_instance_gen2 *first_instance = cache_file_gen2_get_tag_instance(result, 0);
    address_mask = first_instance->offset - (header->offset_to_index + header->index_stream_size);

    for (int i = 0; i < tag_header->count; i++)
    {
        cache_tag_instance_gen2 *instance = cache_file_gen2_get_tag_instance(result, i);
        instance->offset -= address_mask;
    }

    return result;
}

void cache_file_gen2_dispose(
    cache_file *file)
{
    free(file);
}

dword cache_file_gen2_get_base_address()
{
    return 0x80061000;
}

cache_tag_instance_gen2 *cache_file_gen2_get_tag_instance(
    cache_file *file,
    long index)
{
    cache_header_gen2 *header = file;
    cache_tag_header_gen2 *tag_header = file + header->offset_to_index;
    return &((cache_tag_instance_gen2 *)(file + tag_header->tags_offset))[index];
}

long cache_header_gen2_get_file_length(
    cache_header_gen2 *header)
{
    return header->file_length;
}

long cache_header_gen2_get_tag_header_offset(
    cache_header_gen2 *header)
{
    return header->offset_to_index;
}

long cache_header_gen2_get_tag_buffer_size(
    cache_header_gen2 *header)
{
    return header->tag_buffer_size;
}

char const *cache_header_gen2_get_name(
    cache_header_gen2 *header)
{
    return header->name;
}

char const *cache_header_gen2_get_build(
    cache_header_gen2 *header)
{
    return header->build;
}

cache_type cache_header_gen2_get_type(
    cache_header_gen2 *header)
{
    return header->type;
}

cache_shared_type cache_header_gen2_get_shared_type(
    cache_header_gen2 *header)
{
    return _cache_shared_type_none;
}

long cache_tag_header_gen2_get_tag_count(
    cache_tag_header_gen2 *header)
{
    return header->count;
}

dword cache_tag_header_gen2_get_tags_offset(
    cache_tag_header_gen2 *header)
{
    return header->tags_offset;
}

tag cache_tag_instance_gen2_get_group_tag(
    cache_file *file,
    cache_tag_instance_gen2 *instance)
{
    return instance->group_tag;
}

long cache_tag_instance_gen2_get_index(
    cache_file *file,
    cache_tag_instance_gen2 *instance)
{
    return instance->index;
}

dword cache_tag_instance_gen2_get_name_offset(
    cache_file *file,
    cache_tag_instance_gen2 *instance)
{
    cache_header_gen2 *header = file;
    
    long *name_offsets = file + header->tag_names_indices_offset;
    long name_offset = name_offsets[instance->index & 0xFFFF];
    
    if (name_offset == NONE)
        return NONE;

    return header->tag_names_buffer_offset + name_offset;
}

dword cache_tag_instance_gen2_get_offset(
    cache_file *file,
    cache_tag_instance_gen2 *instance)
{
    return instance->offset;
}
