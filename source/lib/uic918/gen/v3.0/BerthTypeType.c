/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ASN-Module-RailTicketData"
 * 	found in "../../../../../etc/UIC-barcode/misc/uicRailTicketData_v3.0.3.asn"
 * 	`asn1c -fcompound-names -fwide-types -gen-PER`
 */

#include "BerthTypeType.h"

int
BerthTypeType_constraint(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	/* Replace with underlying type checker */
	td->check_constraints = asn_DEF_ENUMERATED.check_constraints;
	return td->check_constraints(td, sptr, ctfailcb, app_key);
}

/*
 * This type is implemented using ENUMERATED,
 * so here we adjust the DEF accordingly.
 */
static void
BerthTypeType_1_inherit_TYPE_descriptor(asn_TYPE_descriptor_t *td) {
	td->free_struct    = asn_DEF_ENUMERATED.free_struct;
	td->print_struct   = asn_DEF_ENUMERATED.print_struct;
	td->check_constraints = asn_DEF_ENUMERATED.check_constraints;
	td->ber_decoder    = asn_DEF_ENUMERATED.ber_decoder;
	td->der_encoder    = asn_DEF_ENUMERATED.der_encoder;
	td->xer_decoder    = asn_DEF_ENUMERATED.xer_decoder;
	td->xer_encoder    = asn_DEF_ENUMERATED.xer_encoder;
	td->uper_decoder   = asn_DEF_ENUMERATED.uper_decoder;
	td->uper_encoder   = asn_DEF_ENUMERATED.uper_encoder;
	if(!td->per_constraints)
		td->per_constraints = asn_DEF_ENUMERATED.per_constraints;
	td->elements       = asn_DEF_ENUMERATED.elements;
	td->elements_count = asn_DEF_ENUMERATED.elements_count;
     /* td->specifics      = asn_DEF_ENUMERATED.specifics;	// Defined explicitly */
}

void
BerthTypeType_free(asn_TYPE_descriptor_t *td,
		void *struct_ptr, int contents_only) {
	BerthTypeType_1_inherit_TYPE_descriptor(td);
	td->free_struct(td, struct_ptr, contents_only);
}

int
BerthTypeType_print(asn_TYPE_descriptor_t *td, const void *struct_ptr,
		int ilevel, asn_app_consume_bytes_f *cb, void *app_key) {
	BerthTypeType_1_inherit_TYPE_descriptor(td);
	return td->print_struct(td, struct_ptr, ilevel, cb, app_key);
}

asn_dec_rval_t
BerthTypeType_decode_ber(asn_codec_ctx_t *opt_codec_ctx, asn_TYPE_descriptor_t *td,
		void **structure, const void *bufptr, size_t size, int tag_mode) {
	BerthTypeType_1_inherit_TYPE_descriptor(td);
	return td->ber_decoder(opt_codec_ctx, td, structure, bufptr, size, tag_mode);
}

asn_enc_rval_t
BerthTypeType_encode_der(asn_TYPE_descriptor_t *td,
		void *structure, int tag_mode, ber_tlv_tag_t tag,
		asn_app_consume_bytes_f *cb, void *app_key) {
	BerthTypeType_1_inherit_TYPE_descriptor(td);
	return td->der_encoder(td, structure, tag_mode, tag, cb, app_key);
}

asn_dec_rval_t
BerthTypeType_decode_xer(asn_codec_ctx_t *opt_codec_ctx, asn_TYPE_descriptor_t *td,
		void **structure, const char *opt_mname, const void *bufptr, size_t size) {
	BerthTypeType_1_inherit_TYPE_descriptor(td);
	return td->xer_decoder(opt_codec_ctx, td, structure, opt_mname, bufptr, size);
}

asn_enc_rval_t
BerthTypeType_encode_xer(asn_TYPE_descriptor_t *td, void *structure,
		int ilevel, enum xer_encoder_flags_e flags,
		asn_app_consume_bytes_f *cb, void *app_key) {
	BerthTypeType_1_inherit_TYPE_descriptor(td);
	return td->xer_encoder(td, structure, ilevel, flags, cb, app_key);
}

asn_dec_rval_t
BerthTypeType_decode_uper(asn_codec_ctx_t *opt_codec_ctx, asn_TYPE_descriptor_t *td,
		asn_per_constraints_t *constraints, void **structure, asn_per_data_t *per_data) {
	BerthTypeType_1_inherit_TYPE_descriptor(td);
	return td->uper_decoder(opt_codec_ctx, td, constraints, structure, per_data);
}

asn_enc_rval_t
BerthTypeType_encode_uper(asn_TYPE_descriptor_t *td,
		asn_per_constraints_t *constraints,
		void *structure, asn_per_outp_t *per_out) {
	BerthTypeType_1_inherit_TYPE_descriptor(td);
	return td->uper_encoder(td, constraints, structure, per_out);
}

static asn_per_constraints_t asn_PER_type_BerthTypeType_constr_1 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  0,  5 }	/* (0..5) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_BerthTypeType_value2enum_1[] = {
	{ 0,	6,	"single" },
	{ 1,	7,	"special" },
	{ 2,	6,	"double" },
	{ 3,	2,	"t2" },
	{ 4,	2,	"t3" },
	{ 5,	2,	"t4" }
};
static const unsigned int asn_MAP_BerthTypeType_enum2value_1[] = {
	2,	/* double(2) */
	0,	/* single(0) */
	1,	/* special(1) */
	3,	/* t2(3) */
	4,	/* t3(4) */
	5	/* t4(5) */
};
static const asn_INTEGER_specifics_t asn_SPC_BerthTypeType_specs_1 = {
	asn_MAP_BerthTypeType_value2enum_1,	/* "tag" => N; sorted by tag */
	asn_MAP_BerthTypeType_enum2value_1,	/* N => "tag"; sorted by N */
	6,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_BerthTypeType_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_descriptor_t asn_DEF_BerthTypeType = {
	"BerthTypeType",
	"BerthTypeType",
	BerthTypeType_free,
	BerthTypeType_print,
	BerthTypeType_constraint,
	BerthTypeType_decode_ber,
	BerthTypeType_encode_der,
	BerthTypeType_decode_xer,
	BerthTypeType_encode_xer,
	BerthTypeType_decode_uper,
	BerthTypeType_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_BerthTypeType_tags_1,
	sizeof(asn_DEF_BerthTypeType_tags_1)
		/sizeof(asn_DEF_BerthTypeType_tags_1[0]), /* 1 */
	asn_DEF_BerthTypeType_tags_1,	/* Same as above */
	sizeof(asn_DEF_BerthTypeType_tags_1)
		/sizeof(asn_DEF_BerthTypeType_tags_1[0]), /* 1 */
	&asn_PER_type_BerthTypeType_constr_1,
	0, 0,	/* Defined elsewhere */
	&asn_SPC_BerthTypeType_specs_1	/* Additional specs */
};

