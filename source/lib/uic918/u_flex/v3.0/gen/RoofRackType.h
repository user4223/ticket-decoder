/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module-RailTicketData"
 * 	found in "../../../../../../etc/UIC-barcode/misc/uicRailTicketData_v3.0.3.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#ifndef	_RoofRackType_H_
#define	_RoofRackType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <ENUMERATED.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RoofRackType {
	RoofRackType_norack	= 0,
	RoofRackType_roofRailing	= 1,
	RoofRackType_luggageRack	= 2,
	RoofRackType_skiRack	= 3,
	RoofRackType_boxRack	= 4,
	RoofRackType_rackWithOneBox	= 5,
	RoofRackType_rackWithTwoBoxes	= 6,
	RoofRackType_bicycleRack	= 7,
	RoofRackType_otherRack	= 8
	/*
	 * Enumeration is extensible
	 */
} e_RoofRackType;

/* RoofRackType */
typedef ENUMERATED_t	 RoofRackType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RoofRackType;
asn_struct_free_f RoofRackType_free;
asn_struct_print_f RoofRackType_print;
asn_constr_check_f RoofRackType_constraint;
ber_type_decoder_f RoofRackType_decode_ber;
der_type_encoder_f RoofRackType_encode_der;
xer_type_decoder_f RoofRackType_decode_xer;
xer_type_encoder_f RoofRackType_encode_xer;
per_type_decoder_f RoofRackType_decode_uper;
per_type_encoder_f RoofRackType_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _RoofRackType_H_ */
#include <asn_internal.h>
