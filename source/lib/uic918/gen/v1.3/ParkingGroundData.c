/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v1.3.4.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#include "ParkingGroundData.h"

static int
memb_fromParkingDate_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
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
memb_untilParkingDate_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((value >= 0 && value <= 32000)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_per_constraints_t asn_PER_memb_fromParkingDate_constr_5 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 9,  9, -1,  370 }	/* (-1..370) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_untilParkingDate_constr_6 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 9,  9,  0,  370 }	/* (0..370) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_productOwnerNum_constr_7 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 15,  15,  1,  32000 }	/* (1..32000) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_productIdNum_constr_9 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 15,  15,  0,  32000 }	/* (0..32000) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static int asn_DFL_6_set_0(int set_value, void **sptr) {
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
static asn_TYPE_member_t asn_MBR_vatDetail_20[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_VatDetailType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		""
		},
};
static const ber_tlv_tag_t asn_DEF_vatDetail_tags_20[] = {
	(ASN_TAG_CLASS_CONTEXT | (18 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_vatDetail_specs_20 = {
	sizeof(struct ParkingGroundData__vatDetail),
	offsetof(struct ParkingGroundData__vatDetail, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_vatDetail_20 = {
	"vatDetail",
	"vatDetail",
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
	asn_DEF_vatDetail_tags_20,
	sizeof(asn_DEF_vatDetail_tags_20)
		/sizeof(asn_DEF_vatDetail_tags_20[0]) - 1, /* 1 */
	asn_DEF_vatDetail_tags_20,	/* Same as above */
	sizeof(asn_DEF_vatDetail_tags_20)
		/sizeof(asn_DEF_vatDetail_tags_20[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_vatDetail_20,
	1,	/* Single element */
	&asn_SPC_vatDetail_specs_20	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_ParkingGroundData_1[] = {
	{ ATF_POINTER, 2, offsetof(struct ParkingGroundData, referenceIA5),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"referenceIA5"
		},
	{ ATF_POINTER, 1, offsetof(struct ParkingGroundData, referenceNum),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"referenceNum"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ParkingGroundData, parkingGroundId),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"parkingGroundId"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ParkingGroundData, fromParkingDate),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_fromParkingDate_constraint_1,
		&asn_PER_memb_fromParkingDate_constr_5,
		0,
		"fromParkingDate"
		},
	{ ATF_NOFLAGS, 6, offsetof(struct ParkingGroundData, untilParkingDate),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_untilParkingDate_constraint_1,
		&asn_PER_memb_untilParkingDate_constr_6,
		asn_DFL_6_set_0,	/* DEFAULT 0 */
		"untilParkingDate"
		},
	{ ATF_POINTER, 5, offsetof(struct ParkingGroundData, productOwnerNum),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_productOwnerNum_constraint_1,
		&asn_PER_memb_productOwnerNum_constr_7,
		0,
		"productOwnerNum"
		},
	{ ATF_POINTER, 4, offsetof(struct ParkingGroundData, productOwnerIA5),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"productOwnerIA5"
		},
	{ ATF_POINTER, 3, offsetof(struct ParkingGroundData, productIdNum),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_productIdNum_constraint_1,
		&asn_PER_memb_productIdNum_constr_9,
		0,
		"productIdNum"
		},
	{ ATF_POINTER, 2, offsetof(struct ParkingGroundData, productIdIA5),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"productIdIA5"
		},
	{ ATF_POINTER, 1, offsetof(struct ParkingGroundData, accessCode),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"accessCode"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ParkingGroundData, location),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"location"
		},
	{ ATF_POINTER, 9, offsetof(struct ParkingGroundData, stationCodeTable),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CodeTableType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		asn_DFL_13_set_0,	/* DEFAULT 0 */
		"stationCodeTable"
		},
	{ ATF_POINTER, 8, offsetof(struct ParkingGroundData, stationNum),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"stationNum"
		},
	{ ATF_POINTER, 7, offsetof(struct ParkingGroundData, stationIA5),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"stationIA5"
		},
	{ ATF_POINTER, 6, offsetof(struct ParkingGroundData, specialInformation),
		(ASN_TAG_CLASS_CONTEXT | (14 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"specialInformation"
		},
	{ ATF_POINTER, 5, offsetof(struct ParkingGroundData, entryTrack),
		(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"entryTrack"
		},
	{ ATF_POINTER, 4, offsetof(struct ParkingGroundData, numberPlate),
		(ASN_TAG_CLASS_CONTEXT | (16 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"numberPlate"
		},
	{ ATF_POINTER, 3, offsetof(struct ParkingGroundData, price),
		(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"price"
		},
	{ ATF_POINTER, 2, offsetof(struct ParkingGroundData, vatDetail),
		(ASN_TAG_CLASS_CONTEXT | (18 << 2)),
		0,
		&asn_DEF_vatDetail_20,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"vatDetail"
		},
	{ ATF_POINTER, 1, offsetof(struct ParkingGroundData, extension),
		(ASN_TAG_CLASS_CONTEXT | (19 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ExtensionData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"extension"
		},
};
static const int asn_MAP_ParkingGroundData_oms_1[] = { 0, 1, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
static const ber_tlv_tag_t asn_DEF_ParkingGroundData_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ParkingGroundData_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* referenceIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* referenceNum */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* parkingGroundId */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* fromParkingDate */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* untilParkingDate */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* productOwnerNum */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* productOwnerIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* productIdNum */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 }, /* productIdIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 9, 0, 0 }, /* accessCode */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 10, 0, 0 }, /* location */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 11, 0, 0 }, /* stationCodeTable */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 12, 0, 0 }, /* stationNum */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 13, 0, 0 }, /* stationIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (14 << 2)), 14, 0, 0 }, /* specialInformation */
    { (ASN_TAG_CLASS_CONTEXT | (15 << 2)), 15, 0, 0 }, /* entryTrack */
    { (ASN_TAG_CLASS_CONTEXT | (16 << 2)), 16, 0, 0 }, /* numberPlate */
    { (ASN_TAG_CLASS_CONTEXT | (17 << 2)), 17, 0, 0 }, /* price */
    { (ASN_TAG_CLASS_CONTEXT | (18 << 2)), 18, 0, 0 }, /* vatDetail */
    { (ASN_TAG_CLASS_CONTEXT | (19 << 2)), 19, 0, 0 } /* extension */
};
static asn_SEQUENCE_specifics_t asn_SPC_ParkingGroundData_specs_1 = {
	sizeof(struct ParkingGroundData),
	offsetof(struct ParkingGroundData, _asn_ctx),
	asn_MAP_ParkingGroundData_tag2el_1,
	20,	/* Count of tags in the map */
	asn_MAP_ParkingGroundData_oms_1,	/* Optional members */
	17, 0,	/* Root/Additions */
	19,	/* Start extensions */
	21	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_ParkingGroundData = {
	"ParkingGroundData",
	"ParkingGroundData",
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
	asn_DEF_ParkingGroundData_tags_1,
	sizeof(asn_DEF_ParkingGroundData_tags_1)
		/sizeof(asn_DEF_ParkingGroundData_tags_1[0]), /* 1 */
	asn_DEF_ParkingGroundData_tags_1,	/* Same as above */
	sizeof(asn_DEF_ParkingGroundData_tags_1)
		/sizeof(asn_DEF_ParkingGroundData_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_ParkingGroundData_1,
	20,	/* Elements count */
	&asn_SPC_ParkingGroundData_specs_1	/* Additional specs */
};

