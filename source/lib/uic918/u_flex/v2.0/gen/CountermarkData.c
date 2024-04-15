/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../../etc/UIC-barcode/misc/uicRailTicketData_v2.0.2.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#include "CountermarkData.h"

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
memb_NativeInteger_constraint_36(asn_TYPE_descriptor_t *td, const void *sptr,
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
memb_NativeInteger_constraint_38(asn_TYPE_descriptor_t *td, const void *sptr,
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
memb_numberOfCountermark_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 200)) {
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
memb_totalOfCountermarks_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 200)) {
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
	
	if((value >= -1 && value <= 700)) {
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
	
	if((value >= -1 && value <= 370)) {
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

static asn_per_constraints_t asn_PER_memb_Member_constr_33 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 15,  15,  1,  32000 }	/* (1..32000) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_Member_constr_37 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 15,  15,  1,  32000 }	/* (1..32000) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_Member_constr_39 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 15,  15,  1,  32000 }	/* (1..32000) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_productOwnerNum_constr_4 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 15,  15,  1,  32000 }	/* (1..32000) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_productIdNum_constr_6 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 16,  16,  0,  65535 }	/* (0..65535) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_numberOfCountermark_constr_10 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 8,  8,  1,  200 }	/* (1..200) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_totalOfCountermarks_constr_11 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 8,  8,  1,  200 }	/* (1..200) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_fromStationNum_constr_14 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 24, -1,  1,  9999999 }	/* (1..9999999) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_toStationNum_constr_16 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 24, -1,  1,  9999999 }	/* (1..9999999) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_validFromDay_constr_25 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 10,  10, -1,  700 }	/* (-1..700) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_validFromTime_constr_26 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 11,  11,  0,  1439 }	/* (0..1439) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_validFromUTCOffset_constr_27 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 7,  7, -60,  60 }	/* (-60..60) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_validUntilDay_constr_28 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 9,  9, -1,  370 }	/* (-1..370) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_validUntilTime_constr_29 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 11,  11,  0,  1439 }	/* (0..1439) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_validUntilUTCOffset_constr_30 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 7,  7, -60,  60 }	/* (-60..60) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static int asn_DFL_13_set_0(int set_value, void **sptr) {
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
static asn_TYPE_member_t asn_MBR_validRegion_21[] = {
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
static const ber_tlv_tag_t asn_DEF_validRegion_tags_21[] = {
	(ASN_TAG_CLASS_CONTEXT | (19 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_validRegion_specs_21 = {
	sizeof(struct CountermarkData__validRegion),
	offsetof(struct CountermarkData__validRegion, _asn_ctx),
	2,	/* XER encoding is XMLValueList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_validRegion_21 = {
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
	asn_DEF_validRegion_tags_21,
	sizeof(asn_DEF_validRegion_tags_21)
		/sizeof(asn_DEF_validRegion_tags_21[0]) - 1, /* 1 */
	asn_DEF_validRegion_tags_21,	/* Same as above */
	sizeof(asn_DEF_validRegion_tags_21)
		/sizeof(asn_DEF_validRegion_tags_21[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_validRegion_21,
	1,	/* Single element */
	&asn_SPC_validRegion_specs_21	/* Additional specs */
};

static int asn_DFL_25_set_0(int set_value, void **sptr) {
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
static int asn_DFL_28_set_0(int set_value, void **sptr) {
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
static int asn_DFL_31_set_2(int set_value, void **sptr) {
	TravelClassType_t *st = *sptr;
	
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
static asn_TYPE_member_t asn_MBR_carrierNum_32[] = {
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
static const ber_tlv_tag_t asn_DEF_carrierNum_tags_32[] = {
	(ASN_TAG_CLASS_CONTEXT | (29 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_carrierNum_specs_32 = {
	sizeof(struct CountermarkData__carrierNum),
	offsetof(struct CountermarkData__carrierNum, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_carrierNum_32 = {
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
	asn_DEF_carrierNum_tags_32,
	sizeof(asn_DEF_carrierNum_tags_32)
		/sizeof(asn_DEF_carrierNum_tags_32[0]) - 1, /* 1 */
	asn_DEF_carrierNum_tags_32,	/* Same as above */
	sizeof(asn_DEF_carrierNum_tags_32)
		/sizeof(asn_DEF_carrierNum_tags_32[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_carrierNum_32,
	1,	/* Single element */
	&asn_SPC_carrierNum_specs_32	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_carrierIA5_34[] = {
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
static const ber_tlv_tag_t asn_DEF_carrierIA5_tags_34[] = {
	(ASN_TAG_CLASS_CONTEXT | (30 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_carrierIA5_specs_34 = {
	sizeof(struct CountermarkData__carrierIA5),
	offsetof(struct CountermarkData__carrierIA5, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_carrierIA5_34 = {
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
	asn_DEF_carrierIA5_tags_34,
	sizeof(asn_DEF_carrierIA5_tags_34)
		/sizeof(asn_DEF_carrierIA5_tags_34[0]) - 1, /* 1 */
	asn_DEF_carrierIA5_tags_34,	/* Same as above */
	sizeof(asn_DEF_carrierIA5_tags_34)
		/sizeof(asn_DEF_carrierIA5_tags_34[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_carrierIA5_34,
	1,	/* Single element */
	&asn_SPC_carrierIA5_specs_34	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_includedServiceBrands_36[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NativeInteger,
		memb_NativeInteger_constraint_36,
		&asn_PER_memb_Member_constr_37,
		0,
		""
		},
};
static const ber_tlv_tag_t asn_DEF_includedServiceBrands_tags_36[] = {
	(ASN_TAG_CLASS_CONTEXT | (31 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_includedServiceBrands_specs_36 = {
	sizeof(struct CountermarkData__includedServiceBrands),
	offsetof(struct CountermarkData__includedServiceBrands, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_includedServiceBrands_36 = {
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
	asn_DEF_includedServiceBrands_tags_36,
	sizeof(asn_DEF_includedServiceBrands_tags_36)
		/sizeof(asn_DEF_includedServiceBrands_tags_36[0]) - 1, /* 1 */
	asn_DEF_includedServiceBrands_tags_36,	/* Same as above */
	sizeof(asn_DEF_includedServiceBrands_tags_36)
		/sizeof(asn_DEF_includedServiceBrands_tags_36[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_includedServiceBrands_36,
	1,	/* Single element */
	&asn_SPC_includedServiceBrands_specs_36	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_excludedServiceBrands_38[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NativeInteger,
		memb_NativeInteger_constraint_38,
		&asn_PER_memb_Member_constr_39,
		0,
		""
		},
};
static const ber_tlv_tag_t asn_DEF_excludedServiceBrands_tags_38[] = {
	(ASN_TAG_CLASS_CONTEXT | (32 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_excludedServiceBrands_specs_38 = {
	sizeof(struct CountermarkData__excludedServiceBrands),
	offsetof(struct CountermarkData__excludedServiceBrands, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_excludedServiceBrands_38 = {
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
	asn_DEF_excludedServiceBrands_tags_38,
	sizeof(asn_DEF_excludedServiceBrands_tags_38)
		/sizeof(asn_DEF_excludedServiceBrands_tags_38[0]) - 1, /* 1 */
	asn_DEF_excludedServiceBrands_tags_38,	/* Same as above */
	sizeof(asn_DEF_excludedServiceBrands_tags_38)
		/sizeof(asn_DEF_excludedServiceBrands_tags_38[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_excludedServiceBrands_38,
	1,	/* Single element */
	&asn_SPC_excludedServiceBrands_specs_38	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_CountermarkData_1[] = {
	{ ATF_POINTER, 8, offsetof(struct CountermarkData, referenceIA5),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"referenceIA5"
		},
	{ ATF_POINTER, 7, offsetof(struct CountermarkData, referenceNum),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"referenceNum"
		},
	{ ATF_POINTER, 6, offsetof(struct CountermarkData, productOwnerNum),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_productOwnerNum_constraint_1,
		&asn_PER_memb_productOwnerNum_constr_4,
		0,
		"productOwnerNum"
		},
	{ ATF_POINTER, 5, offsetof(struct CountermarkData, productOwnerIA5),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"productOwnerIA5"
		},
	{ ATF_POINTER, 4, offsetof(struct CountermarkData, productIdNum),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_productIdNum_constraint_1,
		&asn_PER_memb_productIdNum_constr_6,
		0,
		"productIdNum"
		},
	{ ATF_POINTER, 3, offsetof(struct CountermarkData, productIdIA5),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"productIdIA5"
		},
	{ ATF_POINTER, 2, offsetof(struct CountermarkData, ticketReferenceIA5),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"ticketReferenceIA5"
		},
	{ ATF_POINTER, 1, offsetof(struct CountermarkData, ticketReferenceNum),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"ticketReferenceNum"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CountermarkData, numberOfCountermark),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_numberOfCountermark_constraint_1,
		&asn_PER_memb_numberOfCountermark_constr_10,
		0,
		"numberOfCountermark"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CountermarkData, totalOfCountermarks),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_totalOfCountermarks_constraint_1,
		&asn_PER_memb_totalOfCountermarks_constr_11,
		0,
		"totalOfCountermarks"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CountermarkData, groupName),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"groupName"
		},
	{ ATF_POINTER, 9, offsetof(struct CountermarkData, stationCodeTable),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CodeTableType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		asn_DFL_13_set_0,	/* DEFAULT 0 */
		"stationCodeTable"
		},
	{ ATF_POINTER, 8, offsetof(struct CountermarkData, fromStationNum),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_fromStationNum_constraint_1,
		&asn_PER_memb_fromStationNum_constr_14,
		0,
		"fromStationNum"
		},
	{ ATF_POINTER, 7, offsetof(struct CountermarkData, fromStationIA5),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"fromStationIA5"
		},
	{ ATF_POINTER, 6, offsetof(struct CountermarkData, toStationNum),
		(ASN_TAG_CLASS_CONTEXT | (14 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_toStationNum_constraint_1,
		&asn_PER_memb_toStationNum_constr_16,
		0,
		"toStationNum"
		},
	{ ATF_POINTER, 5, offsetof(struct CountermarkData, toStationIA5),
		(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"toStationIA5"
		},
	{ ATF_POINTER, 4, offsetof(struct CountermarkData, fromStationNameUTF8),
		(ASN_TAG_CLASS_CONTEXT | (16 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"fromStationNameUTF8"
		},
	{ ATF_POINTER, 3, offsetof(struct CountermarkData, toStationNameUTF8),
		(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"toStationNameUTF8"
		},
	{ ATF_POINTER, 2, offsetof(struct CountermarkData, validRegionDesc),
		(ASN_TAG_CLASS_CONTEXT | (18 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"validRegionDesc"
		},
	{ ATF_POINTER, 1, offsetof(struct CountermarkData, validRegion),
		(ASN_TAG_CLASS_CONTEXT | (19 << 2)),
		0,
		&asn_DEF_validRegion_21,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"validRegion"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CountermarkData, returnIncluded),
		(ASN_TAG_CLASS_CONTEXT | (20 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"returnIncluded"
		},
	{ ATF_POINTER, 14, offsetof(struct CountermarkData, returnDescription),
		(ASN_TAG_CLASS_CONTEXT | (21 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ReturnRouteDescriptionType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"returnDescription"
		},
	{ ATF_NOFLAGS, 13, offsetof(struct CountermarkData, validFromDay),
		(ASN_TAG_CLASS_CONTEXT | (22 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_validFromDay_constraint_1,
		&asn_PER_memb_validFromDay_constr_25,
		asn_DFL_25_set_0,	/* DEFAULT 0 */
		"validFromDay"
		},
	{ ATF_POINTER, 12, offsetof(struct CountermarkData, validFromTime),
		(ASN_TAG_CLASS_CONTEXT | (23 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_validFromTime_constraint_1,
		&asn_PER_memb_validFromTime_constr_26,
		0,
		"validFromTime"
		},
	{ ATF_POINTER, 11, offsetof(struct CountermarkData, validFromUTCOffset),
		(ASN_TAG_CLASS_CONTEXT | (24 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_validFromUTCOffset_constraint_1,
		&asn_PER_memb_validFromUTCOffset_constr_27,
		0,
		"validFromUTCOffset"
		},
	{ ATF_NOFLAGS, 10, offsetof(struct CountermarkData, validUntilDay),
		(ASN_TAG_CLASS_CONTEXT | (25 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_validUntilDay_constraint_1,
		&asn_PER_memb_validUntilDay_constr_28,
		asn_DFL_28_set_0,	/* DEFAULT 0 */
		"validUntilDay"
		},
	{ ATF_POINTER, 9, offsetof(struct CountermarkData, validUntilTime),
		(ASN_TAG_CLASS_CONTEXT | (26 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_validUntilTime_constraint_1,
		&asn_PER_memb_validUntilTime_constr_29,
		0,
		"validUntilTime"
		},
	{ ATF_POINTER, 8, offsetof(struct CountermarkData, validUntilUTCOffset),
		(ASN_TAG_CLASS_CONTEXT | (27 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_validUntilUTCOffset_constraint_1,
		&asn_PER_memb_validUntilUTCOffset_constr_30,
		0,
		"validUntilUTCOffset"
		},
	{ ATF_POINTER, 7, offsetof(struct CountermarkData, classCode),
		(ASN_TAG_CLASS_CONTEXT | (28 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TravelClassType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		asn_DFL_31_set_2,	/* DEFAULT 2 */
		"classCode"
		},
	{ ATF_POINTER, 6, offsetof(struct CountermarkData, carrierNum),
		(ASN_TAG_CLASS_CONTEXT | (29 << 2)),
		0,
		&asn_DEF_carrierNum_32,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"carrierNum"
		},
	{ ATF_POINTER, 5, offsetof(struct CountermarkData, carrierIA5),
		(ASN_TAG_CLASS_CONTEXT | (30 << 2)),
		0,
		&asn_DEF_carrierIA5_34,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"carrierIA5"
		},
	{ ATF_POINTER, 4, offsetof(struct CountermarkData, includedServiceBrands),
		(ASN_TAG_CLASS_CONTEXT | (31 << 2)),
		0,
		&asn_DEF_includedServiceBrands_36,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"includedServiceBrands"
		},
	{ ATF_POINTER, 3, offsetof(struct CountermarkData, excludedServiceBrands),
		(ASN_TAG_CLASS_CONTEXT | (32 << 2)),
		0,
		&asn_DEF_excludedServiceBrands_38,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"excludedServiceBrands"
		},
	{ ATF_POINTER, 2, offsetof(struct CountermarkData, infoText),
		(ASN_TAG_CLASS_CONTEXT | (33 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"infoText"
		},
	{ ATF_POINTER, 1, offsetof(struct CountermarkData, extension),
		(ASN_TAG_CLASS_CONTEXT | (34 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ExtensionData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"extension"
		},
};
static const int asn_MAP_CountermarkData_oms_1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34 };
static const ber_tlv_tag_t asn_DEF_CountermarkData_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_CountermarkData_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* referenceIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* referenceNum */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* productOwnerNum */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* productOwnerIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* productIdNum */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* productIdIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* ticketReferenceIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* ticketReferenceNum */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 }, /* numberOfCountermark */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 9, 0, 0 }, /* totalOfCountermarks */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 10, 0, 0 }, /* groupName */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 11, 0, 0 }, /* stationCodeTable */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 12, 0, 0 }, /* fromStationNum */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 13, 0, 0 }, /* fromStationIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (14 << 2)), 14, 0, 0 }, /* toStationNum */
    { (ASN_TAG_CLASS_CONTEXT | (15 << 2)), 15, 0, 0 }, /* toStationIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (16 << 2)), 16, 0, 0 }, /* fromStationNameUTF8 */
    { (ASN_TAG_CLASS_CONTEXT | (17 << 2)), 17, 0, 0 }, /* toStationNameUTF8 */
    { (ASN_TAG_CLASS_CONTEXT | (18 << 2)), 18, 0, 0 }, /* validRegionDesc */
    { (ASN_TAG_CLASS_CONTEXT | (19 << 2)), 19, 0, 0 }, /* validRegion */
    { (ASN_TAG_CLASS_CONTEXT | (20 << 2)), 20, 0, 0 }, /* returnIncluded */
    { (ASN_TAG_CLASS_CONTEXT | (21 << 2)), 21, 0, 0 }, /* returnDescription */
    { (ASN_TAG_CLASS_CONTEXT | (22 << 2)), 22, 0, 0 }, /* validFromDay */
    { (ASN_TAG_CLASS_CONTEXT | (23 << 2)), 23, 0, 0 }, /* validFromTime */
    { (ASN_TAG_CLASS_CONTEXT | (24 << 2)), 24, 0, 0 }, /* validFromUTCOffset */
    { (ASN_TAG_CLASS_CONTEXT | (25 << 2)), 25, 0, 0 }, /* validUntilDay */
    { (ASN_TAG_CLASS_CONTEXT | (26 << 2)), 26, 0, 0 }, /* validUntilTime */
    { (ASN_TAG_CLASS_CONTEXT | (27 << 2)), 27, 0, 0 }, /* validUntilUTCOffset */
    { (ASN_TAG_CLASS_CONTEXT | (28 << 2)), 28, 0, 0 }, /* classCode */
    { (ASN_TAG_CLASS_CONTEXT | (29 << 2)), 29, 0, 0 }, /* carrierNum */
    { (ASN_TAG_CLASS_CONTEXT | (30 << 2)), 30, 0, 0 }, /* carrierIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (31 << 2)), 31, 0, 0 }, /* includedServiceBrands */
    { (ASN_TAG_CLASS_CONTEXT | (32 << 2)), 32, 0, 0 }, /* excludedServiceBrands */
    { (ASN_TAG_CLASS_CONTEXT | (33 << 2)), 33, 0, 0 }, /* infoText */
    { (ASN_TAG_CLASS_CONTEXT | (34 << 2)), 34, 0, 0 } /* extension */
};
static asn_SEQUENCE_specifics_t asn_SPC_CountermarkData_specs_1 = {
	sizeof(struct CountermarkData),
	offsetof(struct CountermarkData, _asn_ctx),
	asn_MAP_CountermarkData_tag2el_1,
	35,	/* Count of tags in the map */
	asn_MAP_CountermarkData_oms_1,	/* Optional members */
	31, 0,	/* Root/Additions */
	34,	/* Start extensions */
	36	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_CountermarkData = {
	"CountermarkData",
	"CountermarkData",
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
	asn_DEF_CountermarkData_tags_1,
	sizeof(asn_DEF_CountermarkData_tags_1)
		/sizeof(asn_DEF_CountermarkData_tags_1[0]), /* 1 */
	asn_DEF_CountermarkData_tags_1,	/* Same as above */
	sizeof(asn_DEF_CountermarkData_tags_1)
		/sizeof(asn_DEF_CountermarkData_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_CountermarkData_1,
	35,	/* Elements count */
	&asn_SPC_CountermarkData_specs_1	/* Additional specs */
};

