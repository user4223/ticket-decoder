/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v1.3.4.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#include "CompartmentDetailsType.h"

static int
memb_coachType_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 99)) {
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
memb_compartmentType_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 99)) {
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
memb_specialAllocation_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 99)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_per_constraints_t asn_PER_memb_coachType_constr_2 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 7,  7,  1,  99 }	/* (1..99) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_compartmentType_constr_3 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 7,  7,  1,  99 }	/* (1..99) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_specialAllocation_constr_4 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 7,  7,  1,  99 }	/* (1..99) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static int asn_DFL_8_set_0(int set_value, void **sptr) {
	CompartmentPositionType_t *st = *sptr;
	
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
static asn_TYPE_member_t asn_MBR_CompartmentDetailsType_1[] = {
	{ ATF_POINTER, 7, offsetof(struct CompartmentDetailsType, coachType),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_coachType_constraint_1,
		&asn_PER_memb_coachType_constr_2,
		0,
		"coachType"
		},
	{ ATF_POINTER, 6, offsetof(struct CompartmentDetailsType, compartmentType),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_compartmentType_constraint_1,
		&asn_PER_memb_compartmentType_constr_3,
		0,
		"compartmentType"
		},
	{ ATF_POINTER, 5, offsetof(struct CompartmentDetailsType, specialAllocation),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_specialAllocation_constraint_1,
		&asn_PER_memb_specialAllocation_constr_4,
		0,
		"specialAllocation"
		},
	{ ATF_POINTER, 4, offsetof(struct CompartmentDetailsType, coachTypeDescr),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"coachTypeDescr"
		},
	{ ATF_POINTER, 3, offsetof(struct CompartmentDetailsType, compartmentTypeDescr),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"compartmentTypeDescr"
		},
	{ ATF_POINTER, 2, offsetof(struct CompartmentDetailsType, specialAllocationDescr),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"specialAllocationDescr"
		},
	{ ATF_POINTER, 1, offsetof(struct CompartmentDetailsType, position),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CompartmentPositionType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		asn_DFL_8_set_0,	/* DEFAULT 0 */
		"position"
		},
};
static const int asn_MAP_CompartmentDetailsType_oms_1[] = { 0, 1, 2, 3, 4, 5, 6 };
static const ber_tlv_tag_t asn_DEF_CompartmentDetailsType_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_CompartmentDetailsType_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* coachType */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* compartmentType */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* specialAllocation */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* coachTypeDescr */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* compartmentTypeDescr */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* specialAllocationDescr */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 } /* position */
};
static asn_SEQUENCE_specifics_t asn_SPC_CompartmentDetailsType_specs_1 = {
	sizeof(struct CompartmentDetailsType),
	offsetof(struct CompartmentDetailsType, _asn_ctx),
	asn_MAP_CompartmentDetailsType_tag2el_1,
	7,	/* Count of tags in the map */
	asn_MAP_CompartmentDetailsType_oms_1,	/* Optional members */
	7, 0,	/* Root/Additions */
	6,	/* Start extensions */
	8	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_CompartmentDetailsType = {
	"CompartmentDetailsType",
	"CompartmentDetailsType",
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
	asn_DEF_CompartmentDetailsType_tags_1,
	sizeof(asn_DEF_CompartmentDetailsType_tags_1)
		/sizeof(asn_DEF_CompartmentDetailsType_tags_1[0]), /* 1 */
	asn_DEF_CompartmentDetailsType_tags_1,	/* Same as above */
	sizeof(asn_DEF_CompartmentDetailsType_tags_1)
		/sizeof(asn_DEF_CompartmentDetailsType_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_CompartmentDetailsType_1,
	7,	/* Elements count */
	&asn_SPC_CompartmentDetailsType_specs_1	/* Additional specs */
};

