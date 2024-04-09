/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module-RailTicketData"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v3.0.3.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_BerthTypeType_H_
#define	_BerthTypeType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <ENUMERATED.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum BerthTypeType {
	BerthTypeType_single	= 0,
	BerthTypeType_special	= 1,
	BerthTypeType_double	= 2,
	BerthTypeType_t2	= 3,
	BerthTypeType_t3	= 4,
	BerthTypeType_t4	= 5
} e_BerthTypeType;

/* BerthTypeType */
typedef ENUMERATED_t	 BerthTypeType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_BerthTypeType;
asn_struct_free_f BerthTypeType_free;
asn_struct_print_f BerthTypeType_print;
asn_constr_check_f BerthTypeType_constraint;
ber_type_decoder_f BerthTypeType_decode_ber;
der_type_encoder_f BerthTypeType_encode_der;
xer_type_decoder_f BerthTypeType_decode_xer;
xer_type_encoder_f BerthTypeType_encode_xer;
per_type_decoder_f BerthTypeType_decode_uper;
per_type_encoder_f BerthTypeType_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _BerthTypeType_H_ */
#include <asn_internal.h>
