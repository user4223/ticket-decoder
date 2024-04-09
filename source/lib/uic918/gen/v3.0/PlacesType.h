/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module-RailTicketData"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v3.0.3.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_PlacesType_H_
#define	_PlacesType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <IA5String.h>
#include <UTF8String.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* PlacesType */
typedef struct PlacesType {
	IA5String_t	*coach	/* OPTIONAL */;
	IA5String_t	*placeString	/* OPTIONAL */;
	UTF8String_t	*placeDescription	/* OPTIONAL */;
	struct PlacesType__placeIA5 {
		A_SEQUENCE_OF(IA5String_t) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *placeIA5;
	struct PlacesType__placeNum {
		A_SEQUENCE_OF(long) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *placeNum;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PlacesType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PlacesType;

#ifdef __cplusplus
}
#endif

#endif	/* _PlacesType_H_ */
#include <asn_internal.h>
