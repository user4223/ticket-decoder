/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v1.3.4.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#include "TravelerData.h"

static int check_permitted_alphabet_4(const void *sptr) {
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
memb_preferredLanguage_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
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
	
	if((size == 2)
		 && !check_permitted_alphabet_4(st)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_per_constraints_t asn_PER_memb_preferredLanguage_constr_4 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 7,  7,  0,  127 }	/* (0..127) */,
	{ APC_CONSTRAINED,	 0,  0,  2,  2 }	/* (SIZE(2..2)) */,
	0, 0	/* No PER character map necessary */
};
static asn_TYPE_member_t asn_MBR_traveler_2[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_TravelerType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		""
		},
};
static const ber_tlv_tag_t asn_DEF_traveler_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_traveler_specs_2 = {
	sizeof(struct TravelerData__traveler),
	offsetof(struct TravelerData__traveler, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_traveler_2 = {
	"traveler",
	"traveler",
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
	asn_DEF_traveler_tags_2,
	sizeof(asn_DEF_traveler_tags_2)
		/sizeof(asn_DEF_traveler_tags_2[0]) - 1, /* 1 */
	asn_DEF_traveler_tags_2,	/* Same as above */
	sizeof(asn_DEF_traveler_tags_2)
		/sizeof(asn_DEF_traveler_tags_2[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_traveler_2,
	1,	/* Single element */
	&asn_SPC_traveler_specs_2	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_TravelerData_1[] = {
	{ ATF_POINTER, 3, offsetof(struct TravelerData, traveler),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		0,
		&asn_DEF_traveler_2,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"traveler"
		},
	{ ATF_POINTER, 2, offsetof(struct TravelerData, preferredLanguage),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		memb_preferredLanguage_constraint_1,
		&asn_PER_memb_preferredLanguage_constr_4,
		0,
		"preferredLanguage"
		},
	{ ATF_POINTER, 1, offsetof(struct TravelerData, groupName),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"groupName"
		},
};
static const int asn_MAP_TravelerData_oms_1[] = { 0, 1, 2 };
static const ber_tlv_tag_t asn_DEF_TravelerData_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_TravelerData_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* traveler */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* preferredLanguage */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* groupName */
};
static asn_SEQUENCE_specifics_t asn_SPC_TravelerData_specs_1 = {
	sizeof(struct TravelerData),
	offsetof(struct TravelerData, _asn_ctx),
	asn_MAP_TravelerData_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_TravelerData_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	2,	/* Start extensions */
	4	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_TravelerData = {
	"TravelerData",
	"TravelerData",
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
	asn_DEF_TravelerData_tags_1,
	sizeof(asn_DEF_TravelerData_tags_1)
		/sizeof(asn_DEF_TravelerData_tags_1[0]), /* 1 */
	asn_DEF_TravelerData_tags_1,	/* Same as above */
	sizeof(asn_DEF_TravelerData_tags_1)
		/sizeof(asn_DEF_TravelerData_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_TravelerData_1,
	3,	/* Elements count */
	&asn_SPC_TravelerData_specs_1	/* Additional specs */
};

