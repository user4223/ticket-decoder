/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../doc/barcode.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_GenderType_H_
#define	_GenderType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <ENUMERATED.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum GenderType {
	GenderType_unspecified	= 0,
	GenderType_female	= 1,
	GenderType_male	= 2,
	GenderType_other	= 3
	/*
	 * Enumeration is extensible
	 */
} e_GenderType;

/* GenderType */
typedef ENUMERATED_t	 GenderType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_GenderType;
asn_struct_free_f GenderType_free;
asn_struct_print_f GenderType_print;
asn_constr_check_f GenderType_constraint;
ber_type_decoder_f GenderType_decode_ber;
der_type_encoder_f GenderType_encode_der;
xer_type_decoder_f GenderType_decode_xer;
xer_type_encoder_f GenderType_encode_xer;
per_type_decoder_f GenderType_decode_uper;
per_type_encoder_f GenderType_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _GenderType_H_ */
#include <asn_internal.h>
