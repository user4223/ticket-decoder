/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module-RailTicketData"
 * 	found in "../../../../../../etc/UIC-barcode/misc/uicRailTicketData_v3.0.3.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_FIPTicketData_H_
#define	_FIPTicketData_H_


#include <asn_application.h>

/* Including external dependencies */
#include <IA5String.h>
#include <INTEGER.h>
#include <NativeInteger.h>
#include <BOOLEAN.h>
#include "TravelClassType.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ExtensionData;

/* FIPTicketData */
typedef struct FIPTicketData {
	IA5String_t	*referenceIA5	/* OPTIONAL */;
	INTEGER_t	*referenceNum	/* OPTIONAL */;
	long	*productOwnerNum	/* OPTIONAL */;
	IA5String_t	*productOwnerIA5	/* OPTIONAL */;
	long	*productIdNum	/* OPTIONAL */;
	IA5String_t	*productIdIA5	/* OPTIONAL */;
	long	 validFromDay	/* DEFAULT 0 */;
	long	 validUntilDay	/* DEFAULT 0 */;
	struct FIPTicketData__activatedDay {
		A_SEQUENCE_OF(long) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *activatedDay;
	struct FIPTicketData__carrierNum {
		A_SEQUENCE_OF(long) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *carrierNum;
	struct FIPTicketData__carrierIA5 {
		A_SEQUENCE_OF(IA5String_t) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *carrierIA5;
	long	 numberOfTravelDays;
	BOOLEAN_t	 includesSupplements;
	TravelClassType_t	*classCode	/* DEFAULT 2 */;
	struct ExtensionData	*extension	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} FIPTicketData_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_FIPTicketData;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ExtensionData.h"

#endif	/* _FIPTicketData_H_ */
#include <asn_internal.h>
