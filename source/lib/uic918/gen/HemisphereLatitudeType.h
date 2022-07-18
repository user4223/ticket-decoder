/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../doc/barcode.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_HemisphereLatitudeType_H_
#define	_HemisphereLatitudeType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <ENUMERATED.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum HemisphereLatitudeType {
	HemisphereLatitudeType_east	= 0,
	HemisphereLatitudeType_west	= 1
} e_HemisphereLatitudeType;

/* HemisphereLatitudeType */
typedef ENUMERATED_t	 HemisphereLatitudeType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_HemisphereLatitudeType;
asn_struct_free_f HemisphereLatitudeType_free;
asn_struct_print_f HemisphereLatitudeType_print;
asn_constr_check_f HemisphereLatitudeType_constraint;
ber_type_decoder_f HemisphereLatitudeType_decode_ber;
der_type_encoder_f HemisphereLatitudeType_encode_der;
xer_type_decoder_f HemisphereLatitudeType_decode_xer;
xer_type_encoder_f HemisphereLatitudeType_encode_xer;
per_type_decoder_f HemisphereLatitudeType_decode_uper;
per_type_encoder_f HemisphereLatitudeType_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _HemisphereLatitudeType_H_ */
#include <asn_internal.h>
