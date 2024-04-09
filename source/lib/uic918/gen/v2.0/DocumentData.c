/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v2.0.2.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#include "DocumentData.h"

static asn_per_constraints_t asn_PER_type_ticket_constr_3 GCC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  4,  4,  0,  11 }	/* (0..11,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_ticket_3[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct DocumentData__ticket, choice.reservation),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ReservationData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"reservation"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DocumentData__ticket, choice.carCarriageReservation),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CarCarriageReservationData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"carCarriageReservation"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DocumentData__ticket, choice.openTicket),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_OpenTicketData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"openTicket"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DocumentData__ticket, choice.pass),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PassData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"pass"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DocumentData__ticket, choice.voucher),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VoucherData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"voucher"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DocumentData__ticket, choice.customerCard),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CustomerCardData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"customerCard"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DocumentData__ticket, choice.counterMark),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CountermarkData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"counterMark"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DocumentData__ticket, choice.parkingGround),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ParkingGroundData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"parkingGround"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DocumentData__ticket, choice.fipTicket),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_FIPTicketData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"fipTicket"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DocumentData__ticket, choice.stationPassage),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_StationPassageData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"stationPassage"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DocumentData__ticket, choice.extension),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ExtensionData,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"extension"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DocumentData__ticket, choice.delayConfirmation),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DelayConfirmation,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"delayConfirmation"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_ticket_tag2el_3[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* reservation */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* carCarriageReservation */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* openTicket */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* pass */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* voucher */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* customerCard */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* counterMark */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* parkingGround */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 }, /* fipTicket */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 9, 0, 0 }, /* stationPassage */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 10, 0, 0 }, /* extension */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 11, 0, 0 } /* delayConfirmation */
};
static asn_CHOICE_specifics_t asn_SPC_ticket_specs_3 = {
	sizeof(struct DocumentData__ticket),
	offsetof(struct DocumentData__ticket, _asn_ctx),
	offsetof(struct DocumentData__ticket, present),
	sizeof(((struct DocumentData__ticket *)0)->present),
	asn_MAP_ticket_tag2el_3,
	12,	/* Count of tags in the map */
	0,
	12	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_ticket_3 = {
	"ticket",
	"ticket",
	CHOICE_free,
	CHOICE_print,
	CHOICE_constraint,
	CHOICE_decode_ber,
	CHOICE_encode_der,
	CHOICE_decode_xer,
	CHOICE_encode_xer,
	CHOICE_decode_uper,
	CHOICE_encode_uper,
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	&asn_PER_type_ticket_constr_3,
	asn_MBR_ticket_3,
	12,	/* Elements count */
	&asn_SPC_ticket_specs_3	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_DocumentData_1[] = {
	{ ATF_POINTER, 1, offsetof(struct DocumentData, token),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TokenType,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"token"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct DocumentData, ticket),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_ticket_3,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"ticket"
		},
};
static const int asn_MAP_DocumentData_oms_1[] = { 0 };
static const ber_tlv_tag_t asn_DEF_DocumentData_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_DocumentData_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* token */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* ticket */
};
static asn_SEQUENCE_specifics_t asn_SPC_DocumentData_specs_1 = {
	sizeof(struct DocumentData),
	offsetof(struct DocumentData, _asn_ctx),
	asn_MAP_DocumentData_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_DocumentData_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	1,	/* Start extensions */
	3	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_DocumentData = {
	"DocumentData",
	"DocumentData",
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
	asn_DEF_DocumentData_tags_1,
	sizeof(asn_DEF_DocumentData_tags_1)
		/sizeof(asn_DEF_DocumentData_tags_1[0]), /* 1 */
	asn_DEF_DocumentData_tags_1,	/* Same as above */
	sizeof(asn_DEF_DocumentData_tags_1)
		/sizeof(asn_DEF_DocumentData_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_DocumentData_1,
	2,	/* Elements count */
	&asn_SPC_DocumentData_specs_1	/* Additional specs */
};

