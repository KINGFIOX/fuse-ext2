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

int op_symlink(const char *sourcename, const char *destname) {
  int rt;
  size_t wr;
  ext2_file_t efile;
  ext2_filsys e2fs = current_ext2fs();
  int sourcelen = strlen(sourcename);

  debugf("enter");
  debugf("source: %s, dest: %s", sourcename, destname);

  /* a short symlink is stored in the inode (recycling the i_block array) */
  if (sourcelen < (EXT2_N_BLOCKS * sizeof(__u32))) {
    rt = do_create(e2fs, destname, LINUX_S_IFLNK | 0777, 0, sourcename);
    if (rt != 0) {
      debugf("do_create(%s, LINUX_S_IFLNK | 0777, FAST); failed", destname);
      return rt;
    }
  } else {
    rt = do_create(e2fs, destname, LINUX_S_IFLNK | 0777, 0, NULL);
    if (rt != 0) {
      debugf("do_create(%s, LINUX_S_IFLNK | 0777); failed", destname);
      return rt;
    }
    efile = do_open(e2fs, destname, O_WRONLY);
    if (efile == NULL) {
      debugf("do_open(%s); failed", destname);
      return -EIO;
    }
    wr = do_write(efile, sourcename, sourcelen, 0);
    if (wr != strlen(sourcename)) {
      debugf("do_write(efile, %s, %d, 0); failed", sourcename, strlen(sourcename) + 1);
      return -EIO;
    }
    rt = do_release(efile);
    if (rt != 0) {
      debugf("do_release(efile); failed");
      return rt;
    }
  }
  debugf("leave");
  return 0;
}
