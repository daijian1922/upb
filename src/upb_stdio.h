/*
 * upb - a minimalist implementation of protocol buffers.
 *
 * Copyright (c) 2010 Google Inc.  See LICENSE for details.
 * Author: Josh Haberman <jhaberman@gmail.com>
 *
 * This file provides upb_bytesrc and upb_bytesink implementations for
 * ANSI C stdio, which is less efficient than posixfd, but more portable.
 *
 * Specifically, stdio functions acquire locks on every operation (unless you
 * use the f{read,write,...}_unlocked variants, which are not standard) and
 * performs redundant buffering (unless you disable it with setvbuf(), but we
 * can only do this on newly-opened filehandles).
 */

#include <stdio.h>
#include "upb_bytestream.h"

#ifndef UPB_STDIO_H_
#define UPB_STDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint64_t ofs;
  uint32_t refcount;
  char data[];
} upb_stdio_buf;

// We use a single object for both bytesrc and bytesink for simplicity.
// The object is still not thread-safe, and may only be used by one reader
// and one writer at a time.
typedef struct {
  upb_bytesrc src;
  upb_bytesink sink;
  FILE *file;
  bool should_close;
  upb_stdio_buf **bufs;
  uint32_t nbuf, szbuf;
} upb_stdio;

void upb_stdio_init(upb_stdio *stdio);
// Caller should call upb_stdio_flush prior to calling this to ensure that
// all data is flushed, otherwise data can be silently dropped if an error
// occurs flushing the remaining buffers.
void upb_stdio_uninit(upb_stdio *stdio);

// Resets the object to read/write to the given "file."  The caller is
// responsible for closing the file, which must outlive this object.
void upb_stdio_reset(upb_stdio *stdio, FILE *file);

// As an alternative to upb_stdio_reset(), initializes the object by opening a
// file, and will handle closing it.  This may result in more efficient I/O
// than the previous since we can call setvbuf() to disable buffering.
void upb_stdio_open(upb_stdio *stdio, const char *filename, const char *mode,
                    upb_status *s);

// Must be called to cleanup after the object, including closing the file if
// it was opened with upb_stdio_open() (which can fail, hence the status).
//

upb_bytesrc *upb_stdio_bytesrc(upb_stdio *stdio);
upb_bytesink *upb_stdio_bytesink(upb_stdio *stdio);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif
