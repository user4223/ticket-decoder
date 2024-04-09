/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v2.0.2.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#include "GeoCoordinateType.h"

static int asn_DFL_2_set_2(int set_value, void **sptr) {
	GeoUnitType_t *st = *sptr;
	
	if(!st) {
		if(!set_value) return -1;	/* Not a default value */
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	if(set_value) {
		/* Install default value 2 */
		return asn_long2INTEGER(st, 2);
	} else {
		/* Test default value 2 */
		long value;
		if(asn_INTEGER2long(st, &value))
			return -1;
		return (value == 2);
	}
}
static int asn_DFL_3_set_0(int set_value, void **sptr) {
	GeoCoordinateSystemType_t *st = *sptr;
	
	if(!st) {
		if(!set_value) return -1;	/* Not a default value */
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	if(set_value) {
		/* Install default value 0 */
		return asn_long2INTEGER(st, 0);
	} else {
		/* Test default value 0 */
		long value;
		if(asn_INTEGER2long(st, &value))
			return -1;
		return (value == 0);
	}
}
static int asn_DFL_4_set_0(int set_value, void **sptr) {
	HemisphereLongitudeType_t *st = *sptr;
	
	if(!st) {
		if(!set_value) return -1;	/* Not a default value */
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	if(set_value) {
		/* Install default value 0 */
		return asn_long2INTEGER(st, 0);
	} else {
		/* Test default value 0 */
		long value;
		if(asn_INTEGER2long(st, &value))
			return -1;
		return (value == 0);
	}
}
static int asn_DFL_5_set_0(int set_value, void **sptr) {
	HemisphereLatitudeType_t *st = *sptr;
	
	if(!st) {
		if(!set_value) return -1;	/* Not a default value */
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	if(set_value) {
		/* Install default value 0 */
		return asn_long2INTEGER(st, 0);
	} else {
		/* Test default value 0 */
		long value;
		if(asn_INTEGER2long(st, &value))
			return -1;
		return (value == 0);
	}
}
static asn_TYPE_member_t asn_MBR_GeoCoordinateType_1[] = {
	{ ATF_POINTER, 4, offsetof(struct GeoCoordinateType, geoUnit),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_GeoUnitType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		asn_DFL_2_set_2,	/* DEFAULT 2 */
		"geoUnit"
		},
	{ ATF_POINTER, 3, offsetof(struct GeoCoordinateType, coordinateSystem),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_GeoCoordinateSystemType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		asn_DFL_3_set_0,	/* DEFAULT 0 */
		"coordinateSystem"
		},
	{ ATF_POINTER, 2, offsetof(struct GeoCoordinateType, hemisphereLongitude),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_HemisphereLongitudeType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		asn_DFL_4_set_0,	/* DEFAULT 0 */
		"hemisphereLongitude"
		},
	{ ATF_POINTER, 1, offsetof(struct GeoCoordinateType, hemisphereLatitude),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_HemisphereLatitudeType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		asn_DFL_5_set_0,	/* DEFAULT 0 */
		"hemisphereLatitude"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct GeoCoordinateType, longitude),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"longitude"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct GeoCoordinateType, latitude),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"latitude"
		},
	{ ATF_POINTER, 1, offsetof(struct GeoCoordinateType, accuracy),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_GeoUnitType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"accuracy"
		},
};
static const int asn_MAP_GeoCoordinateType_oms_1[] = { 0, 1, 2, 3, 6 };
static const ber_tlv_tag_t asn_DEF_GeoCoordinateType_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_GeoCoordinateType_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* geoUnit */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* coordinateSystem */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* hemisphereLongitude */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* hemisphereLatitude */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* longitude */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* latitude */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 } /* accuracy */
};
static asn_SEQUENCE_specifics_t asn_SPC_GeoCoordinateType_specs_1 = {
	sizeof(struct GeoCoordinateType),
	offsetof(struct GeoCoordinateType, _asn_ctx),
	asn_MAP_GeoCoordinateType_tag2el_1,
	7,	/* Count of tags in the map */
	asn_MAP_GeoCoordinateType_oms_1,	/* Optional members */
	5, 0,	/* Root/Additions */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_GeoCoordinateType = {
	"GeoCoordinateType",
	"GeoCoordinateType",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	SEQUENCE_decode_uper,
	SEQUENCE_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_GeoCoordinateType_tags_1,
	sizeof(asn_DEF_GeoCoordinateType_tags_1)
		/sizeof(asn_DEF_GeoCoordinateType_tags_1[0]), /* 1 */
	asn_DEF_GeoCoordinateType_tags_1,	/* Same as above */
	sizeof(asn_DEF_GeoCoordinateType_tags_1)
		/sizeof(asn_DEF_GeoCoordinateType_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_GeoCoordinateType_1,
	7,	/* Elements count */
	&asn_SPC_GeoCoordinateType_specs_1	/* Additional specs */
};

