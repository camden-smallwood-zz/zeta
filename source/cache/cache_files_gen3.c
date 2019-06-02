
/* ---------- headers */

#include <cseries/cseries.h>
#include <cache/cache_files.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
    _cache_section_type_resource,
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
_Static_assert(sizeof(cache_header_gen3) == 0x3000, "invalid cache_header_gen3 size");

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
void cache_header_gen3_byteswap(cache_header_gen3 *header);

long cache_tag_header_gen3_get_tag_count(cache_tag_header_gen3 *header);
dword cache_tag_header_gen3_get_tags_offset(cache_tag_header_gen3 *header);
void cache_tag_header_gen3_byteswap(cache_tag_header_gen3 *header);

tag cache_tag_instance_gen3_get_group_tag(cache_file *file, cache_tag_instance_gen3 *instance);
long cache_tag_instance_gen3_get_index(cache_file *file, cache_tag_instance_gen3 *instance);
dword cache_tag_instance_gen3_get_name_offset(cache_file *file, cache_tag_instance_gen3 *instance);
dword cache_tag_instance_gen3_get_offset(cache_file *file, cache_tag_instance_gen3 *instance);
void cache_tag_instance_gen3_byteswap(cache_tag_instance_gen3 *instance);

/* ---------- globals */

cache_file_functions cache_file_gen3_functions =
{
    cache_file_gen3_load,
    cache_file_gen3_dispose,
    cache_file_gen3_get_base_address,
    cache_file_gen3_get_tag_instance
};

cache_header_functions cache_header_gen3_functions =
{
    cache_header_gen3_get_file_length,
    cache_header_gen3_get_tag_header_offset,
    cache_header_gen3_get_tag_buffer_size,
    cache_header_gen3_get_name,
    cache_header_gen3_get_build,
    cache_header_gen3_get_type,
    cache_header_gen3_get_shared_type
};

cache_tag_header_functions cache_tag_header_gen3_functions =
{
    cache_tag_header_gen3_get_tag_count,
    cache_tag_header_gen3_get_tags_offset
};

cache_tag_instance_functions cache_tag_instance_gen3_functions =
{
    cache_tag_instance_gen3_get_group_tag,
    cache_tag_instance_gen3_get_index,
    cache_tag_instance_gen3_get_name_offset,
    cache_tag_instance_gen3_get_offset
};

/* ---------- code */

cache_file *cache_file_gen3_load(
    char const *path)
{
    FILE *stream = fopen(path, "rb");
    
    fseek(stream, 0, SEEK_END);
    dword size = ftell(stream);

    cache_file *result = malloc(size);
    
    fseek(stream, 0, SEEK_SET);
    fread(result, size, 1, stream);

    cache_header_gen3 *header = result;
    cache_header_gen3_byteswap(header);

    dword address_mask = 0;

    if (header->interop.resource_base_address == 0)
    {
        address_mask = (long)(header->interop.runtime_base_address - header->tag_buffer_size);
    }
    else
    {
        long value = header->string_id_indices_offset - sizeof(cache_header_gen3);

        header->index_stream_size -= value;
        header->string_id_indices_offset -= value;
        header->string_id_buffer_offset -= value;
        header->tag_names_buffer_offset -= value;
        header->tag_names_indices_offset -= value;

        address_mask = *(long *)&header->partitions[_cache_partition_type_resources].base_address - (*(long *)&header->interop.debug_section_size + *(long *)&header->interop.sections[_cache_section_type_resource].size);
    }

    if (header->type != _cache_type_shared && header->type != _cache_type_shared_campaign)
    {
        //
        // TODO: fix this
        //

        header->offset_to_index -= address_mask;
     
        cache_tag_header_gen3 *tag_header = result + header->offset_to_index;
        cache_tag_header_gen3_byteswap(tag_header);
     
        tag_header->group_tags_offset -= address_mask;
        tag_header->tags_offset -= address_mask;
        tag_header->dependent_tags_offset -= address_mask;
        
        for (int i = 0; i < tag_header->count; i++)
        {
            cache_tag_instance_gen3 *instance = cache_file_gen3_get_tag_instance(result, i);
            cache_tag_instance_gen3_byteswap(instance);

            instance->offset -= address_mask;
        }
    }

    return result;
}

void cache_file_gen3_dispose(
    cache_file *file)
{
    free(file);
}

dword cache_file_gen3_get_base_address()
{
    return 0xA0000000;
}

cache_tag_instance_gen3 *cache_file_gen3_get_tag_instance(
    cache_file *file,
    long index)
{
    cache_header_gen3 *header = file;
    cache_tag_header_gen3 *tag_header = file + header->offset_to_index;
    return &((cache_tag_instance_gen3 *)(file + tag_header->tags_offset))[index];
}

void cache_header_gen3_byteswap(
    cache_header_gen3 *header)
{
    int i;
    header->header_signature = _byteswap_ulong(header->header_signature);
    header->version = _byteswap_ulong(header->version);
    header->file_length = _byteswap_ulong(header->file_length);
    header->compressed_file_length = _byteswap_ulong(header->compressed_file_length);
    header->offset_to_index = _byteswap_ulong(header->offset_to_index);
    header->index_stream_size = _byteswap_ulong(header->index_stream_size);
    header->tag_buffer_size = _byteswap_ulong(header->tag_buffer_size);
    header->type = _byteswap_ushort(header->type);
    header->shared_type = _byteswap_ushort(header->shared_type);
    header->string_id_count = _byteswap_ulong(header->string_id_count);
    header->string_id_buffer_size = _byteswap_ulong(header->string_id_buffer_size);
    header->string_id_indices_offset = _byteswap_ulong(header->string_id_indices_offset);
    header->string_id_buffer_offset = _byteswap_ulong(header->string_id_buffer_offset);
    header->external_dependencies = _byteswap_ulong(header->external_dependencies);
    header->high_date_time = _byteswap_ulong(header->high_date_time);
    header->low_date_time = _byteswap_ulong(header->low_date_time);
    header->ui_high_date_time = _byteswap_ulong(header->ui_high_date_time);
    header->ui_low_date_time = _byteswap_ulong(header->ui_low_date_time);
    header->shared_high_date_time = _byteswap_ulong(header->shared_high_date_time);
    header->shared_low_date_time = _byteswap_ulong(header->shared_low_date_time);
    header->campaign_high_date_time = _byteswap_ulong(header->campaign_high_date_time);
    header->campaign_low_date_time = _byteswap_ulong(header->campaign_low_date_time);
    header->minor_version = _byteswap_ulong(header->minor_version);
    header->tag_names_count = _byteswap_ulong(header->tag_names_count);
    header->tag_names_buffer_offset = _byteswap_ulong(header->tag_names_buffer_offset);
    header->tag_names_buffer_size = _byteswap_ulong(header->tag_names_buffer_size);
    header->tag_names_indices_offset = _byteswap_ulong(header->tag_names_indices_offset);
    header->checksum = _byteswap_ulong(header->checksum);
    header->base_address = _byteswap_ulong(header->base_address);
    header->xdk_version = _byteswap_ulong(header->xdk_version);
    for (i = 0; i < NUMBER_OF_CACHE_PARTITIONS; i++)
    {
        header->partitions[i].base_address = _byteswap_ulong(header->partitions[i].base_address);
        header->partitions[i].size = _byteswap_ulong(header->partitions[i].size);
    }
    for (i = 0; i < 5; i++)
        header->sha1_a[i] = _byteswap_ulong(header->sha1_a[i]);
    for (i = 0; i < 5; i++)
        header->sha1_b[i] = _byteswap_ulong(header->sha1_b[i]);
    for (i = 0; i < 5; i++)
        header->sha1_c[i] = _byteswap_ulong(header->sha1_c[i]);
    for (i = 0; i < 64; i++)
        header->rsa[i] = _byteswap_ulong(header->rsa[i]);
    header->interop.resource_base_address = _byteswap_ulong(header->interop.resource_base_address);
    header->interop.debug_section_size = _byteswap_ulong(header->interop.debug_section_size);
    header->interop.runtime_base_address = _byteswap_ulong(header->interop.runtime_base_address);
    header->interop.unknown_base_address = _byteswap_ulong(header->interop.unknown_base_address);
    for (i = 0; i < NUMBER_OF_CACHE_SECTIONS; i++)
    {
        header->interop.sections[i].virtual_address = _byteswap_ulong(header->interop.sections[i].virtual_address);
        header->interop.sections[i].size = _byteswap_ulong(header->interop.sections[i].size);
    }
    for (i = 0; i < 4; i++)
        header->guid[i] = _byteswap_ulong(header->guid[i]);
    for (i = 0; i < 4; i++)
        header->compression_guid[i] = _byteswap_ulong(header->compression_guid[i]);
    header->footer_signature = _byteswap_ulong(header->footer_signature);
}

long cache_header_gen3_get_file_length(
    cache_header_gen3 *header)
{
    return header->file_length;
}

long cache_header_gen3_get_tag_header_offset(
    cache_header_gen3 *header)
{
    return header->offset_to_index;
}

long cache_header_gen3_get_tag_buffer_size(
    cache_header_gen3 *header)
{
    return header->tag_buffer_size;
}

char const *cache_header_gen3_get_name(
    cache_header_gen3 *header)
{
    return header->name;
}

char const *cache_header_gen3_get_build(
    cache_header_gen3 *header)
{
    return header->build;
}

cache_type cache_header_gen3_get_type(
    cache_header_gen3 *header)
{
    return header->type;
}

cache_shared_type cache_header_gen3_get_shared_type(
    cache_header_gen3 *header)
{
    return _cache_shared_type_none;
}

long cache_tag_header_gen3_get_tag_count(
    cache_tag_header_gen3 *header)
{
    return header->count;
}

dword cache_tag_header_gen3_get_tags_offset(
    cache_tag_header_gen3 *header)
{
    return header->tags_offset;
}

void cache_tag_header_gen3_byteswap(
    cache_tag_header_gen3 *header)
{
    header->group_tags_offset = _byteswap_ulong(header->group_tags_offset);
    header->group_tags_count = _byteswap_ulong(header->group_tags_count);
    header->count = _byteswap_ulong(header->count);
    header->tags_offset = _byteswap_ulong(header->tags_offset);
    header->dependent_tags_count = _byteswap_ulong(header->dependent_tags_count);
    header->dependent_tags_offset = _byteswap_ulong(header->dependent_tags_offset);
    header->dependent_tags_count2 = _byteswap_ulong(header->dependent_tags_count2);
    header->dependent_tags_offset2 = _byteswap_ulong(header->dependent_tags_offset2);
    header->checksum = _byteswap_ulong(header->checksum);
    header->signature = _byteswap_ulong(header->signature);
}

tag cache_tag_instance_gen3_get_group_tag(
    cache_file *file,
    cache_tag_instance_gen3 *instance)
{
    if (instance->group_index == (word)NONE)
        return NONE;

    cache_tag_header_gen3 *tag_header = cache_header_gen3_get_tag_header_offset(file);
    return *(tag *)(file + tag_header->group_tags_offset + 12 * instance->group_index);
}

long cache_tag_instance_gen3_get_index(
    cache_file *file,
    cache_tag_instance_gen3 *instance)
{
    long tag_count = cache_file_get_tag_count(file);
    
    for (long i = 0; i < tag_count; i++)
    {
        cache_tag_instance_gen3 *current_instance = cache_file_gen3_get_tag_instance(file, i);

        if (instance->identifier == current_instance->identifier)
            return i | (current_instance->identifier << 16);
    }

    return NONE;
}

dword cache_tag_instance_gen3_get_name_offset(
    cache_file *file,
    cache_tag_instance_gen3 *instance)
{
    cache_header_gen3 *header = file;
    
    long *name_offsets = file + header->tag_names_indices_offset;
    long index = cache_tag_instance_gen3_get_index(file, instance);
    long name_offset = name_offsets[index & 0xFFFF];
    
    if (name_offset == NONE)
        return NONE;

    return header->tag_names_buffer_offset + name_offset;
}

dword cache_tag_instance_gen3_get_offset(
    cache_file *file,
    cache_tag_instance_gen3 *instance)
{
    return instance->offset;
}

void cache_tag_instance_gen3_byteswap(
    cache_tag_instance_gen3 *instance)
{
    instance->identifier = _byteswap_ushort(instance->identifier);
    instance->group_index = _byteswap_ushort(instance->group_index);
    instance->offset = _byteswap_ulong(instance->offset);
}
