/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v1.3.4.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_BerthDetailData_H_
#define	_BerthDetailData_H_


#include <asn_application.h>

/* Including external dependencies */
#include "BerthTypeType.h"
#include <NativeInteger.h>
#include "CompartmentGenderType.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* BerthDetailData */
typedef struct BerthDetailData {
	BerthTypeType_t	 berthType;
	long	 numberOfBerths;
	CompartmentGenderType_t	*gender	/* DEFAULT 1 */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} BerthDetailData_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_BerthDetailData;

#ifdef __cplusplus
}
#endif

#endif	/* _BerthDetailData_H_ */
#include <asn_internal.h>
