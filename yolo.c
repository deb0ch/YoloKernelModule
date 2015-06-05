/*
** yolo.c for yolo
**
** Made by chauvo_t
** Login   <chauvo_t@epitech.net>
**
** Started on  Fri Jun  5 19:51:27 2015 chauvo_t
** Last update Fri Jun  5 21:16:14 2015 chauvo_t
*/

#include <linux/backing-dev.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/uaccess.h>

static char	yolo_buf[64] = "Yolo, World !\n";

static int __init	yolo_init(void);
static ssize_t		yolo_read(struct file *file, char *buf,
				  size_t count, loff_t *ppos);
static void __exit	yolo_exit(void);

static const struct file_operations yolo_fops = {
	.owner		= THIS_MODULE,
     /* .llseek		= no_llseek, */
	.read		= yolo_read,
     /* .write		= yolo_write, */
     /* .unlocked_ioctl	= yolo_ioctl, */
     /* .open		= yolo_open, */
     /* .release	= yolo_close, */
};

static struct miscdevice yolo_dev = {
	MISC_DYNAMIC_MINOR,
	"yolo",
	&yolo_fops
};

/*
 * Create the "hello" device in the /sys/class/misc directory.
 * Udev will automatically create the /dev/hello device using
 * the default rules.
 */
static int __init yolo_init(void)
{
	int ret;

	ret = misc_register(&yolo_dev);
	if (ret)
		pr_err("Unable to register \"yolo\" misc device\n");
	pr_info("Yolo module properly initialized !\n");
	return ret;
}

/*
 * ppos: cursor position
 */
static ssize_t yolo_read(struct file *file, char *buf,
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
