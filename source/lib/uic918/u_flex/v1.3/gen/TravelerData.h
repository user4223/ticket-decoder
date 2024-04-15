/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../../etc/UIC-barcode/misc/uicRailTicketData_v1.3.4.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_TravelerData_H_
#define	_TravelerData_H_


#include <asn_application.h>

/* Including external dependencies */
#include <IA5String.h>
#include <UTF8String.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct TravelerType;

/* TravelerData */
typedef struct TravelerData {
	struct TravelerData__traveler {
		A_SEQUENCE_OF(struct TravelerType) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *traveler;
	IA5String_t	*preferredLanguage	/* OPTIONAL */;
	UTF8String_t	*groupName	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} TravelerData_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TravelerData;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "TravelerType.h"

#endif	/* _TravelerData_H_ */
#include <asn_internal.h>
