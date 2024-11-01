/**
 * Copyright (c) 2008-2015 Alper Akcan <alper.akcan@gmail.com>
 * Copyright (c) 2009 Renzo Davoli <renzo@cs.unibo.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the main directory of the fuse-ext2
 * distribution in the file COPYING); if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "fuse-ext2.h"

int op_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
  __u64 pos;
  errcode_t rc;
  unsigned int bytes;
  ext2_file_t efile = EXT2FS_FILE(fi->fh);
  ext2_filsys e2fs = current_ext2fs();

  debugf("enter");
  debugf("path = %s", path);

  efile = do_open(e2fs, path, O_RDONLY);
  rc = ext2fs_file_llseek(efile, offset, SEEK_SET, &pos);
  if (rc) {
    do_release(efile);
    return -EINVAL;
  }

  rc = ext2fs_file_read(efile, buf, size, &bytes);
  if (rc) {
    do_release(efile);
    return -EIO;
  }
  do_release(efile);

  debugf("leave");
  return bytes;
}
