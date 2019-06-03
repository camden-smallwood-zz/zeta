
/* ---------- headers */

#include <stdio.h>
#include <stdlib.h>

#include <cseries/cseries.h>
#include <cache/cache_files.h>
#include <tag_files/tag_groups.h>

/* ---------- definitions */

TAG_ENUM(cache_version_enum, NUMBER_OF_CACHE_VERSIONS)
{
	{ "unknown", _cache_version_unknown },
	{ "gen1", _cache_version_gen1 },
	{ "gen1_ce", _cache_version_gen1_ce },
	{ "gen2", _cache_version_gen2 },
	{ "gen3", _cache_version_gen3 },
	{ "gen4", _cache_version_gen4 }
};

TAG_ENUM(cache_type_enum, NUMBER_OF_CACHE_TYPES + 1)
{
	{ "none", _cache_type_none },
	{ "campaign", _cache_type_campaign },
	{ "multiplayer", _cache_type_multiplayer },
	{ "mainmenu", _cache_type_mainmenu },
	{ "shared", _cache_type_shared },
	{ "shared_campaign", _cache_type_shared_campaign },
	{ "unused5", _cache_type_unused5 },
	{ "unused6", _cache_type_unused6 }
};

TAG_ENUM(cache_shared_type_enum, NUMBER_OF_CACHE_SHARED_TYPES + 1)
{
	{ "none", _cache_shared_type_none },
	{ "ui", _cache_shared_type_ui },
	{ "shared", _cache_shared_type_shared },
	{ "campaign", _cache_shared_type_campaign }
};

/* ---------- code */

cache_file_definition *cache_file_definition_get(
	cache_version version)
{
	extern cache_file_definition
		cache_file_gen1_definition,
		cache_file_gen2_definition,
		cache_file_gen3_definition;

	switch (version)
	{
	case _cache_version_gen1:
		return &cache_file_gen1_definition;
	case _cache_version_gen2:
		return &cache_file_gen2_definition;
	case _cache_version_gen3:
		return &cache_file_gen3_definition;
	default:
		return 0;
	}
}

cache_header_definition *cache_header_definition_get(
	cache_version version)
{
	extern cache_header_definition
		cache_header_gen1_definition,
		cache_header_gen2_definition,
		cache_header_gen3_definition;

	switch (version)
	{
	case _cache_version_gen1:
		return &cache_header_gen1_definition;
	case _cache_version_gen2:
		return &cache_header_gen2_definition;
	case _cache_version_gen3:
		return &cache_header_gen3_definition;
	default:
		return 0;
	}
}

cache_tag_header_definition *cache_tag_header_definition_get(
	cache_version version)
{
	extern cache_tag_header_definition
		cache_tag_header_gen1_definition,
		cache_tag_header_gen2_definition,
		cache_tag_header_gen3_definition;

	switch (version)
	{
	case _cache_version_gen1:
		return &cache_tag_header_gen1_definition;
	case _cache_version_gen2:
		return &cache_tag_header_gen2_definition;
	case _cache_version_gen3:
		return &cache_tag_header_gen3_definition;
	default:
		return 0;
	}
}

cache_tag_instance_definition *cache_tag_instance_definition_get(
	cache_version version)
{
	extern cache_tag_instance_definition
		cache_tag_instance_gen1_definition,
		cache_tag_instance_gen2_definition,
		cache_tag_instance_gen3_definition;

	switch (version)
	{
	case _cache_version_gen1:
		return &cache_tag_instance_gen1_definition;
	case _cache_version_gen2:
		return &cache_tag_instance_gen2_definition;
	case _cache_version_gen3:
		return &cache_tag_instance_gen3_definition;
	default:
		return 0;
	}
}

cache_strings_definition *cache_strings_definition_get(
	cache_version version)
{
	switch (version)
	{
	default:
		return 0;
	}
}

cache_file *cache_file_load(
	char const *path)
{
	tag header_tag = NONE;
	cache_version version = _cache_version_unknown;
	
	FILE *stream = fopen(path, "rb");
	fread(&header_tag, sizeof(tag), 1, stream);
	fread(&version, sizeof(long), 1, stream);
	fclose(stream);

	if (header_tag == 'daeh')
		version = _byteswap_ulong(version);
	
	cache_file_definition *definition = cache_file_definition_get(version);

	if (definition && definition->load)
		return definition->load(path);

	return 0;
}

void cache_file_dispose(
	cache_file *file)
{
	cache_version version = cache_file_get_version(file);
	cache_file_definition *definition = cache_file_definition_get(version);

	if (definition && definition->dispose)
		definition->dispose(file);
}

dword cache_file_get_base_address(
	cache_file *file)
{
	cache_version version = cache_file_get_version(file);
	cache_file_definition *definition = cache_file_definition_get(version);

	if (definition && definition->get_base_address)
		return definition->get_base_address();
	
	return 0;
}

cache_tag_instance *cache_file_get_tag_instance(
	cache_file *file,
	long index)
{
	cache_version version = cache_file_get_version(file);
	cache_file_definition *definition = cache_file_definition_get(version);

	if (definition && definition->get_tag_instance)
		return definition->get_tag_instance(file, index);
	
	return 0;
}

cache_version cache_file_get_version(
	cache_file *file)
{
	tag header_tag = *(tag *)file;
	cache_version version = *(cache_version *)((char *)file + 4);

	if (header_tag == 'daeh')
		version = _byteswap_ulong(version);
	
	return version;
}

long cache_file_get_file_length(
	cache_file *file)
{
	cache_version version = cache_file_get_version(file);
	cache_header_definition *definition = cache_header_definition_get(version);
	
	if (definition && definition->get_file_length)
		return definition->get_file_length(file);
	
	return NONE;
}

long cache_file_get_tag_header_offset(
	cache_file *file)
{
	cache_version version = cache_file_get_version(file);
	cache_header_definition *definition = cache_header_definition_get(version);
	
	if (definition && definition->get_tag_header_offset)
		return definition->get_tag_header_offset(file);
	
	return NONE;
}

long cache_file_get_tag_buffer_size(
	cache_file *file)
{
	cache_version version = cache_file_get_version(file);
	cache_header_definition *definition = cache_header_definition_get(version);
	
	if (definition && definition->get_tag_buffer_size)
		return definition->get_tag_buffer_size(file);
	
	return NONE;
}

char const *cache_file_get_name(
	cache_file *file)
{
	cache_version version = cache_file_get_version(file);
	cache_header_definition *definition = cache_header_definition_get(version);
	
	if (definition && definition->get_name)
		return definition->get_name(file);
	
	return 0;
}

char const *cache_file_get_build(
	cache_file *file)
{
	cache_version version = cache_file_get_version(file);
	cache_header_definition *definition = cache_header_definition_get(version);
	
	if (definition && definition->get_build)
		return definition->get_build(file);
	
	return 0;
}

cache_type cache_file_get_type(
	cache_file *file)
{
	cache_version version = cache_file_get_version(file);
	cache_header_definition *definition = cache_header_definition_get(version);
	
	if (definition && definition->get_type)
		return definition->get_type(file);
	
	return _cache_type_none;
}

cache_shared_type cache_file_get_shared_type(
	cache_file *file)
{
	cache_version version = cache_file_get_version(file);
	cache_header_definition *definition = cache_header_definition_get(version);
	
	if (definition && definition->get_shared_type)
		return definition->get_shared_type(file);
	
	return _cache_shared_type_none;
}

long cache_file_get_tag_count(
	cache_file *file)
{
	cache_version version = cache_file_get_version(file);
	cache_tag_header *header = (char *)file + cache_file_get_tag_header_offset(file);
	cache_tag_header_definition *definition = cache_tag_header_definition_get(version);
	
	if (definition && definition->get_tag_count)
		return definition->get_tag_count(header);
	
	return 0;
}

dword cache_file_get_tags_offset(
	cache_file *file)
{
	cache_version version = cache_file_get_version(file);
	cache_tag_header *header = (char *)file + cache_file_get_tag_header_offset(file);
	cache_tag_header_definition *definition = cache_tag_header_definition_get(version);
	
	if (definition && definition->get_tags_offset)
		return definition->get_tags_offset(header);
	
	return 0;
}

tag cache_file_get_tag_group_tag(
	cache_file *file,
	long index)
{
	cache_version version = cache_file_get_version(file);
	cache_tag_instance *instance = cache_file_get_tag_instance(file, index);
	cache_tag_instance_definition *definition = cache_tag_instance_definition_get(version);

	if (definition && definition->get_group_tag)
		return definition->get_group_tag(file, instance);
	
	return NONE;
}

char const *cache_file_get_tag_name(
	cache_file *file,
	long index)
{
	cache_version version = cache_file_get_version(file);
	cache_tag_instance *instance = cache_file_get_tag_instance(file, index);
	cache_tag_instance_definition *definition = cache_tag_instance_definition_get(version);

	if (definition && definition->get_name_offset)
		return (char *)file + definition->get_name_offset(file, instance);
	
	return 0;
}

dword cache_file_get_tag_offset(
	cache_file *file,
	long index)
{
	cache_version version = cache_file_get_version(file);
	cache_tag_instance *instance = cache_file_get_tag_instance(file, index);
	cache_tag_instance_definition *definition = cache_tag_instance_definition_get(version);

	if (definition && definition->get_offset)
		return definition->get_offset(file, instance);
	
	return 0;
}

char const *cache_file_get_string(
	cache_file *file,
	long index)
{
	cache_version version = cache_file_get_version(file);
	cache_strings_definition *definition = cache_strings_definition_get(version);
	
	if (definition && definition->get_string)
		return definition->get_string(file, index);
	
	return 0;
}
