
/* ---------- headers */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <cseries/cseries.h>
#include <cache/cache_files.h>
#include <cache/cache_files_gen1.h>
#include <tag_files/tag_groups.h>

/* ---------- definitions */

long const cache_header_gen1_unused_length = 1936;

TAG_STRUCT(cache_header_gen1_struct, sizeof(cache_header_gen1))
{
	{ _field_tag, "header_signature" },
	{ _field_long_enum, "version" },
	{ _field_long_integer, "file_length" },
	{ _field_long_integer, "compressed_file_length" },
	{ _field_long_integer, "offset_to_index" },
	{ _field_long_integer, "tag_buffer_size" },
	{ _field_long_integer, "" },
	{ _field_long_integer, "" },
	{ _field_string, "name" },
	{ _field_string, "build" },
	{ _field_long_enum, "type" },
	{ _field_dword_integer, "crc" },
	{ _field_long_integer, "" },
	{ _field_padding, "", &cache_header_gen1_unused_length },
	{ _field_tag, "footer_signature" },
	{ _field_terminator }
};

TAG_STRUCT(cache_tag_header_gen1_struct, sizeof(cache_tag_header_gen1))
{
	{ _field_dword_integer, "tags_offset" },
	{ _field_long_integer, "scenario_index" },
	{ _field_dword_integer, "checksum" },
	{ _field_long_integer, "count" },
	{ _field_long_integer, "vertices_offset" },
	{ _field_long_integer, "vertex_count" },
	{ _field_long_integer, "indices_offset" },
	{ _field_long_integer, "index_count" },
	{ _field_long_integer, "model_data_size" },
	{ _field_tag, "signature" },
	{ _field_terminator }
};

TAG_ARRAY(_field_tag, group_tags_array_gen1, 3);

TAG_STRUCT(cache_tag_instance_gen1_struct, sizeof(cache_tag_instance_gen1))
{
	{ _field_array, "group_tags", &group_tags_array_gen1 },
	{ _field_long_integer, "index" },
	{ _field_dword_integer, "name_offset" },
	{ _field_dword_integer, "offset" },
	{ _field_byte_integer, "external" },
	{ _field_char_integer, "" },
	{ _field_short_integer, "" },
	{ _field_long_integer, "" },
	{ _field_terminator }
};

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
	&cache_header_gen1_struct,
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
	&cache_tag_header_gen1_struct,
	cache_tag_header_gen1_get_tag_count,
	cache_tag_header_gen1_get_tags_offset
};

cache_tag_instance_definition cache_tag_instance_gen1_definition =
{
	&cache_tag_instance_gen1_struct,
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
	cache_tag_header_gen1 *tag_header = (char *)result + header->offset_to_index;
	
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
	cache_tag_header_gen1 *tag_header = (char *)file + header->offset_to_index;
	return &((cache_tag_instance_gen1 *)((char *)file + tag_header->tags_offset))[index];
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
