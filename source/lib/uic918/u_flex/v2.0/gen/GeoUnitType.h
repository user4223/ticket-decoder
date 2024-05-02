/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../../etc/UIC-barcode/misc/uicRailTicketData_v2.0.2.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_GeoUnitType_H_
#define	_GeoUnitType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <ENUMERATED.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum GeoUnitType {
	GeoUnitType_microDegree	= 0,
	GeoUnitType_tenthmilliDegree	= 1,
	GeoUnitType_milliDegree	= 2,
	GeoUnitType_centiDegree	= 3,
	GeoUnitType_deciDegree	= 4
} e_GeoUnitType;

/* GeoUnitType */
typedef ENUMERATED_t	 GeoUnitType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_GeoUnitType;
asn_struct_free_f GeoUnitType_free;
asn_struct_print_f GeoUnitType_print;
asn_constr_check_f GeoUnitType_constraint;
ber_type_decoder_f GeoUnitType_decode_ber;
der_type_encoder_f GeoUnitType_encode_der;
xer_type_decoder_f GeoUnitType_decode_xer;
xer_type_encoder_f GeoUnitType_encode_xer;
per_type_decoder_f GeoUnitType_decode_uper;
per_type_encoder_f GeoUnitType_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _GeoUnitType_H_ */
#include <asn_internal.h>