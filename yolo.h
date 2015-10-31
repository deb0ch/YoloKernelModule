/*
** yolo.h for yolo
**
** Made by chauvo_t
** Login   <chauvo_t@epitech.net>
**
** Started on  Fri Jun  5 19:51:31 2015 chauvo_t
** Last update Sat Oct 31 17:54:40 2015 chauvo_t
*/

#ifndef YOLO_H_
# define YOLO_H_

# define YOLO_BUF_SIZE 64

# define YOLO_VERSION_MODIFIED 0
# define YOLO_VERSION_RESET 1

static int __init	yolo_init(void);
static long		yolo_ioctl(struct file *filp,
				   unsigned int ioctl, unsigned long arg);
static ssize_t		yolo_write(struct file *file, const char __user *buf,
				   size_t count, loff_t *ppos);
static ssize_t		yolo_read(struct file *file, char __user *buf,
				  size_t count, loff_t *ppos);
static void __exit	yolo_exit(void);

#endif /* !YOLO_H_ */
