/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module-RailTicketData"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v3.0.3.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_VatDetailType_H_
#define	_VatDetailType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <INTEGER.h>
#include <IA5String.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* VatDetailType */
typedef struct VatDetailType {
	long	 country;
	long	 percentage;
	INTEGER_t	*amount	/* OPTIONAL */;
	IA5String_t	*vatId	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} VatDetailType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_VatDetailType;

#ifdef __cplusplus
}
#endif

#endif	/* _VatDetailType_H_ */
#include <asn_internal.h>
