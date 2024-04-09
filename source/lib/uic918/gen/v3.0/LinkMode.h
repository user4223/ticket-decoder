/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module-RailTicketData"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v3.0.3.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_LinkMode_H_
#define	_LinkMode_H_


#include <asn_application.h>

/* Including external dependencies */
#include <ENUMERATED.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum LinkMode {
	LinkMode_issuedTogether	= 0,
	LinkMode_onlyValidInCombination	= 1
	/*
	 * Enumeration is extensible
	 */
} e_LinkMode;

/* LinkMode */
typedef ENUMERATED_t	 LinkMode_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_LinkMode;
asn_struct_free_f LinkMode_free;
asn_struct_print_f LinkMode_print;
asn_constr_check_f LinkMode_constraint;
ber_type_decoder_f LinkMode_decode_ber;
der_type_encoder_f LinkMode_encode_der;
xer_type_decoder_f LinkMode_decode_xer;
xer_type_encoder_f LinkMode_encode_xer;
per_type_decoder_f LinkMode_decode_uper;
per_type_encoder_f LinkMode_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _LinkMode_H_ */
#include <asn_internal.h>
