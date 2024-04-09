/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v1.3.4.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_TariffType_H_
#define	_TariffType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include "PassengerType.h"
#include <BOOLEAN.h>
#include <INTEGER.h>
#include <IA5String.h>
#include <UTF8String.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct RouteSectionType;
struct SeriesDetailType;
struct CardReferenceType;

/* TariffType */
typedef struct TariffType {
	long	*numberOfPassengers	/* DEFAULT 1 */;
	PassengerType_t	*passengerType	/* OPTIONAL */;
	long	*ageBelow	/* OPTIONAL */;
	long	*ageAbove	/* OPTIONAL */;
	struct TariffType__travelerid {
		A_SEQUENCE_OF(long) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *travelerid;
	BOOLEAN_t	 restrictedToCountryOfResidence;
	struct RouteSectionType	*restrictedToRouteSection	/* OPTIONAL */;
	struct SeriesDetailType	*seriesDataDetails	/* OPTIONAL */;
	INTEGER_t	*tariffIdNum	/* OPTIONAL */;
	IA5String_t	*tariffIdIA5	/* OPTIONAL */;
	UTF8String_t	*tariffDesc	/* OPTIONAL */;
	struct TariffType__reductionCard {
		A_SEQUENCE_OF(struct CardReferenceType) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *reductionCard;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} TariffType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TariffType;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "RouteSectionType.h"
#include "SeriesDetailType.h"
#include "CardReferenceType.h"

#endif	/* _TariffType_H_ */
#include <asn_internal.h>
