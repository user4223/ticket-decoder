/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../../etc/UIC-barcode/misc/uicRailTicketData_v2.0.2.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_TravelClassType_H_
#define	_TravelClassType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <ENUMERATED.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum TravelClassType {
	TravelClassType_notApplicable	= 0,
	TravelClassType_first	= 1,
	TravelClassType_second	= 2,
	TravelClassType_tourist	= 3,
	TravelClassType_comfort	= 4,
	TravelClassType_premium	= 5,
	TravelClassType_business	= 6,
	TravelClassType_all	= 7,
	TravelClassType_premiumFirst	= 8,
	TravelClassType_standardFirst	= 9,
	TravelClassType_premiumSecond	= 10,
	TravelClassType_standardSecond	= 11
	/*
	 * Enumeration is extensible
	 */
} e_TravelClassType;

/* TravelClassType */
typedef ENUMERATED_t	 TravelClassType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TravelClassType;
asn_struct_free_f TravelClassType_free;
asn_struct_print_f TravelClassType_print;
asn_constr_check_f TravelClassType_constraint;
ber_type_decoder_f TravelClassType_decode_ber;
der_type_encoder_f TravelClassType_encode_der;
xer_type_decoder_f TravelClassType_decode_xer;
xer_type_encoder_f TravelClassType_encode_xer;
per_type_decoder_f TravelClassType_decode_uper;
per_type_encoder_f TravelClassType_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _TravelClassType_H_ */
#include <asn_internal.h>
