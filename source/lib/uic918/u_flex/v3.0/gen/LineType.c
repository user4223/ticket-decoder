/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module-RailTicketData"
 * 	found in "../../../../../../etc/UIC-barcode/misc/uicRailTicketData_v3.0.3.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#include "LineType.h"

static int
memb_carrierNum_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 32000)) {
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
memb_entryStationNum_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
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
memb_terminatingStationNum_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
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

static asn_per_constraints_t asn_PER_memb_carrierNum_constr_2 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 15,  15,  1,  32000 }	/* (1..32000) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_entryStationNum_constr_7 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 24, -1,  1,  9999999 }	/* (1..9999999) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_terminatingStationNum_constr_9 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 24, -1,  1,  9999999 }	/* (1..9999999) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_lineId_4[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		""
		},
};
static const ber_tlv_tag_t asn_DEF_lineId_tags_4[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_lineId_specs_4 = {
	sizeof(struct LineType__lineId),
	offsetof(struct LineType__lineId, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_lineId_4 = {
	"lineId",
	"lineId",
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
	asn_DEF_lineId_tags_4,
	sizeof(asn_DEF_lineId_tags_4)
		/sizeof(asn_DEF_lineId_tags_4[0]) - 1, /* 1 */
	asn_DEF_lineId_tags_4,	/* Same as above */
	sizeof(asn_DEF_lineId_tags_4)
		/sizeof(asn_DEF_lineId_tags_4[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_lineId_4,
	1,	/* Single element */
	&asn_SPC_lineId_specs_4	/* Additional specs */
};

static int asn_DFL_6_set_0(int set_value, void **sptr) {
	CodeTableType_t *st = *sptr;
	
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
static asn_TYPE_member_t asn_MBR_LineType_1[] = {
	{ ATF_POINTER, 9, offsetof(struct LineType, carrierNum),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_carrierNum_constraint_1,
		&asn_PER_memb_carrierNum_constr_2,
		0,
		"carrierNum"
		},
	{ ATF_POINTER, 8, offsetof(struct LineType, carrierIA5),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"carrierIA5"
		},
	{ ATF_POINTER, 7, offsetof(struct LineType, lineId),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		0,
		&asn_DEF_lineId_4,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"lineId"
		},
	{ ATF_POINTER, 6, offsetof(struct LineType, stationCodeTable),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CodeTableType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		asn_DFL_6_set_0,	/* DEFAULT 0 */
		"stationCodeTable"
		},
	{ ATF_POINTER, 5, offsetof(struct LineType, entryStationNum),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_entryStationNum_constraint_1,
		&asn_PER_memb_entryStationNum_constr_7,
		0,
		"entryStationNum"
		},
	{ ATF_POINTER, 4, offsetof(struct LineType, entryStationIA5),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"entryStationIA5"
		},
	{ ATF_POINTER, 3, offsetof(struct LineType, terminatingStationNum),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_terminatingStationNum_constraint_1,
		&asn_PER_memb_terminatingStationNum_constr_9,
		0,
		"terminatingStationNum"
		},
	{ ATF_POINTER, 2, offsetof(struct LineType, terminatingStationIA5),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"terminatingStationIA5"
		},
	{ ATF_POINTER, 1, offsetof(struct LineType, city),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"city"
		},
};
static const int asn_MAP_LineType_oms_1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
static const ber_tlv_tag_t asn_DEF_LineType_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_LineType_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* carrierNum */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* carrierIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* lineId */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* stationCodeTable */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* entryStationNum */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* entryStationIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* terminatingStationNum */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* terminatingStationIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 } /* city */
};
static asn_SEQUENCE_specifics_t asn_SPC_LineType_specs_1 = {
	sizeof(struct LineType),
	offsetof(struct LineType, _asn_ctx),
	asn_MAP_LineType_tag2el_1,
	9,	/* Count of tags in the map */
	asn_MAP_LineType_oms_1,	/* Optional members */
	9, 0,	/* Root/Additions */
	8,	/* Start extensions */
	10	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_LineType = {
	"LineType",
	"LineType",
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
	asn_DEF_LineType_tags_1,
	sizeof(asn_DEF_LineType_tags_1)
		/sizeof(asn_DEF_LineType_tags_1[0]), /* 1 */
	asn_DEF_LineType_tags_1,	/* Same as above */
	sizeof(asn_DEF_LineType_tags_1)
		/sizeof(asn_DEF_LineType_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_LineType_1,
	9,	/* Elements count */
	&asn_SPC_LineType_specs_1	/* Additional specs */
};

