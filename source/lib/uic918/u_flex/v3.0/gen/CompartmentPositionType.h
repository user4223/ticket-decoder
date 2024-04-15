/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module-RailTicketData"
 * 	found in "../../../../../../etc/UIC-barcode/misc/uicRailTicketData_v3.0.3.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_CompartmentPositionType_H_
#define	_CompartmentPositionType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <ENUMERATED.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CompartmentPositionType {
	CompartmentPositionType_unspecified	= 0,
	CompartmentPositionType_upperLevel	= 1,
	CompartmentPositionType_lowerLevel	= 2
} e_CompartmentPositionType;

/* CompartmentPositionType */
typedef ENUMERATED_t	 CompartmentPositionType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CompartmentPositionType;
asn_struct_free_f CompartmentPositionType_free;
asn_struct_print_f CompartmentPositionType_print;
asn_constr_check_f CompartmentPositionType_constraint;
ber_type_decoder_f CompartmentPositionType_decode_ber;
der_type_encoder_f CompartmentPositionType_encode_der;
xer_type_decoder_f CompartmentPositionType_decode_xer;
xer_type_encoder_f CompartmentPositionType_encode_xer;
per_type_decoder_f CompartmentPositionType_decode_uper;
per_type_encoder_f CompartmentPositionType_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _CompartmentPositionType_H_ */
#include <asn_internal.h>
