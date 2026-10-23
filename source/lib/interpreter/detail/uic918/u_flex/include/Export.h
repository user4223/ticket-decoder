// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#if defined(__GNUC__) || defined(__clang__)
#define TICKET_DECODER_EXPORT __attribute__((__visibility__("default")))
#else
#define TICKET_DECODER_EXPORT
#endif
