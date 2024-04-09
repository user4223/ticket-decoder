/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module-RailTicketData"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v3.0.3.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#include "ReturnRouteDescriptionType.h"

static int
memb_fromStationNum_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 9999999)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_toStationNum_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 9999999)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_per_constraints_t asn_PER_memb_fromStationNum_constr_2 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 24, -1,  1,  9999999 }	/* (1..9999999) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_toStationNum_constr_4 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 24, -1,  1,  9999999 }	/* (1..9999999) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_validReturnRegion_9[] = {
	{ ATF_POINTER, 0, 0,
		-1 /* Ambiguous tag (CHOICE?) */,
		0,
		&asn_DEF_RegionalValidityType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		""
		},
};
static const ber_tlv_tag_t asn_DEF_validReturnRegion_tags_9[] = {
	(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_validReturnRegion_specs_9 = {
	sizeof(struct ReturnRouteDescriptionType__validReturnRegion),
	offsetof(struct ReturnRouteDescriptionType__validReturnRegion, _asn_ctx),
	2,	/* XER encoding is XMLValueList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_validReturnRegion_9 = {
	"validReturnRegion",
	"validReturnRegion",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	SEQUENCE_OF_decode_uper,
	SEQUENCE_OF_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_validReturnRegion_tags_9,
	sizeof(asn_DEF_validReturnRegion_tags_9)
		/sizeof(asn_DEF_validReturnRegion_tags_9[0]) - 1, /* 1 */
	asn_DEF_validReturnRegion_tags_9,	/* Same as above */
	sizeof(asn_DEF_validReturnRegion_tags_9)
		/sizeof(asn_DEF_validReturnRegion_tags_9[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_validReturnRegion_9,
	1,	/* Single element */
	&asn_SPC_validReturnRegion_specs_9	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_ReturnRouteDescriptionType_1[] = {
	{ ATF_POINTER, 8, offsetof(struct ReturnRouteDescriptionType, fromStationNum),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_fromStationNum_constraint_1,
		&asn_PER_memb_fromStationNum_constr_2,
		0,
		"fromStationNum"
		},
	{ ATF_POINTER, 7, offsetof(struct ReturnRouteDescriptionType, fromStationIA5),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"fromStationIA5"
		},
	{ ATF_POINTER, 6, offsetof(struct ReturnRouteDescriptionType, toStationNum),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_toStationNum_constraint_1,
		&asn_PER_memb_toStationNum_constr_4,
		0,
		"toStationNum"
		},
	{ ATF_POINTER, 5, offsetof(struct ReturnRouteDescriptionType, toStationIA5),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"toStationIA5"
		},
	{ ATF_POINTER, 4, offsetof(struct ReturnRouteDescriptionType, fromStationNameUTF8),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"fromStationNameUTF8"
		},
	{ ATF_POINTER, 3, offsetof(struct ReturnRouteDescriptionType, toStationNameUTF8),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"toStationNameUTF8"
		},
	{ ATF_POINTER, 2, offsetof(struct ReturnRouteDescriptionType, validReturnRegionDesc),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"validReturnRegionDesc"
		},
	{ ATF_POINTER, 1, offsetof(struct ReturnRouteDescriptionType, validReturnRegion),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		0,
		&asn_DEF_validReturnRegion_9,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"validReturnRegion"
		},
};
static const int asn_MAP_ReturnRouteDescriptionType_oms_1[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
static const ber_tlv_tag_t asn_DEF_ReturnRouteDescriptionType_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ReturnRouteDescriptionType_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* fromStationNum */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* fromStationIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* toStationNum */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* toStationIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* fromStationNameUTF8 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* toStationNameUTF8 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* validReturnRegionDesc */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 } /* validReturnRegion */
};
static asn_SEQUENCE_specifics_t asn_SPC_ReturnRouteDescriptionType_specs_1 = {
	sizeof(struct ReturnRouteDescriptionType),
	offsetof(struct ReturnRouteDescriptionType, _asn_ctx),
	asn_MAP_ReturnRouteDescriptionType_tag2el_1,
	8,	/* Count of tags in the map */
	asn_MAP_ReturnRouteDescriptionType_oms_1,	/* Optional members */
	8, 0,	/* Root/Additions */
	7,	/* Start extensions */
	9	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_ReturnRouteDescriptionType = {
	"ReturnRouteDescriptionType",
	"ReturnRouteDescriptionType",
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
	asn_DEF_ReturnRouteDescriptionType_tags_1,
	sizeof(asn_DEF_ReturnRouteDescriptionType_tags_1)
		/sizeof(asn_DEF_ReturnRouteDescriptionType_tags_1[0]), /* 1 */
	asn_DEF_ReturnRouteDescriptionType_tags_1,	/* Same as above */
	sizeof(asn_DEF_ReturnRouteDescriptionType_tags_1)
		/sizeof(asn_DEF_ReturnRouteDescriptionType_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_ReturnRouteDescriptionType_1,
	8,	/* Elements count */
	&asn_SPC_ReturnRouteDescriptionType_specs_1	/* Additional specs */
};

