/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module-RailTicketData"
 * 	found in "../../../../../../etc/UIC-barcode/misc/uicRailTicketData_v3.0.3.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_LuggageRestrictionType_H_
#define	_LuggageRestrictionType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct RegisteredLuggageType;

/* LuggageRestrictionType */
typedef struct LuggageRestrictionType {
	long	*maxHandLuggagePieces	/* DEFAULT 3 */;
	long	*maxNonHandLuggagePieces	/* DEFAULT 1 */;
	struct LuggageRestrictionType__registeredLuggage {
		A_SEQUENCE_OF(struct RegisteredLuggageType) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *registeredLuggage;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} LuggageRestrictionType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_LuggageRestrictionType;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "RegisteredLuggageType.h"

#endif	/* _LuggageRestrictionType_H_ */
#include <asn_internal.h>
