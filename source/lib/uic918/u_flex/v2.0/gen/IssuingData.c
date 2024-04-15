/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../../etc/UIC-barcode/misc/uicRailTicketData_v2.0.2.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#include "IssuingData.h"

static int check_permitted_alphabet_13(const void *sptr) {
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
memb_securityProviderNum_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
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
memb_issuerNum_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
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
memb_issuingYear_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 2016 && value <= 2269)) {
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
memb_issuingDay_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 366)) {
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
memb_issuingTime_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
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
memb_currency_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size == 3)
		 && !check_permitted_alphabet_13(st)) {
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
memb_currencyFract_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 3)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_per_constraints_t asn_PER_memb_securityProviderNum_constr_2 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 15,  15,  1,  32000 }	/* (1..32000) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_issuerNum_constr_4 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 15,  15,  1,  32000 }	/* (1..32000) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_issuingYear_constr_6 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 8,  8,  2016,  2269 }	/* (2016..2269) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_issuingDay_constr_7 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 9,  9,  1,  366 }	/* (1..366) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_issuingTime_constr_8 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 11,  11,  0,  1439 }	/* (0..1439) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_currency_constr_13 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 7,  7,  0,  127 }	/* (0..127) */,
	{ APC_CONSTRAINED,	 0,  0,  3,  3 }	/* (SIZE(3..3)) */,
	0, 0	/* No PER character map necessary */
};
static asn_per_constraints_t asn_PER_memb_currencyFract_constr_14 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 2,  2,  1,  3 }	/* (1..3) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static int asn_DFL_13_set(int set_value, void **sptr) {
	static uint8_t defv[] = "EUR";
	IA5String_t *st = *sptr;
	
	if(!st) {
		if(!set_value) return -1;	/* Not a default value */
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	if(set_value) {
		uint8_t *ptr = MALLOC(sizeof(defv));
		if(!ptr) return -1;
		memcpy(ptr, &defv, sizeof(defv));
		FREEMEM(st->buf);
		st->buf = ptr;
		st->size = sizeof(defv) - 1;
		return 0;
	} else {
		if(st->size != (sizeof(defv) - 1)
		|| memcmp(st->buf, &defv, sizeof(defv) - 1))
			return 0;
		return 1;
	}
	
}
static int asn_DFL_14_set_2(int set_value, void **sptr) {
	long *st = *sptr;
	
	if(!st) {
		if(!set_value) return -1;	/* Not a default value */
		st = (*sptr = CALLOC(1, sizeof(*st)));
		if(!st) return -1;
	}
	
	if(set_value) {
		/* Install default value 2 */
		*st = 2;
		return 0;
	} else {
		/* Test default value 2 */
		return (*st == 2);
	}
}
static asn_TYPE_member_t asn_MBR_IssuingData_1[] = {
	{ ATF_POINTER, 4, offsetof(struct IssuingData, securityProviderNum),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_securityProviderNum_constraint_1,
		&asn_PER_memb_securityProviderNum_constr_2,
		0,
		"securityProviderNum"
		},
	{ ATF_POINTER, 3, offsetof(struct IssuingData, securityProviderIA5),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"securityProviderIA5"
		},
	{ ATF_POINTER, 2, offsetof(struct IssuingData, issuerNum),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_issuerNum_constraint_1,
		&asn_PER_memb_issuerNum_constr_4,
		0,
		"issuerNum"
		},
	{ ATF_POINTER, 1, offsetof(struct IssuingData, issuerIA5),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"issuerIA5"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct IssuingData, issuingYear),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_issuingYear_constraint_1,
		&asn_PER_memb_issuingYear_constr_6,
		0,
		"issuingYear"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct IssuingData, issuingDay),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_issuingDay_constraint_1,
		&asn_PER_memb_issuingDay_constr_7,
		0,
		"issuingDay"
		},
	{ ATF_POINTER, 2, offsetof(struct IssuingData, issuingTime),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_issuingTime_constraint_1,
		&asn_PER_memb_issuingTime_constr_8,
		0,
		"issuingTime"
		},
	{ ATF_POINTER, 1, offsetof(struct IssuingData, issuerName),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"issuerName"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct IssuingData, specimen),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"specimen"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct IssuingData, securePaperTicket),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"securePaperTicket"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct IssuingData, activated),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"activated"
		},
	{ ATF_POINTER, 8, offsetof(struct IssuingData, currency),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		memb_currency_constraint_1,
		&asn_PER_memb_currency_constr_13,
		asn_DFL_13_set,	/* DEFAULT "EUR" */
		"currency"
		},
	{ ATF_POINTER, 7, offsetof(struct IssuingData, currencyFract),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_currencyFract_constraint_1,
		&asn_PER_memb_currencyFract_constr_14,
		asn_DFL_14_set_2,	/* DEFAULT 2 */
		"currencyFract"
		},
	{ ATF_POINTER, 6, offsetof(struct IssuingData, issuerPNR),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"issuerPNR"
		},
	{ ATF_POINTER, 5, offsetof(struct IssuingData, extension),
		(ASN_TAG_CLASS_CONTEXT | (14 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ExtensionData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"extension"
		},
	{ ATF_POINTER, 4, offsetof(struct IssuingData, issuedOnTrainNum),
		(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"issuedOnTrainNum"
		},
	{ ATF_POINTER, 3, offsetof(struct IssuingData, issuedOnTrainIA5),
		(ASN_TAG_CLASS_CONTEXT | (16 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"issuedOnTrainIA5"
		},
	{ ATF_POINTER, 2, offsetof(struct IssuingData, issuedOnLine),
		(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"issuedOnLine"
		},
	{ ATF_POINTER, 1, offsetof(struct IssuingData, pointOfSale),
		(ASN_TAG_CLASS_CONTEXT | (18 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_GeoCoordinateType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"pointOfSale"
		},
};
static const int asn_MAP_IssuingData_oms_1[] = { 0, 1, 2, 3, 6, 7, 11, 12, 13, 14, 15, 16, 17, 18 };
static const ber_tlv_tag_t asn_DEF_IssuingData_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_IssuingData_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* securityProviderNum */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* securityProviderIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* issuerNum */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* issuerIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* issuingYear */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* issuingDay */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* issuingTime */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* issuerName */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 }, /* specimen */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 9, 0, 0 }, /* securePaperTicket */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 10, 0, 0 }, /* activated */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 11, 0, 0 }, /* currency */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 12, 0, 0 }, /* currencyFract */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 13, 0, 0 }, /* issuerPNR */
    { (ASN_TAG_CLASS_CONTEXT | (14 << 2)), 14, 0, 0 }, /* extension */
    { (ASN_TAG_CLASS_CONTEXT | (15 << 2)), 15, 0, 0 }, /* issuedOnTrainNum */
    { (ASN_TAG_CLASS_CONTEXT | (16 << 2)), 16, 0, 0 }, /* issuedOnTrainIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (17 << 2)), 17, 0, 0 }, /* issuedOnLine */
    { (ASN_TAG_CLASS_CONTEXT | (18 << 2)), 18, 0, 0 } /* pointOfSale */
};
static asn_SEQUENCE_specifics_t asn_SPC_IssuingData_specs_1 = {
	sizeof(struct IssuingData),
	offsetof(struct IssuingData, _asn_ctx),
	asn_MAP_IssuingData_tag2el_1,
	19,	/* Count of tags in the map */
	asn_MAP_IssuingData_oms_1,	/* Optional members */
	14, 0,	/* Root/Additions */
	18,	/* Start extensions */
	20	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_IssuingData = {
	"IssuingData",
	"IssuingData",
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
	asn_DEF_IssuingData_tags_1,
	sizeof(asn_DEF_IssuingData_tags_1)
		/sizeof(asn_DEF_IssuingData_tags_1[0]), /* 1 */
	asn_DEF_IssuingData_tags_1,	/* Same as above */
	sizeof(asn_DEF_IssuingData_tags_1)
		/sizeof(asn_DEF_IssuingData_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_IssuingData_1,
	19,	/* Elements count */
	&asn_SPC_IssuingData_specs_1	/* Additional specs */
};

