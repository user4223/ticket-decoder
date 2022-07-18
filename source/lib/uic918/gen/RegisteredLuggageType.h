/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../doc/barcode.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_RegisteredLuggageType_H_
#define	_RegisteredLuggageType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <IA5String.h>
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RegisteredLuggageType */
typedef struct RegisteredLuggageType {
	IA5String_t	*registrationId	/* OPTIONAL */;
	long	*maxWeight	/* OPTIONAL */;
	long	*maxSize	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RegisteredLuggageType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RegisteredLuggageType;

#ifdef __cplusplus
}
#endif

#endif	/* _RegisteredLuggageType_H_ */
#include <asn_internal.h>
