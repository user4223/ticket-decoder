/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../doc/barcode.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_TravelerType_H_
#define	_TravelerType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <UTF8String.h>
#include <IA5String.h>
#include "GenderType.h"
#include <INTEGER.h>
#include <NativeInteger.h>
#include <BOOLEAN.h>
#include "PassengerType.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct CustomerStatusType;

/* TravelerType */
typedef struct TravelerType {
	UTF8String_t	*firstName	/* OPTIONAL */;
	UTF8String_t	*secondName	/* OPTIONAL */;
	UTF8String_t	*lastName	/* OPTIONAL */;
	IA5String_t	*idCard	/* OPTIONAL */;
	IA5String_t	*passportId	/* OPTIONAL */;
	IA5String_t	*title	/* OPTIONAL */;
	GenderType_t	*gender	/* OPTIONAL */;
	IA5String_t	*customerIdIA5	/* OPTIONAL */;
	INTEGER_t	*customerIdNum	/* OPTIONAL */;
	long	*yearOfBirth	/* OPTIONAL */;
	long	*dayOfBirth	/* OPTIONAL */;
	BOOLEAN_t	 ticketHolder;
	PassengerType_t	*passengerType	/* OPTIONAL */;
	BOOLEAN_t	*passengerWithReducedMobility	/* OPTIONAL */;
	long	*countryOfResidence	/* OPTIONAL */;
	long	*countryOfPassport	/* OPTIONAL */;
	long	*countryOfIdCard	/* OPTIONAL */;
	struct TravelerType__status {
		A_SEQUENCE_OF(struct CustomerStatusType) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *status;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} TravelerType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TravelerType;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "CustomerStatusType.h"

#endif	/* _TravelerType_H_ */
#include <asn_internal.h>
