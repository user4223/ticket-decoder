/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module-RailTicketData"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v3.0.3.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#include "PolygoneType.h"

static asn_TYPE_member_t asn_MBR_edges_3[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_DeltaCoordinates,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		""
		},
};
static const ber_tlv_tag_t asn_DEF_edges_tags_3[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_edges_specs_3 = {
	sizeof(struct PolygoneType__edges),
	offsetof(struct PolygoneType__edges, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_edges_3 = {
	"edges",
	"edges",
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
	asn_DEF_edges_tags_3,
	sizeof(asn_DEF_edges_tags_3)
		/sizeof(asn_DEF_edges_tags_3[0]) - 1, /* 1 */
	asn_DEF_edges_tags_3,	/* Same as above */
	sizeof(asn_DEF_edges_tags_3)
		/sizeof(asn_DEF_edges_tags_3[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_edges_3,
	1,	/* Single element */
	&asn_SPC_edges_specs_3	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_PolygoneType_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct PolygoneType, firstEdge),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_GeoCoordinateType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"firstEdge"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PolygoneType, edges),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		0,
		&asn_DEF_edges_3,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"edges"
		},
};
static const ber_tlv_tag_t asn_DEF_PolygoneType_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_PolygoneType_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* firstEdge */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* edges */
};
static asn_SEQUENCE_specifics_t asn_SPC_PolygoneType_specs_1 = {
	sizeof(struct PolygoneType),
	offsetof(struct PolygoneType, _asn_ctx),
	asn_MAP_PolygoneType_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_PolygoneType = {
	"PolygoneType",
	"PolygoneType",
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
	asn_DEF_PolygoneType_tags_1,
	sizeof(asn_DEF_PolygoneType_tags_1)
		/sizeof(asn_DEF_PolygoneType_tags_1[0]), /* 1 */
	asn_DEF_PolygoneType_tags_1,	/* Same as above */
	sizeof(asn_DEF_PolygoneType_tags_1)
		/sizeof(asn_DEF_PolygoneType_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_PolygoneType_1,
	2,	/* Elements count */
	&asn_SPC_PolygoneType_specs_1	/* Additional specs */
};

