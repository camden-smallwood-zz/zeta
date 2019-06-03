
/* ---------- headers */

#include <stdio.h>
#include <stdlib.h>

#include <cseries/cseries.h>
#include <cache/cache_files.h>
#include <tag_files/tag_groups.h>

/* ---------- code */

long field_byteswap(
	field_type type,
	cache_version version,
	void *definition,
	void *address)
{
	long result = 0;

	switch (type)
	{
	case _field_tag:
	case _field_string_id:
	case _field_long_integer:
	case _field_dword_integer:
	case _field_long_enum:
	case _field_long_flags:
	case _field_rgb_color:
	case _field_argb_color:
	case _field_angle:
	case _field_real:
	case _field_real_fraction:
	case _field_long_block_index:
	case _field_long_block_flags:
		((dword *)address)[0] =  _byteswap_ulong(((dword *)address)[0]);
		result = sizeof(dword);
		break;

	case _field_int64_integer:
	case _field_qword_integer:
		((qword *)address)[0] = _byteswap_uint64(((qword *)address)[0]);
		result = sizeof(qword);
		break;

	case _field_char_integer:
	case _field_byte_integer:
	case _field_char_enum:
	case _field_byte_flags:
	case _field_char_block_index:
	case _field_byte_block_flags:
		result += sizeof(byte);
		break;
	
	case _field_string:
		result += sizeof(tag_string);
		break;
	
	case _field_long_string:
		result += sizeof(long_string);
		break;

	case _field_short_integer:
	case _field_word_integer:
	case _field_short_enum:
	case _field_word_flags:
	case _field_short_block_index:
	case _field_word_block_flags:
		((word *)address)[0] = _byteswap_ushort(((word *)address)[0]);
		result = sizeof(word);
		break;

	case _field_point2d:
	case _field_short_bounds:
		((word *)address)[0] = _byteswap_ushort(((word *)address)[0]);
		((word *)address)[1] = _byteswap_ushort(((word *)address)[1]);
		result = sizeof(word) * 2;
		break;

	case _field_rectangle2d:
		((word *)address)[0] = _byteswap_ushort(((word *)address)[0]);
		((word *)address)[1] = _byteswap_ushort(((word *)address)[1]);
		((word *)address)[2] = _byteswap_ushort(((word *)address)[2]);
		((word *)address)[3] = _byteswap_ushort(((word *)address)[3]);
		result = sizeof(word) * 4;
		break;

	case _field_real_point2d:
	case _field_real_vector2d:
	case _field_real_euler_angles2d:
	case _field_angle_bounds:
	case _field_real_bounds:
	case _field_real_fraction_bounds:
		((dword *)address)[0] =  _byteswap_ulong(((dword *)address)[0]);
		((dword *)address)[1] =  _byteswap_ulong(((dword *)address)[1]);
		result = sizeof(dword);
		break;

	case _field_real_point3d:
	case _field_real_vector3d:
	case _field_real_euler_angles3d:
	case _field_real_plane2d:
	case _field_real_rgb_color:
	case _field_real_hsv_color:
	case _field_real_ahsv_color:
		((dword *)address)[0] =  _byteswap_ulong(((dword *)address)[0]);
		((dword *)address)[1] =  _byteswap_ulong(((dword *)address)[1]);
		((dword *)address)[2] =  _byteswap_ulong(((dword *)address)[2]);
		result = sizeof(dword) * 3;
		break;

	case _field_real_quaternion:
	case _field_real_plane3d:
	case _field_real_argb_color:
		((dword *)address)[0] =  _byteswap_ulong(((dword *)address)[0]);
		((dword *)address)[1] =  _byteswap_ulong(((dword *)address)[1]);
		((dword *)address)[2] =  _byteswap_ulong(((dword *)address)[2]);
		((dword *)address)[3] =  _byteswap_ulong(((dword *)address)[3]);
		result = sizeof(dword) * 4;
		break;

	case _field_tag_reference:
		((dword *)address)[0] =  _byteswap_ulong(((dword *)address)[0]);
		if (version < _cache_version_gen3)
		{
			((dword *)address)[1] =  _byteswap_ulong(((dword *)address)[1]);
			result = sizeof(dword) * 2;
		}
		else
		{
			((dword *)address)[3] =  _byteswap_ulong(((dword *)address)[3]);
			result = sizeof(dword) * 4;
		}
		break;

	case _field_block:
		((dword *)address)[0] =  _byteswap_ulong(((dword *)address)[0]);
		((dword *)address)[1] =  _byteswap_ulong(((dword *)address)[1]);
		if (version < _cache_version_gen3)
			result = sizeof(dword) * 2;
		else
			result = sizeof(dword) * 3;
		break;

	case _field_data:
		((dword *)address)[0] =  _byteswap_ulong(((dword *)address)[0]);
		if (version < _cache_version_gen3)
		{
			((dword *)address)[1] =  _byteswap_ulong(((dword *)address)[1]);
			result = sizeof(dword) * 2;
		}
		else
		{
			((dword *)address)[3] =  _byteswap_ulong(((dword *)address)[3]);
			result = sizeof(dword) * 5;
		}
		break;

	case _field_array:
		result = array_byteswap(definition, version, address);
		break;

	case _field_struct:
		result = struct_byteswap(definition, version, address);
		break;

	case _field_padding:
		result = *(long *)definition;
		break;
		
	case _field_pointer:
		puts("ERROR: _field_pointer byteswap not implemented!");
		exit(NONE);
		break;

	case _field_skip:
		puts("ERROR: _field_skip byteswap not implemented!");
		exit(NONE);
		break;
		
	case _field_explanation:
		puts("ERROR: _field_explanation byteswap not implemented!");
		exit(NONE);
		break;
	}

	return result;
}

long struct_byteswap(
	struct_definition *definition,
	cache_version version,
	void *address)
{
	long i = 0, result = 0;

	for (i = 0; definition->fields[i].type != _field_terminator; i++)
		result += field_byteswap(definition->fields[i].type, version, definition->fields[i].definition, (char *)address + result);
	
	return result;
}

long array_byteswap(
	array_definition *definition,
	cache_version version,
	void *address)
{
	long i = 0, result = 0;

	for (i = 0; i < definition->length; i++)
		result += field_byteswap(definition->element_type, version, definition->definition, (char *)address + result);
	
	return result;
}
