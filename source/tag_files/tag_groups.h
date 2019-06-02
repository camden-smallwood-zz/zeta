#pragma once

/* ---------- macros */

#define TAG_STRUCT(NAME, SIZE) \
	extern field_definition _##NAME##_fields[]; \
	struct_definition NAME = \
	{ \
		#NAME, \
		(SIZE), \
		&_##NAME##_fields[0] \
	}; \
	field_definition _##NAME##_fields[] =

#define TAG_ENUM(NAME, OPTION_COUNT) \
	extern enum_option _##NAME##_options[(OPTION_COUNT)]; \
	enum_definition NAME = \
	{ \
		#NAME, \
		(OPTION_COUNT), \
		_##NAME##_options \
	}; \
	enum_option _##NAME##_options[(OPTION_COUNT)] =

#define TAG_ARRAY(TYPE, NAME, LENGTH, ...) \
	array_definition NAME = \
	{ \
		(TYPE), \
		#NAME, \
		(LENGTH), \
		__VA_ARGS__ \
	}

/* ---------- types */

typedef enum field_type
{
    _field_tag,
	_field_string,
	_field_long_string,
	_field_string_id,
	_field_char_integer,
	_field_short_integer,
	_field_long_integer,
	_field_int64_integer,
	_field_byte_integer,
	_field_word_integer,
	_field_dword_integer,
	_field_qword_integer,
	_field_char_enum,
	_field_short_enum,
	_field_long_enum,
	_field_byte_flags,
	_field_word_flags,
	_field_long_flags,
	_field_point2d,
	_field_rectangle2d,
	_field_rgb_color,
	_field_argb_color,
	_field_short_bounds,
	_field_angle,
	_field_real,
	_field_real_fraction,
	_field_real_point2d,
	_field_real_point3d,
	_field_real_vector2d,
	_field_real_vector3d,
	_field_real_quaternion,
	_field_real_euler_angles2d,
	_field_real_euler_angles3d,
	_field_real_plane2d,
	_field_real_plane3d,
	_field_real_rgb_color,
	_field_real_argb_color,
	_field_real_hsv_color,
	_field_real_ahsv_color,
	_field_angle_bounds,
	_field_real_bounds,
	_field_real_fraction_bounds,
	_field_tag_reference,
	_field_block,
	_field_char_block_index,
	_field_short_block_index,
	_field_long_block_index,
	_field_byte_block_flags,
	_field_word_block_flags,
	_field_long_block_flags,
	_field_data,
	_field_array,
	_field_struct,
	_field_pointer,
	_field_skip,
	_field_padding,
	_field_explanation,
	_field_terminator,
    NUMBER_OF_FIELD_TYPES
} field_type;

typedef struct field_definition
{
	field_type type;
	char const *name;
	void *definition;
} field_definition;

typedef struct struct_definition
{
	char const *name;
	long size;
	field_definition *fields;
} struct_definition;

typedef struct enum_option
{
	char const *name;
	long value;
} enum_option;

typedef struct enum_definition
{
	char const *name;
	long option_count;
	enum_option *options;
} enum_definition;

typedef struct array_definition
{
	field_type element_type;
	char const *name;
	long length;
	void *definition;
} array_definition;

/* ---------- prototypes/TAG_GROUPS.C */

long field_byteswap(field_type type, cache_version version, void *definition, void *address);
long struct_byteswap(struct_definition *definition, cache_version version, void *address);
long array_byteswap(array_definition *definition, cache_version version, void *address);
