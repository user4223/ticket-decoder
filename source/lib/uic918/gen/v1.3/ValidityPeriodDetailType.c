/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v1.3.4.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#include "ValidityPeriodDetailType.h"

static asn_TYPE_member_t asn_MBR_validityPeriod_2[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_ValidityPeriodType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		""
		},
};
static const ber_tlv_tag_t asn_DEF_validityPeriod_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_validityPeriod_specs_2 = {
	sizeof(struct ValidityPeriodDetailType__validityPeriod),
	offsetof(struct ValidityPeriodDetailType__validityPeriod, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_validityPeriod_2 = {
	"validityPeriod",
	"validityPeriod",
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
	asn_DEF_validityPeriod_tags_2,
	sizeof(asn_DEF_validityPeriod_tags_2)
		/sizeof(asn_DEF_validityPeriod_tags_2[0]) - 1, /* 1 */
	asn_DEF_validityPeriod_tags_2,	/* Same as above */
	sizeof(asn_DEF_validityPeriod_tags_2)
		/sizeof(asn_DEF_validityPeriod_tags_2[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_validityPeriod_2,
	1,	/* Single element */
	&asn_SPC_validityPeriod_specs_2	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_excludedTimeRange_4[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_TimeRangeType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		""
		},
};
static const ber_tlv_tag_t asn_DEF_excludedTimeRange_tags_4[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_excludedTimeRange_specs_4 = {
	sizeof(struct ValidityPeriodDetailType__excludedTimeRange),
	offsetof(struct ValidityPeriodDetailType__excludedTimeRange, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_excludedTimeRange_4 = {
	"excludedTimeRange",
	"excludedTimeRange",
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
	asn_DEF_excludedTimeRange_tags_4,
	sizeof(asn_DEF_excludedTimeRange_tags_4)
		/sizeof(asn_DEF_excludedTimeRange_tags_4[0]) - 1, /* 1 */
	asn_DEF_excludedTimeRange_tags_4,	/* Same as above */
	sizeof(asn_DEF_excludedTimeRange_tags_4)
		/sizeof(asn_DEF_excludedTimeRange_tags_4[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_excludedTimeRange_4,
	1,	/* Single element */
	&asn_SPC_excludedTimeRange_specs_4	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_ValidityPeriodDetailType_1[] = {
	{ ATF_POINTER, 2, offsetof(struct ValidityPeriodDetailType, validityPeriod),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		0,
		&asn_DEF_validityPeriod_2,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"validityPeriod"
		},
	{ ATF_POINTER, 1, offsetof(struct ValidityPeriodDetailType, excludedTimeRange),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		0,
		&asn_DEF_excludedTimeRange_4,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"excludedTimeRange"
		},
};
static const int asn_MAP_ValidityPeriodDetailType_oms_1[] = { 0, 1 };
static const ber_tlv_tag_t asn_DEF_ValidityPeriodDetailType_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ValidityPeriodDetailType_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* validityPeriod */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* excludedTimeRange */
};
static asn_SEQUENCE_specifics_t asn_SPC_ValidityPeriodDetailType_specs_1 = {
	sizeof(struct ValidityPeriodDetailType),
	offsetof(struct ValidityPeriodDetailType, _asn_ctx),
	asn_MAP_ValidityPeriodDetailType_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_ValidityPeriodDetailType_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_ValidityPeriodDetailType = {
	"ValidityPeriodDetailType",
	"ValidityPeriodDetailType",
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
	asn_DEF_ValidityPeriodDetailType_tags_1,
	sizeof(asn_DEF_ValidityPeriodDetailType_tags_1)
		/sizeof(asn_DEF_ValidityPeriodDetailType_tags_1[0]), /* 1 */
	asn_DEF_ValidityPeriodDetailType_tags_1,	/* Same as above */
	sizeof(asn_DEF_ValidityPeriodDetailType_tags_1)
		/sizeof(asn_DEF_ValidityPeriodDetailType_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_ValidityPeriodDetailType_1,
	2,	/* Elements count */
	&asn_SPC_ValidityPeriodDetailType_specs_1	/* Additional specs */
};

