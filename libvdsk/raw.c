/*-
 * Copyright (c) 2014 Marcel Moolenaar
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: user/marcel/libvdsk/libvdsk/raw.c 286996 2015-08-21 15:20:01Z marcel $");

#include <sys/ioctl.h>
#include <sys/disk.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vdsk.h>

#include "vdsk_int.h"

static int vdsk_debug = 1;

static int
raw_probe(struct vdsk *vdsk __unused)
{

	return (0);
}

static int
raw_open(struct vdsk *vdsk __unused)
{

	return (0);
}

static int
raw_close(struct vdsk *vdsk __unused)
{

	return (0);
}

static ssize_t
raw_readv(struct vdsk *vdsk, const struct iovec *iov, int iovcnt, off_t offset)
{
	ssize_t res;

	DPRINTF(("===> raw_readv\n"));

	res = preadv(vdsk->fd, iov, iovcnt, offset);
	return (res);
}

static ssize_t
raw_read(struct vdsk *vdsk, void *buffer, size_t nbytes, off_t offset)
{
	ssize_t res;

	DPRINTF(("===> raw_read\n"));

	res = pread(vdsk->fd, buffer, nbytes, offset);
	return (res);
}

static ssize_t
raw_writev(struct vdsk *vdsk, const struct iovec *iov, int iovcnt, off_t offset)
{
	ssize_t res;

	DPRINTF(("====> raw_writev\n"));

	res = pwritev(vdsk->fd, iov, iovcnt, offset);
	return (res);
}

static ssize_t
raw_write(struct vdsk *vdsk, void *buffer, size_t nbytes, off_t offset)
{
	ssize_t res;

	DPRINTF(("===> raw_write\n"));

	res = pwrite(vdsk->fd, buffer, nbytes, offset);
	return (res);
}

static int
raw_trim(struct vdsk *vdsk, unsigned long diocg, off_t arg[2])
{
	int res = 0;

	DPRINTF(("==> raw_flush\n"));
	DPRINTF(("==> diocg: %lu\n", diocg));

	if (ioctl(vdsk->fd, diocg, arg))
		res = errno;

	return (res);
}

static int
raw_flush(struct vdsk *vdsk, unsigned long diocg)
{
	int res = 0;

	DPRINTF(("==> raw_flush\n"));
	DPRINTF(("==> diocg: %lu\n", diocg));

	if (diocg != 0) {
		if (ioctl(vdsk->fd, diocg))
			res = errno;
	} else if (fsync(vdsk->fd) == -1) {
		res = errno;

	}

	DPRINTF(("==> return res: %d\n", res));

	return (res);
}

static struct vdsk_format raw_format = {
	.name = "raw",
	.description = "Raw Disk File or Device",
	.flags = VDSKFMT_CAN_WRITE | VDSKFMT_DEVICE_OK | VDSKFMT_NO_METADATA,
	.probe = raw_probe,
	.open = raw_open,
	.close = raw_close,
	.readv = raw_readv,
	.read = raw_read,
	.writev = raw_writev,
	.write = raw_write,
	.trim = raw_trim,
	.flush = raw_flush,
};
FORMAT_DEFINE(raw_format);

