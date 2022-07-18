/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../doc/barcode.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_CustomerStatusType_H_
#define	_CustomerStatusType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <IA5String.h>
#include <INTEGER.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* CustomerStatusType */
typedef struct CustomerStatusType {
	long	*statusProviderNum	/* OPTIONAL */;
	IA5String_t	*statusProviderIA5	/* OPTIONAL */;
	INTEGER_t	*customerStatus	/* OPTIONAL */;
	IA5String_t	*customerStatusDescr	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CustomerStatusType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CustomerStatusType;

#ifdef __cplusplus
}
#endif

#endif	/* _CustomerStatusType_H_ */
#include <asn_internal.h>
