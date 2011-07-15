/*
 * upb - a minimalist implementation of protocol buffers.
 *
 * Copyright (c) 2011 Google Inc.  See LICENSE for details.
 * Author: Josh Haberman <jhaberman@gmail.com>
 *
 * upb's core components like upb_decoder and upb_msg are carefully designed to
 * avoid depending on each other for maximum orthogonality.  In other words,
 * you can use a upb_decoder to decode into *any* kind of structure; upb_msg is
 * just one such structure.  A upb_msg can be serialized/deserialized into any
 * format, protobuf binary format is just one such format.
 *
 * However, for convenience we provide functions here for doing common
 * operations like deserializing protobuf binary format into a upb_msg.  The
 * compromise is that this file drags in almost all of upb as a dependency,
 * which could be undesirable if you're trying to use a trimmed-down build of
 * upb.
 *
 * While these routines are convenient, they do not reuse any encoding/decoding
 * state.  For example, if a decoder is JIT-based, it will be re-JITted every
 * time these functions are called.  For this reason, if you are parsing lots
 * of data and efficiency is an issue, these may not be the best functions to
 * use (though they are useful for prototyping, before optimizing).
 */

#ifndef UPB_GLUE_H
#define UPB_GLUE_H

#include <stdbool.h>
#include "upb.h"

#ifdef __cplusplus
extern "C" {
#endif

// Forward-declares so we don't have to include everything in this .h file.
// Clients should use the regular, typedef'd names (eg. upb_string).
struct _upb_msg;
struct _upb_msgdef;
struct _upb_symtab;

// Decodes the given string, which must be in protobuf binary format, to the
// given upb_msg with msgdef "md", storing the status of the operation in "s".
void upb_strtomsg(const char *str, size_t len, void *msg,
                  struct _upb_msgdef *md, upb_status *s);

//void upb_msgtotext(struct _upb_string *str, void *msg,
//                   struct _upb_msgdef *md, bool single_line);

void upb_read_descriptor(struct _upb_symtab *symtab, const char *str, size_t len,
                         upb_status *status);

void upb_read_descriptorfile(struct _upb_symtab *symtab, const char *fname,
                             upb_status *status);

char *upb_readfile(const char *filename, size_t *len);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif
