/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module"
 * 	found in "../../../../../../etc/UIC-barcode/misc/uicRailTicketData_v1.3.4.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_ConfirmationType_H_
#define	_ConfirmationType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <ENUMERATED.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ConfirmationType {
	ConfirmationType_trainDelayConfirmation	= 0,
	ConfirmationType_travelerDelayConfirmation	= 1,
	ConfirmationType_trainLinkedTicketDelay	= 2
	/*
	 * Enumeration is extensible
	 */
} e_ConfirmationType;

/* ConfirmationType */
typedef ENUMERATED_t	 ConfirmationType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ConfirmationType;
asn_struct_free_f ConfirmationType_free;
asn_struct_print_f ConfirmationType_print;
asn_constr_check_f ConfirmationType_constraint;
ber_type_decoder_f ConfirmationType_decode_ber;
der_type_encoder_f ConfirmationType_encode_der;
xer_type_decoder_f ConfirmationType_decode_xer;
xer_type_encoder_f ConfirmationType_encode_xer;
per_type_decoder_f ConfirmationType_decode_uper;
per_type_encoder_f ConfirmationType_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _ConfirmationType_H_ */
#include <asn_internal.h>
