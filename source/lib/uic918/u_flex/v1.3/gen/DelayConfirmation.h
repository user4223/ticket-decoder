/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../../etc/UIC-barcode/misc/uicRailTicketData_v1.3.4.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_DelayConfirmation_H_
#define	_DelayConfirmation_H_


#include <asn_application.h>

/* Including external dependencies */
#include <IA5String.h>
#include <INTEGER.h>
#include <NativeInteger.h>
#include "CodeTableType.h"
#include <BOOLEAN.h>
#include "ConfirmationType.h"
#include <UTF8String.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ExtensionData;
struct TicketLinkType;

/* DelayConfirmation */
typedef struct DelayConfirmation {
	IA5String_t	*referenceIA5	/* OPTIONAL */;
	INTEGER_t	*referenceNum	/* OPTIONAL */;
	INTEGER_t	*trainNum	/* OPTIONAL */;
	IA5String_t	*trainIA5	/* OPTIONAL */;
	long	*departureYear	/* OPTIONAL */;
	long	*departureDay	/* OPTIONAL */;
	long	*departureTime	/* OPTIONAL */;
	long	*departureUTCOffset	/* OPTIONAL */;
	CodeTableType_t	*stationCodeTable	/* DEFAULT 0 */;
	long	*stationNum	/* OPTIONAL */;
	IA5String_t	*stationIA5	/* OPTIONAL */;
	long	 delay;
	BOOLEAN_t	 trainCancelled;
	ConfirmationType_t	*confirmationType	/* DEFAULT 1 */;
	struct DelayConfirmation__affectedTickets {
		A_SEQUENCE_OF(struct TicketLinkType) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *affectedTickets;
	UTF8String_t	*infoText	/* OPTIONAL */;
	struct ExtensionData	*extension	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DelayConfirmation_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DelayConfirmation;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ExtensionData.h"
#include "TicketLinkType.h"

#endif	/* _DelayConfirmation_H_ */
#include <asn_internal.h>
