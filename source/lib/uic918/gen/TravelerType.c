/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../doc/barcode.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#include "TravelerType.h"

static int check_permitted_alphabet_7(const void *sptr) {
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
memb_title_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size >= 1 && size <= 3)
		 && !check_permitted_alphabet_7(st)) {
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
memb_yearOfBirth_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1901 && value <= 2155)) {
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
memb_dayOfBirth_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 370)) {
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
memb_countryOfResidence_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 999)) {
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
memb_countryOfPassport_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 999)) {
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
memb_countryOfIdCard_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 999)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_per_constraints_t asn_PER_memb_title_constr_7 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 7,  7,  0,  127 }	/* (0..127) */,
	{ APC_CONSTRAINED,	 2,  2,  1,  3 }	/* (SIZE(1..3)) */,
	0, 0	/* No PER character map necessary */
};
static asn_per_constraints_t asn_PER_memb_yearOfBirth_constr_11 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 8,  8,  1901,  2155 }	/* (1901..2155) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_dayOfBirth_constr_12 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 9,  9,  0,  370 }	/* (0..370) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_countryOfResidence_constr_16 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 10,  10,  1,  999 }	/* (1..999) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_countryOfPassport_constr_17 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 10,  10,  1,  999 }	/* (1..999) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_countryOfIdCard_constr_18 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 10,  10,  1,  999 }	/* (1..999) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_status_19[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_CustomerStatusType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		""
		},
};
static const ber_tlv_tag_t asn_DEF_status_tags_19[] = {
	(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_status_specs_19 = {
	sizeof(struct TravelerType__status),
	offsetof(struct TravelerType__status, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_status_19 = {
	"status",
	"status",
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
	asn_DEF_status_tags_19,
	sizeof(asn_DEF_status_tags_19)
		/sizeof(asn_DEF_status_tags_19[0]) - 1, /* 1 */
	asn_DEF_status_tags_19,	/* Same as above */
	sizeof(asn_DEF_status_tags_19)
		/sizeof(asn_DEF_status_tags_19[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_status_19,
	1,	/* Single element */
	&asn_SPC_status_specs_19	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_TravelerType_1[] = {
	{ ATF_POINTER, 11, offsetof(struct TravelerType, firstName),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"firstName"
		},
	{ ATF_POINTER, 10, offsetof(struct TravelerType, secondName),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"secondName"
		},
	{ ATF_POINTER, 9, offsetof(struct TravelerType, lastName),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"lastName"
		},
	{ ATF_POINTER, 8, offsetof(struct TravelerType, idCard),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"idCard"
		},
	{ ATF_POINTER, 7, offsetof(struct TravelerType, passportId),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"passportId"
		},
	{ ATF_POINTER, 6, offsetof(struct TravelerType, title),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		memb_title_constraint_1,
		&asn_PER_memb_title_constr_7,
		0,
		"title"
		},
	{ ATF_POINTER, 5, offsetof(struct TravelerType, gender),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_GenderType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"gender"
		},
	{ ATF_POINTER, 4, offsetof(struct TravelerType, customerIdIA5),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"customerIdIA5"
		},
	{ ATF_POINTER, 3, offsetof(struct TravelerType, customerIdNum),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"customerIdNum"
		},
	{ ATF_POINTER, 2, offsetof(struct TravelerType, yearOfBirth),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_yearOfBirth_constraint_1,
		&asn_PER_memb_yearOfBirth_constr_11,
		0,
		"yearOfBirth"
		},
	{ ATF_POINTER, 1, offsetof(struct TravelerType, dayOfBirth),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_dayOfBirth_constraint_1,
		&asn_PER_memb_dayOfBirth_constr_12,
		0,
		"dayOfBirth"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TravelerType, ticketHolder),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"ticketHolder"
		},
	{ ATF_POINTER, 6, offsetof(struct TravelerType, passengerType),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PassengerType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"passengerType"
		},
	{ ATF_POINTER, 5, offsetof(struct TravelerType, passengerWithReducedMobility),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"passengerWithReducedMobility"
		},
	{ ATF_POINTER, 4, offsetof(struct TravelerType, countryOfResidence),
		(ASN_TAG_CLASS_CONTEXT | (14 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_countryOfResidence_constraint_1,
		&asn_PER_memb_countryOfResidence_constr_16,
		0,
		"countryOfResidence"
		},
	{ ATF_POINTER, 3, offsetof(struct TravelerType, countryOfPassport),
		(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_countryOfPassport_constraint_1,
		&asn_PER_memb_countryOfPassport_constr_17,
		0,
		"countryOfPassport"
		},
	{ ATF_POINTER, 2, offsetof(struct TravelerType, countryOfIdCard),
		(ASN_TAG_CLASS_CONTEXT | (16 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_countryOfIdCard_constraint_1,
		&asn_PER_memb_countryOfIdCard_constr_18,
		0,
		"countryOfIdCard"
		},
	{ ATF_POINTER, 1, offsetof(struct TravelerType, status),
		(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
		0,
		&asn_DEF_status_19,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"status"
		},
};
static const int asn_MAP_TravelerType_oms_1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 17 };
static const ber_tlv_tag_t asn_DEF_TravelerType_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_TravelerType_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* firstName */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* secondName */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* lastName */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* idCard */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* passportId */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* title */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* gender */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* customerIdIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 }, /* customerIdNum */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 9, 0, 0 }, /* yearOfBirth */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 10, 0, 0 }, /* dayOfBirth */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 11, 0, 0 }, /* ticketHolder */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 12, 0, 0 }, /* passengerType */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 13, 0, 0 }, /* passengerWithReducedMobility */
    { (ASN_TAG_CLASS_CONTEXT | (14 << 2)), 14, 0, 0 }, /* countryOfResidence */
    { (ASN_TAG_CLASS_CONTEXT | (15 << 2)), 15, 0, 0 }, /* countryOfPassport */
    { (ASN_TAG_CLASS_CONTEXT | (16 << 2)), 16, 0, 0 }, /* countryOfIdCard */
    { (ASN_TAG_CLASS_CONTEXT | (17 << 2)), 17, 0, 0 } /* status */
};
static asn_SEQUENCE_specifics_t asn_SPC_TravelerType_specs_1 = {
	sizeof(struct TravelerType),
	offsetof(struct TravelerType, _asn_ctx),
	asn_MAP_TravelerType_tag2el_1,
	18,	/* Count of tags in the map */
	asn_MAP_TravelerType_oms_1,	/* Optional members */
	17, 0,	/* Root/Additions */
	17,	/* Start extensions */
	19	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_TravelerType = {
	"TravelerType",
	"TravelerType",
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
	asn_DEF_TravelerType_tags_1,
	sizeof(asn_DEF_TravelerType_tags_1)
		/sizeof(asn_DEF_TravelerType_tags_1[0]), /* 1 */
	asn_DEF_TravelerType_tags_1,	/* Same as above */
	sizeof(asn_DEF_TravelerType_tags_1)
		/sizeof(asn_DEF_TravelerType_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_TravelerType_1,
	18,	/* Elements count */
	&asn_SPC_TravelerType_specs_1	/* Additional specs */
};

