/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module-RailTicketData"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v3.0.3.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#include "IncludedOpenTicketType.h"

static int check_permitted_alphabet_18(const void *sptr) {
	/* The underlying type is IA5String */
	const IA5String_t *st = (const IA5String_t *)sptr;
	const uint8_t *ch = st->buf;
	const uint8_t *end = ch + st->size;
	
	for(; ch < end; ch++) {
		uint8_t cv = *ch;
		if(!(cv <= 127)) return -1;
	}
	return 0;
}

static int
memb_NativeInteger_constraint_19(asn_TYPE_descriptor_t *td, const void *sptr,
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
memb_NativeInteger_constraint_23(asn_TYPE_descriptor_t *td, const void *sptr,
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
memb_NativeInteger_constraint_25(asn_TYPE_descriptor_t *td, const void *sptr,
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
memb_NativeInteger_constraint_30(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 31)) {
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
memb_NativeInteger_constraint_32(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 31)) {
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
memb_productOwnerNum_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
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
memb_productIdNum_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 65535)) {
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
memb_validFromDay_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= -367 && value <= 700)) {
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
memb_validFromTime_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 1439)) {
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
memb_validFromUTCOffset_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= -60 && value <= 60)) {
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
memb_validUntilDay_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= -1 && value <= 500)) {
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
memb_validUntilTime_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 1439)) {
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
memb_validUntilUTCOffset_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= -60 && value <= 60)) {
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
memb_serviceLevel_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	const IA5String_t *st = (const IA5String_t *)sptr;
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	size = st->size;
	
	if((size >= 1 && size <= 2)
		 && !check_permitted_alphabet_18(st)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_per_constraints_t asn_PER_memb_Member_constr_20 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 15,  15,  1,  32000 }	/* (1..32000) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_Member_constr_24 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 15,  15,  1,  32000 }	/* (1..32000) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_Member_constr_26 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 15,  15,  1,  32000 }	/* (1..32000) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_Member_constr_31 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 5,  5,  0,  31 }	/* (0..31) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_Member_constr_33 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 5,  5,  0,  31 }	/* (0..31) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_productOwnerNum_constr_2 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 15,  15,  1,  32000 }	/* (1..32000) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_productIdNum_constr_4 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 16,  16,  0,  65535 }	/* (0..65535) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_validFromDay_constr_11 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 11,  11, -367,  700 }	/* (-367..700) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_validFromTime_constr_12 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 11,  11,  0,  1439 }	/* (0..1439) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_validFromUTCOffset_constr_13 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 7,  7, -60,  60 }	/* (-60..60) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_validUntilDay_constr_14 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 9,  9, -1,  500 }	/* (-1..500) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_validUntilTime_constr_15 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 11,  11,  0,  1439 }	/* (0..1439) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_validUntilUTCOffset_constr_16 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 7,  7, -60,  60 }	/* (-60..60) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_serviceLevel_constr_18 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 7,  7,  0,  127 }	/* (0..127) */,
	{ APC_CONSTRAINED,	 1,  1,  1,  2 }	/* (SIZE(1..2)) */,
	0, 0	/* No PER character map necessary */
};
static int asn_DFL_8_set_0(int set_value, void **sptr) {
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
static asn_TYPE_member_t asn_MBR_validRegion_9[] = {
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
static const ber_tlv_tag_t asn_DEF_validRegion_tags_9[] = {
	(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_validRegion_specs_9 = {
	sizeof(struct IncludedOpenTicketType__validRegion),
	offsetof(struct IncludedOpenTicketType__validRegion, _asn_ctx),
	2,	/* XER encoding is XMLValueList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_validRegion_9 = {
	"validRegion",
	"validRegion",
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
	asn_DEF_validRegion_tags_9,
	sizeof(asn_DEF_validRegion_tags_9)
		/sizeof(asn_DEF_validRegion_tags_9[0]) - 1, /* 1 */
	asn_DEF_validRegion_tags_9,	/* Same as above */
	sizeof(asn_DEF_validRegion_tags_9)
		/sizeof(asn_DEF_validRegion_tags_9[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_validRegion_9,
	1,	/* Single element */
	&asn_SPC_validRegion_specs_9	/* Additional specs */
};

static int asn_DFL_11_set_0(int set_value, void **sptr) {
	long *st = *sptr;
	
	if(!st) {
		if(!set_value) return -1;	/* Not a default value */
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	if(set_value) {
		/* Install default value 0 */
		*st = 0;
		return 0;
	} else {
		/* Test default value 0 */
		return (*st == 0);
	}
}
static int asn_DFL_14_set_0(int set_value, void **sptr) {
	long *st = *sptr;
	
	if(!st) {
		if(!set_value) return -1;	/* Not a default value */
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	if(set_value) {
		/* Install default value 0 */
		*st = 0;
		return 0;
	} else {
		/* Test default value 0 */
		return (*st == 0);
	}
}
static asn_TYPE_member_t asn_MBR_carrierNum_19[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NativeInteger,
		memb_NativeInteger_constraint_19,
		&asn_PER_memb_Member_constr_20,
		0,
		""
		},
};
static const ber_tlv_tag_t asn_DEF_carrierNum_tags_19[] = {
	(ASN_TAG_CLASS_CONTEXT | (16 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_carrierNum_specs_19 = {
	sizeof(struct IncludedOpenTicketType__carrierNum),
	offsetof(struct IncludedOpenTicketType__carrierNum, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_carrierNum_19 = {
	"carrierNum",
	"carrierNum",
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
	asn_DEF_carrierNum_tags_19,
	sizeof(asn_DEF_carrierNum_tags_19)
		/sizeof(asn_DEF_carrierNum_tags_19[0]) - 1, /* 1 */
	asn_DEF_carrierNum_tags_19,	/* Same as above */
	sizeof(asn_DEF_carrierNum_tags_19)
		/sizeof(asn_DEF_carrierNum_tags_19[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_carrierNum_19,
	1,	/* Single element */
	&asn_SPC_carrierNum_specs_19	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_carrierIA5_21[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (22 << 2)),
		0,
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		""
		},
};
static const ber_tlv_tag_t asn_DEF_carrierIA5_tags_21[] = {
	(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_carrierIA5_specs_21 = {
	sizeof(struct IncludedOpenTicketType__carrierIA5),
	offsetof(struct IncludedOpenTicketType__carrierIA5, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_carrierIA5_21 = {
	"carrierIA5",
	"carrierIA5",
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
	asn_DEF_carrierIA5_tags_21,
	sizeof(asn_DEF_carrierIA5_tags_21)
		/sizeof(asn_DEF_carrierIA5_tags_21[0]) - 1, /* 1 */
	asn_DEF_carrierIA5_tags_21,	/* Same as above */
	sizeof(asn_DEF_carrierIA5_tags_21)
		/sizeof(asn_DEF_carrierIA5_tags_21[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_carrierIA5_21,
	1,	/* Single element */
	&asn_SPC_carrierIA5_specs_21	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_includedServiceBrands_23[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NativeInteger,
		memb_NativeInteger_constraint_23,
		&asn_PER_memb_Member_constr_24,
		0,
		""
		},
};
static const ber_tlv_tag_t asn_DEF_includedServiceBrands_tags_23[] = {
	(ASN_TAG_CLASS_CONTEXT | (18 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_includedServiceBrands_specs_23 = {
	sizeof(struct IncludedOpenTicketType__includedServiceBrands),
	offsetof(struct IncludedOpenTicketType__includedServiceBrands, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_includedServiceBrands_23 = {
	"includedServiceBrands",
	"includedServiceBrands",
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
	asn_DEF_includedServiceBrands_tags_23,
	sizeof(asn_DEF_includedServiceBrands_tags_23)
		/sizeof(asn_DEF_includedServiceBrands_tags_23[0]) - 1, /* 1 */
	asn_DEF_includedServiceBrands_tags_23,	/* Same as above */
	sizeof(asn_DEF_includedServiceBrands_tags_23)
		/sizeof(asn_DEF_includedServiceBrands_tags_23[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_includedServiceBrands_23,
	1,	/* Single element */
	&asn_SPC_includedServiceBrands_specs_23	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_excludedServiceBrands_25[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NativeInteger,
		memb_NativeInteger_constraint_25,
		&asn_PER_memb_Member_constr_26,
		0,
		""
		},
};
static const ber_tlv_tag_t asn_DEF_excludedServiceBrands_tags_25[] = {
	(ASN_TAG_CLASS_CONTEXT | (19 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_excludedServiceBrands_specs_25 = {
	sizeof(struct IncludedOpenTicketType__excludedServiceBrands),
	offsetof(struct IncludedOpenTicketType__excludedServiceBrands, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_excludedServiceBrands_25 = {
	"excludedServiceBrands",
	"excludedServiceBrands",
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
	asn_DEF_excludedServiceBrands_tags_25,
	sizeof(asn_DEF_excludedServiceBrands_tags_25)
		/sizeof(asn_DEF_excludedServiceBrands_tags_25[0]) - 1, /* 1 */
	asn_DEF_excludedServiceBrands_tags_25,	/* Same as above */
	sizeof(asn_DEF_excludedServiceBrands_tags_25)
		/sizeof(asn_DEF_excludedServiceBrands_tags_25[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_excludedServiceBrands_25,
	1,	/* Single element */
	&asn_SPC_excludedServiceBrands_specs_25	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_tariffs_27[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_TariffType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		""
		},
};
static const ber_tlv_tag_t asn_DEF_tariffs_tags_27[] = {
	(ASN_TAG_CLASS_CONTEXT | (20 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_tariffs_specs_27 = {
	sizeof(struct IncludedOpenTicketType__tariffs),
	offsetof(struct IncludedOpenTicketType__tariffs, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_tariffs_27 = {
	"tariffs",
	"tariffs",
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
	asn_DEF_tariffs_tags_27,
	sizeof(asn_DEF_tariffs_tags_27)
		/sizeof(asn_DEF_tariffs_tags_27[0]) - 1, /* 1 */
	asn_DEF_tariffs_tags_27,	/* Same as above */
	sizeof(asn_DEF_tariffs_tags_27)
		/sizeof(asn_DEF_tariffs_tags_27[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_tariffs_27,
	1,	/* Single element */
	&asn_SPC_tariffs_specs_27	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_includedTransportType_30[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NativeInteger,
		memb_NativeInteger_constraint_30,
		&asn_PER_memb_Member_constr_31,
		0,
		""
		},
};
static const ber_tlv_tag_t asn_DEF_includedTransportType_tags_30[] = {
	(ASN_TAG_CLASS_CONTEXT | (22 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_includedTransportType_specs_30 = {
	sizeof(struct IncludedOpenTicketType__includedTransportType),
	offsetof(struct IncludedOpenTicketType__includedTransportType, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_includedTransportType_30 = {
	"includedTransportType",
	"includedTransportType",
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
	asn_DEF_includedTransportType_tags_30,
	sizeof(asn_DEF_includedTransportType_tags_30)
		/sizeof(asn_DEF_includedTransportType_tags_30[0]) - 1, /* 1 */
	asn_DEF_includedTransportType_tags_30,	/* Same as above */
	sizeof(asn_DEF_includedTransportType_tags_30)
		/sizeof(asn_DEF_includedTransportType_tags_30[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_includedTransportType_30,
	1,	/* Single element */
	&asn_SPC_includedTransportType_specs_30	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_excludedTransportType_32[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NativeInteger,
		memb_NativeInteger_constraint_32,
		&asn_PER_memb_Member_constr_33,
		0,
		""
		},
};
static const ber_tlv_tag_t asn_DEF_excludedTransportType_tags_32[] = {
	(ASN_TAG_CLASS_CONTEXT | (23 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_excludedTransportType_specs_32 = {
	sizeof(struct IncludedOpenTicketType__excludedTransportType),
	offsetof(struct IncludedOpenTicketType__excludedTransportType, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_excludedTransportType_32 = {
	"excludedTransportType",
	"excludedTransportType",
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
	asn_DEF_excludedTransportType_tags_32,
	sizeof(asn_DEF_excludedTransportType_tags_32)
		/sizeof(asn_DEF_excludedTransportType_tags_32[0]) - 1, /* 1 */
	asn_DEF_excludedTransportType_tags_32,	/* Same as above */
	sizeof(asn_DEF_excludedTransportType_tags_32)
		/sizeof(asn_DEF_excludedTransportType_tags_32[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_excludedTransportType_32,
	1,	/* Single element */
	&asn_SPC_excludedTransportType_specs_32	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_IncludedOpenTicketType_1[] = {
	{ ATF_POINTER, 25, offsetof(struct IncludedOpenTicketType, productOwnerNum),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_productOwnerNum_constraint_1,
		&asn_PER_memb_productOwnerNum_constr_2,
		0,
		"productOwnerNum"
		},
	{ ATF_POINTER, 24, offsetof(struct IncludedOpenTicketType, productOwnerIA5),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"productOwnerIA5"
		},
	{ ATF_POINTER, 23, offsetof(struct IncludedOpenTicketType, productIdNum),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_productIdNum_constraint_1,
		&asn_PER_memb_productIdNum_constr_4,
		0,
		"productIdNum"
		},
	{ ATF_POINTER, 22, offsetof(struct IncludedOpenTicketType, productIdIA5),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"productIdIA5"
		},
	{ ATF_POINTER, 21, offsetof(struct IncludedOpenTicketType, externalIssuerId),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"externalIssuerId"
		},
	{ ATF_POINTER, 20, offsetof(struct IncludedOpenTicketType, issuerAutorizationId),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"issuerAutorizationId"
		},
	{ ATF_POINTER, 19, offsetof(struct IncludedOpenTicketType, stationCodeTable),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CodeTableType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		asn_DFL_8_set_0,	/* DEFAULT 0 */
		"stationCodeTable"
		},
	{ ATF_POINTER, 18, offsetof(struct IncludedOpenTicketType, validRegion),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		0,
		&asn_DEF_validRegion_9,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"validRegion"
		},
	{ ATF_NOFLAGS, 17, offsetof(struct IncludedOpenTicketType, validFromDay),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_validFromDay_constraint_1,
		&asn_PER_memb_validFromDay_constr_11,
		asn_DFL_11_set_0,	/* DEFAULT 0 */
		"validFromDay"
		},
	{ ATF_POINTER, 16, offsetof(struct IncludedOpenTicketType, validFromTime),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_validFromTime_constraint_1,
		&asn_PER_memb_validFromTime_constr_12,
		0,
		"validFromTime"
		},
	{ ATF_POINTER, 15, offsetof(struct IncludedOpenTicketType, validFromUTCOffset),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_validFromUTCOffset_constraint_1,
		&asn_PER_memb_validFromUTCOffset_constr_13,
		0,
		"validFromUTCOffset"
		},
	{ ATF_NOFLAGS, 14, offsetof(struct IncludedOpenTicketType, validUntilDay),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_validUntilDay_constraint_1,
		&asn_PER_memb_validUntilDay_constr_14,
		asn_DFL_14_set_0,	/* DEFAULT 0 */
		"validUntilDay"
		},
	{ ATF_POINTER, 13, offsetof(struct IncludedOpenTicketType, validUntilTime),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_validUntilTime_constraint_1,
		&asn_PER_memb_validUntilTime_constr_15,
		0,
		"validUntilTime"
		},
	{ ATF_POINTER, 12, offsetof(struct IncludedOpenTicketType, validUntilUTCOffset),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_validUntilUTCOffset_constraint_1,
		&asn_PER_memb_validUntilUTCOffset_constr_16,
		0,
		"validUntilUTCOffset"
		},
	{ ATF_POINTER, 11, offsetof(struct IncludedOpenTicketType, classCode),
		(ASN_TAG_CLASS_CONTEXT | (14 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TravelClassType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"classCode"
		},
	{ ATF_POINTER, 10, offsetof(struct IncludedOpenTicketType, serviceLevel),
		(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		memb_serviceLevel_constraint_1,
		&asn_PER_memb_serviceLevel_constr_18,
		0,
		"serviceLevel"
		},
	{ ATF_POINTER, 9, offsetof(struct IncludedOpenTicketType, carrierNum),
		(ASN_TAG_CLASS_CONTEXT | (16 << 2)),
		0,
		&asn_DEF_carrierNum_19,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"carrierNum"
		},
	{ ATF_POINTER, 8, offsetof(struct IncludedOpenTicketType, carrierIA5),
		(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
		0,
		&asn_DEF_carrierIA5_21,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"carrierIA5"
		},
	{ ATF_POINTER, 7, offsetof(struct IncludedOpenTicketType, includedServiceBrands),
		(ASN_TAG_CLASS_CONTEXT | (18 << 2)),
		0,
		&asn_DEF_includedServiceBrands_23,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"includedServiceBrands"
		},
	{ ATF_POINTER, 6, offsetof(struct IncludedOpenTicketType, excludedServiceBrands),
		(ASN_TAG_CLASS_CONTEXT | (19 << 2)),
		0,
		&asn_DEF_excludedServiceBrands_25,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"excludedServiceBrands"
		},
	{ ATF_POINTER, 5, offsetof(struct IncludedOpenTicketType, tariffs),
		(ASN_TAG_CLASS_CONTEXT | (20 << 2)),
		0,
		&asn_DEF_tariffs_27,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"tariffs"
		},
	{ ATF_POINTER, 4, offsetof(struct IncludedOpenTicketType, infoText),
		(ASN_TAG_CLASS_CONTEXT | (21 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"infoText"
		},
	{ ATF_POINTER, 3, offsetof(struct IncludedOpenTicketType, includedTransportType),
		(ASN_TAG_CLASS_CONTEXT | (22 << 2)),
		0,
		&asn_DEF_includedTransportType_30,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"includedTransportType"
		},
	{ ATF_POINTER, 2, offsetof(struct IncludedOpenTicketType, excludedTransportType),
		(ASN_TAG_CLASS_CONTEXT | (23 << 2)),
		0,
		&asn_DEF_excludedTransportType_32,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"excludedTransportType"
		},
	{ ATF_POINTER, 1, offsetof(struct IncludedOpenTicketType, extension),
		(ASN_TAG_CLASS_CONTEXT | (24 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ExtensionData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"extension"
		},
};
static const int asn_MAP_IncludedOpenTicketType_oms_1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
static const ber_tlv_tag_t asn_DEF_IncludedOpenTicketType_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_IncludedOpenTicketType_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* productOwnerNum */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* productOwnerIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* productIdNum */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* productIdIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* externalIssuerId */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* issuerAutorizationId */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* stationCodeTable */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* validRegion */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 }, /* validFromDay */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 9, 0, 0 }, /* validFromTime */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 10, 0, 0 }, /* validFromUTCOffset */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 11, 0, 0 }, /* validUntilDay */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 12, 0, 0 }, /* validUntilTime */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 13, 0, 0 }, /* validUntilUTCOffset */
    { (ASN_TAG_CLASS_CONTEXT | (14 << 2)), 14, 0, 0 }, /* classCode */
    { (ASN_TAG_CLASS_CONTEXT | (15 << 2)), 15, 0, 0 }, /* serviceLevel */
    { (ASN_TAG_CLASS_CONTEXT | (16 << 2)), 16, 0, 0 }, /* carrierNum */
    { (ASN_TAG_CLASS_CONTEXT | (17 << 2)), 17, 0, 0 }, /* carrierIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (18 << 2)), 18, 0, 0 }, /* includedServiceBrands */
    { (ASN_TAG_CLASS_CONTEXT | (19 << 2)), 19, 0, 0 }, /* excludedServiceBrands */
    { (ASN_TAG_CLASS_CONTEXT | (20 << 2)), 20, 0, 0 }, /* tariffs */
    { (ASN_TAG_CLASS_CONTEXT | (21 << 2)), 21, 0, 0 }, /* infoText */
    { (ASN_TAG_CLASS_CONTEXT | (22 << 2)), 22, 0, 0 }, /* includedTransportType */
    { (ASN_TAG_CLASS_CONTEXT | (23 << 2)), 23, 0, 0 }, /* excludedTransportType */
    { (ASN_TAG_CLASS_CONTEXT | (24 << 2)), 24, 0, 0 } /* extension */
};
static asn_SEQUENCE_specifics_t asn_SPC_IncludedOpenTicketType_specs_1 = {
	sizeof(struct IncludedOpenTicketType),
	offsetof(struct IncludedOpenTicketType, _asn_ctx),
	asn_MAP_IncludedOpenTicketType_tag2el_1,
	25,	/* Count of tags in the map */
	asn_MAP_IncludedOpenTicketType_oms_1,	/* Optional members */
	25, 0,	/* Root/Additions */
	24,	/* Start extensions */
	26	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_IncludedOpenTicketType = {
	"IncludedOpenTicketType",
	"IncludedOpenTicketType",
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
	asn_DEF_IncludedOpenTicketType_tags_1,
	sizeof(asn_DEF_IncludedOpenTicketType_tags_1)
		/sizeof(asn_DEF_IncludedOpenTicketType_tags_1[0]), /* 1 */
	asn_DEF_IncludedOpenTicketType_tags_1,	/* Same as above */
	sizeof(asn_DEF_IncludedOpenTicketType_tags_1)
		/sizeof(asn_DEF_IncludedOpenTicketType_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_IncludedOpenTicketType_1,
	25,	/* Elements count */
	&asn_SPC_IncludedOpenTicketType_specs_1	/* Additional specs */
};

