/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module-RailTicketData"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v3.0.3.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_TicketLinkType_H_
#define	_TicketLinkType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <IA5String.h>
#include <INTEGER.h>
#include <UTF8String.h>
#include <NativeInteger.h>
#include "TicketType.h"
#include "LinkMode.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* TicketLinkType */
typedef struct TicketLinkType {
	IA5String_t	*referenceIA5	/* OPTIONAL */;
	INTEGER_t	*referenceNum	/* OPTIONAL */;
	UTF8String_t	*issuerName	/* OPTIONAL */;
	IA5String_t	*issuerPNR	/* OPTIONAL */;
	long	*productOwnerNum	/* OPTIONAL */;
	IA5String_t	*productOwnerIA5	/* OPTIONAL */;
	TicketType_t	*ticketType	/* DEFAULT 0 */;
	LinkMode_t	*linkMode	/* DEFAULT 0 */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} TicketLinkType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TicketLinkType;

#ifdef __cplusplus
}
#endif

#endif	/* _TicketLinkType_H_ */
#include <asn_internal.h>
