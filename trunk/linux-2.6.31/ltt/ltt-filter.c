/*
 * Copyright (C) 2008 Mathieu Desnoyers
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/ltt-tracer.h>
#include <linux/mutex.h>

#define LTT_FILTER_DIR	"filter"

/*
 * Protects the ltt_filter_dir allocation.
 */
static DEFINE_MUTEX(ltt_filter_mutex);

static struct dentry *ltt_filter_dir;

struct dentry *get_filter_root(void)
{
	struct dentry *ltt_root_dentry;

	mutex_lock(&ltt_filter_mutex);
	if (!ltt_filter_dir) {
		ltt_root_dentry = get_ltt_root();
		if (!ltt_root_dentry)
			goto err_no_root;

		ltt_filter_dir = debugfs_create_dir(LTT_FILTER_DIR,
						    ltt_root_dentry);
		if (!ltt_filter_dir)
			printk(KERN_ERR
				"ltt_filter_init: failed to create dir %s\n",
				LTT_FILTER_DIR);
	}
err_no_root:
	mutex_unlock(&ltt_filter_mutex);
	return ltt_filter_dir;
}
EXPORT_SYMBOL_GPL(get_filter_root);

static void __exit ltt_filter_exit(void)
{
	debugfs_remove(ltt_filter_dir);
}

module_exit(ltt_filter_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mathieu Desnoyers <mathieu.desnoyers@polymtl.ca>");
MODULE_DESCRIPTION("Linux Trace Toolkit Filter");
