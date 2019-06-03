
/* ---------- headers */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <cseries/cseries.h>
#include <cache/cache_files.h>
#include <cache/cache_files_gen2.h>
#include <tag_files/tag_groups.h>

/* ---------- definitions */

extern enum_definition cache_version_enum;
extern enum_definition cache_type_enum;

long const cache_file_gen2_padding_length = 1320;

TAG_STRUCT(cache_header_gen2_struct, sizeof(cache_header_gen2))
{
	{ _field_tag, "header_signature" },
	{ _field_long_enum, "version", &cache_version_enum },
	{ _field_dword_integer, "file_length" },
	{ _field_dword_integer, "compressed_file_length" },
	{ _field_dword_integer, "offset_to_index" },
	{ _field_dword_integer, "index_stream_size" },
	{ _field_dword_integer, "tag_buffer_size" },
	{ _field_dword_integer, "total_stream_size" },
	{ _field_long_string, "source_file" },
	{ _field_string, "build" },
	{ _field_long_enum, "type", &cache_type_enum },
	{ _field_dword_integer, "crc" },
	{ _field_char_integer, "" },
	{ _field_byte_integer, "tracked_build" },
	{ _field_char_integer, "" },
	{ _field_char_integer, "" },
	{ _field_long_integer, "" },
	{ _field_long_integer, "" },
	{ _field_long_integer, "" },
	{ _field_long_integer, "" },
	{ _field_long_integer, "" },
	{ _field_dword_integer, "string_id_buffer_aligned_offset" },
	{ _field_dword_integer, "string_id_count" },
	{ _field_dword_integer, "string_id_buffer_size" },
	{ _field_dword_integer, "string_id_indices_offset" },
	{ _field_dword_integer, "string_id_buffer_offset" },
	{ _field_long_integer, "external_dependencies" },
	{ _field_long_integer, "high_date_time" },
	{ _field_long_integer, "low_date_time" },
	{ _field_long_integer, "ui_high_date_time" },
	{ _field_long_integer, "ui_low_date_time" },
	{ _field_long_integer, "shared_high_date_time" },
	{ _field_long_integer, "shared_low_date_time" },
	{ _field_long_integer, "campaign_high_date_time" },
	{ _field_long_integer, "campaign_low_date_time" },
	{ _field_string, "name" },
	{ _field_long_integer, ":" },
	{ _field_long_string, "scenario_name" },
	{ _field_dword_integer, "minor_version" },
	{ _field_dword_integer, "tag_names_count" },
	{ _field_dword_integer, "tag_names_buffer_offset" },
	{ _field_dword_integer, "tag_names_buffer_size" },
	{ _field_dword_integer, "tag_names_indices_offset" },
	{ _field_dword_integer, "checksum" },
	{ _field_padding, "", &cache_file_gen2_padding_length },
	{ _field_tag, "footer_signature" },
	{ _field_terminator }
};

TAG_STRUCT(cache_tag_header_gen2_struct, sizeof(cache_tag_header_gen2))
{
	{ _field_dword_integer, "group_tags_offset" },
	{ _field_long_integer, "group_tags_count" },
	{ _field_dword_integer, "tags_offset" },
	{ _field_long_integer, "scenario_index" },
	{ _field_long_integer, "globals_index" },
	{ _field_dword_integer, "checksum" },
	{ _field_long_integer, "count" },
	{ _field_tag, "signature" },
	{ _field_terminator }
};

TAG_STRUCT(cache_tag_instance_gen2_struct, sizeof(cache_tag_instance_gen2))
{
	{ _field_tag, "group_tag" },
	{ _field_long_integer, "index" },
	{ _field_dword_integer, "offset" },
	{ _field_dword_integer, "size" },
	{ _field_terminator }
};

/* ---------- globals */

cache_file_definition cache_file_gen2_definition =
{
	cache_file_gen2_load,
	cache_file_gen2_dispose,
	cache_file_gen2_get_base_address,
	cache_file_gen2_get_tag_instance
};

cache_header_definition cache_header_gen2_definition =
{
	&cache_header_gen2_struct,
	cache_header_gen2_get_file_length,
	cache_header_gen2_get_tag_header_offset,
	cache_header_gen2_get_tag_buffer_size,
	cache_header_gen2_get_name,
	cache_header_gen2_get_build,
	cache_header_gen2_get_type,
	cache_header_gen2_get_shared_type
};

cache_tag_header_definition cache_tag_header_gen2_definition =
{
	&cache_tag_header_gen2_struct,
	cache_tag_header_gen2_get_tag_count,
	cache_tag_header_gen2_get_tags_offset
};

cache_tag_instance_definition cache_tag_instance_gen2_definition =
{
	&cache_tag_instance_gen2_struct,
	cache_tag_instance_gen2_get_group_tag,
	cache_tag_instance_gen2_get_index,
	cache_tag_instance_gen2_get_name_offset,
	cache_tag_instance_gen2_get_offset
};

cache_strings_definition cache_strings_gen2_definition =
{
	cache_strings_gen2_get_string_count,
	cache_strings_gen2_get_string_offset
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
	cache_tag_header_gen2 *tag_header = (char *)result + header->offset_to_index;
	
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
	cache_tag_header_gen2 *tag_header = (char *)file + header->offset_to_index;
	return &((cache_tag_instance_gen2 *)((char *)file + tag_header->tags_offset))[index];
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
	
	long *name_offsets = (char *)file + header->tag_names_indices_offset;
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

dword cache_strings_gen2_get_string_count(
	cache_file *file)
{
	return ((cache_header_gen2 *)file)->string_id_count;
}

dword cache_strings_gen2_get_string_offset(
	cache_file *file,
	long index)
{
	cache_header_gen2 *header = file;
	
	long *string_offsets = (char *)file + header->string_id_indices_offset;
	long string_offset = string_offsets[index & 0xFFFF];
	
	if (string_offset == NONE)
		return NONE;

	return header->string_id_buffer_offset + string_offset;
}
