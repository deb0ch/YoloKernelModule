/*
** yolo.c for yolo
**
** Made by chauvo_t
** Login   <chauvo_t@epitech.net>
**
** Started on  Fri Jun  5 19:51:27 2015 chauvo_t
** Last update Sat Oct 31 17:54:41 2015 chauvo_t
*/

#include <linux/backing-dev.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/uaccess.h>
#include <linux/utsname.h>

#include "yolo.h"

static char	yolo_buf[YOLO_BUF_SIZE];
static int	yolo_modified = 0;

static const struct file_operations yolo_fops = {
	.owner		= THIS_MODULE,
	.read		= yolo_read,
	.write		= yolo_write,
	.unlocked_ioctl	= yolo_ioctl,
	.compat_ioctl	= yolo_ioctl
};

static struct miscdevice yolo_dev = {
	MISC_DYNAMIC_MINOR,
	"yolo",
	&yolo_fops
};

static void yolo_init_contents(void)
{
	snprintf(yolo_buf, YOLO_BUF_SIZE,
		 "%s\n",
		 utsname()->version);
}

/*
 * Create the "yolo" device in the /sys/class/misc directory.
 * Udev will automatically create the /dev/hello device using
 * the default rules.
 */
static int __init yolo_init(void)
{
	int ret;

	if ((ret = misc_register(&yolo_dev))) {
		pr_err("Unable to register \"yolo\" misc device\n");
		return ret;
	}
	yolo_init_contents();
	pr_info("Yolo module properly initialized !\n");
	return ret;
}

static int yolo_ioctl_version_modified(void)
{
	return yolo_modified;
}

static void yolo_ioctl_version_reset(void)
{
	yolo_init_contents();
	yolo_modified = 0;
}

static long yolo_ioctl(struct file *filp,
		       unsigned int ioctl, unsigned long arg)
{
	long r = -EINVAL;

	switch (ioctl) {
	case YOLO_VERSION_MODIFIED:
		return yolo_ioctl_version_modified();
		break;
	case YOLO_VERSION_RESET:
		yolo_ioctl_version_reset();
		r = 0;
		break;
	default:
		goto out;
	}
out:
	return r;
}

/*
 * ppos: cursor position
 */
static ssize_t yolo_write(struct file *file, const char __user *buf,
			  size_t count, loff_t *ppos)
{
	int	max_len;

	max_len = YOLO_BUF_SIZE - 1;
	if (*ppos >= max_len)
		return 0;
	if (count > max_len)
		count = max_len;
	if (count + *ppos > max_len)
		count = max_len - *ppos;
	yolo_modified = 1;
	if (copy_from_user(yolo_buf, buf, count))
		return -EINVAL;
	yolo_buf[count] = '\0';
	*ppos += count;
	return count;
}

/*
 * ppos: cursor position
 */
static ssize_t yolo_read(struct file *file, char __user *buf,
			 size_t count, loff_t *ppos)
{
	int	len;

	len = strlen(yolo_buf);
	if (*ppos >= len)
		return 0;
	if (count > len)
		count = len;
	if (count + *ppos > len)
		count = len - *ppos;
	if (copy_to_user(buf + *ppos, yolo_buf, count))
		return -EINVAL;
	*ppos += count;
	return count;
}

static void __exit yolo_exit(void)
{
	pr_info("Yolo says goodbye.\n");
	misc_deregister(&yolo_dev);
}

module_init(yolo_init);
module_exit(yolo_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Maps pseudo-device /dev/version which gives the version of the running kernel.");
MODULE_AUTHOR("Thomas de Beauchene");
