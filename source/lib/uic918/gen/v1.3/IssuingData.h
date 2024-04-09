/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v1.3.4.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_IssuingData_H_
#define	_IssuingData_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <IA5String.h>
#include <UTF8String.h>
#include <BOOLEAN.h>
#include <INTEGER.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ExtensionData;
struct GeoCoordinateType;

/* IssuingData */
typedef struct IssuingData {
	long	*securityProviderNum	/* OPTIONAL */;
	IA5String_t	*securityProviderIA5	/* OPTIONAL */;
	long	*issuerNum	/* OPTIONAL */;
	IA5String_t	*issuerIA5	/* OPTIONAL */;
	long	 issuingYear;
	long	 issuingDay;
	long	*issuingTime	/* OPTIONAL */;
	UTF8String_t	*issuerName	/* OPTIONAL */;
	BOOLEAN_t	 specimen;
	BOOLEAN_t	 securePaperTicket;
	BOOLEAN_t	 activated;
	IA5String_t	*currency	/* DEFAULT EUR */;
	long	*currencyFract	/* DEFAULT 2 */;
	IA5String_t	*issuerPNR	/* OPTIONAL */;
	struct ExtensionData	*extension	/* OPTIONAL */;
	INTEGER_t	*issuedOnTrainNum	/* OPTIONAL */;
	IA5String_t	*issuedOnTrainIA5	/* OPTIONAL */;
	INTEGER_t	*issuedOnLine	/* OPTIONAL */;
	struct GeoCoordinateType	*pointOfSale	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} IssuingData_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_IssuingData;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ExtensionData.h"
#include "GeoCoordinateType.h"

#endif	/* _IssuingData_H_ */
#include <asn_internal.h>
