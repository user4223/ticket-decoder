/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v1.3.4.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_HemisphereLongitudeType_H_
#define	_HemisphereLongitudeType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <ENUMERATED.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum HemisphereLongitudeType {
	HemisphereLongitudeType_north	= 0,
	HemisphereLongitudeType_south	= 1
} e_HemisphereLongitudeType;

/* HemisphereLongitudeType */
typedef ENUMERATED_t	 HemisphereLongitudeType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_HemisphereLongitudeType;
asn_struct_free_f HemisphereLongitudeType_free;
asn_struct_print_f HemisphereLongitudeType_print;
asn_constr_check_f HemisphereLongitudeType_constraint;
ber_type_decoder_f HemisphereLongitudeType_decode_ber;
der_type_encoder_f HemisphereLongitudeType_encode_der;
xer_type_decoder_f HemisphereLongitudeType_decode_xer;
xer_type_encoder_f HemisphereLongitudeType_encode_xer;
per_type_decoder_f HemisphereLongitudeType_decode_uper;
per_type_encoder_f HemisphereLongitudeType_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _HemisphereLongitudeType_H_ */
#include <asn_internal.h>
