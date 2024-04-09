/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v2.0.2.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#include "TokenType.h"

static asn_TYPE_member_t asn_MBR_TokenType_1[] = {
	{ ATF_POINTER, 3, offsetof(struct TokenType, tokenProviderNum),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"tokenProviderNum"
		},
	{ ATF_POINTER, 2, offsetof(struct TokenType, tokenProviderIA5),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"tokenProviderIA5"
		},
	{ ATF_POINTER, 1, offsetof(struct TokenType, tokenSpecification),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IA5String,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"tokenSpecification"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TokenType, token),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_OCTET_STRING,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"token"
		},
};
static const int asn_MAP_TokenType_oms_1[] = { 0, 1, 2 };
static const ber_tlv_tag_t asn_DEF_TokenType_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_TokenType_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* tokenProviderNum */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* tokenProviderIA5 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* tokenSpecification */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* token */
};
static asn_SEQUENCE_specifics_t asn_SPC_TokenType_specs_1 = {
	sizeof(struct TokenType),
	offsetof(struct TokenType, _asn_ctx),
	asn_MAP_TokenType_tag2el_1,
	4,	/* Count of tags in the map */
	asn_MAP_TokenType_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_TokenType = {
	"TokenType",
	"TokenType",
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
	asn_DEF_TokenType_tags_1,
	sizeof(asn_DEF_TokenType_tags_1)
		/sizeof(asn_DEF_TokenType_tags_1[0]), /* 1 */
	asn_DEF_TokenType_tags_1,	/* Same as above */
	sizeof(asn_DEF_TokenType_tags_1)
		/sizeof(asn_DEF_TokenType_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_TokenType_1,
	4,	/* Elements count */
	&asn_SPC_TokenType_specs_1	/* Additional specs */
};

