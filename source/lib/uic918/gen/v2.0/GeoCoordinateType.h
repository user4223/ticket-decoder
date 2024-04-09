/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v2.0.2.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_GeoCoordinateType_H_
#define	_GeoCoordinateType_H_


#include <asn_application.h>

/* Including external dependencies */
#include "GeoUnitType.h"
#include "GeoCoordinateSystemType.h"
#include "HemisphereLongitudeType.h"
#include "HemisphereLatitudeType.h"
#include <INTEGER.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* GeoCoordinateType */
typedef struct GeoCoordinateType {
	GeoUnitType_t	*geoUnit	/* DEFAULT 2 */;
	GeoCoordinateSystemType_t	*coordinateSystem	/* DEFAULT 0 */;
	HemisphereLongitudeType_t	*hemisphereLongitude	/* DEFAULT 0 */;
	HemisphereLatitudeType_t	*hemisphereLatitude	/* DEFAULT 0 */;
	INTEGER_t	 longitude;
	INTEGER_t	 latitude;
	GeoUnitType_t	*accuracy	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} GeoCoordinateType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_GeoCoordinateType;

#ifdef __cplusplus
}
#endif

#endif	/* _GeoCoordinateType_H_ */
#include <asn_internal.h>
