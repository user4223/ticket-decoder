/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v1.3.4.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_RegionalValidityType_H_
#define	_RegionalValidityType_H_


#include <asn_application.h>

/* Including external dependencies */
#include "TrainLinkType.h"
#include "ViaStationType.h"
#include "ZoneType.h"
#include "LineType.h"
#include "PolygoneType.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RegionalValidityType_PR {
	RegionalValidityType_PR_NOTHING,	/* No components present */
	RegionalValidityType_PR_trainLink,
	RegionalValidityType_PR_viaStations,
	RegionalValidityType_PR_zones,
	RegionalValidityType_PR_lines,
	RegionalValidityType_PR_polygone,
	/* Extensions may appear below */
	
} RegionalValidityType_PR;

/* RegionalValidityType */
typedef struct RegionalValidityType {
	RegionalValidityType_PR present;
	union RegionalValidityType_u {
		TrainLinkType_t	 trainLink;
		ViaStationType_t	 viaStations;
		ZoneType_t	 zones;
		LineType_t	 lines;
		PolygoneType_t	 polygone;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RegionalValidityType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RegionalValidityType;

#ifdef __cplusplus
}
#endif

#endif	/* _RegionalValidityType_H_ */
#include <asn_internal.h>
