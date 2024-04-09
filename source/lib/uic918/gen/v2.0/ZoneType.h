/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v2.0.2.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_ZoneType_H_
#define	_ZoneType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <IA5String.h>
#include "CodeTableType.h"
#include <INTEGER.h>
#include <OCTET_STRING.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ZoneType */
typedef struct ZoneType {
	long	*carrierNum	/* OPTIONAL */;
	IA5String_t	*carrierIA5	/* OPTIONAL */;
	CodeTableType_t	*stationCodeTable	/* DEFAULT 0 */;
	long	*entryStationNum	/* OPTIONAL */;
	IA5String_t	*entryStationIA5	/* OPTIONAL */;
	long	*terminatingStationNum	/* OPTIONAL */;
	IA5String_t	*terminatingStationIA5	/* OPTIONAL */;
	INTEGER_t	*city	/* OPTIONAL */;
	struct ZoneType__zoneId {
		A_SEQUENCE_OF(INTEGER_t) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *zoneId;
	OCTET_STRING_t	*binaryZoneId	/* OPTIONAL */;
	IA5String_t	*nutsCode	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ZoneType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ZoneType;

#ifdef __cplusplus
}
#endif

#endif	/* _ZoneType_H_ */
#include <asn_internal.h>
