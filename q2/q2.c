#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 
#include <linux/uaccess.h>      
#include<linux/timer.h>
#include<linux/jiffies.h>

#define mem_size 1024 
#define TIMEOUT 500

//magic no and seq:no creation:

#define RD_DATA _IOR('a','a',int32_t*)
#define WR_DATA _IOW('a','b',int32_t*)


int8_t val=0;
dev_t dev = 0;
static struct timer_list led_timer;
static struct cdev cdev_file;
uint8_t *kbuff;

static int      led_open(struct inode *inode, struct file *file);
static int      led_release(struct inode *inode, struct file *file);
static ssize_t  led_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  led_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long     led_ioctl(struct file *file,unsigned int cmd,unsigned long arg);


static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = file_read,
        .write          = file_write,
        .open           = file_open,
        .release        = file_release,
	.unlocked_ioctl   =file_ioctl,
};
 
void timer_callback(struct timer_list * data)
{
	mod_timer(&led_timer, jiffies + msecs_to_jiffies(TIMEOUT));



static int led_open(struct inode *inode, struct file *file)
{
        pr_info("Device File Opened...!!!\n");
        return 0;
}

static int led_release(struct inode *inode, struct file *file)
{
        pr_info("Device File Closed...!!!\n");
        return 0;
}

static ssize_t led_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
                if( copy_to_user(buf, kbuff, mem_size) )
        {
                pr_err("Data Read error\n");
        }
        pr_info("Data Read : Done!\n");
        return mem_size;
}

static ssize_t led_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
               if( copy_from_user(kbuff, buf, len) )
        {
                pr_err("Data Write '%s': error\n",kbuff);
        }
        pr_info("Data Write complete!\n");
        return len;
}

static long led_ioctl(struct file *file,unsigned int cmd,unsigned long arg)
{
        switch(cmd)
        {
                case WR_DATA:
                        copy_from_user(&val,(int32_t*)arg,sizeof(val));
			printk("val copied from user=%d",val);
			break;
		case RD_DATA:
	                printk("val 1=%d",val);
			copy_to_user((int32_t*)arg,&val,sizeof(val));
			break;
	}
return 0;
}

static int __init led_init(void)
{		
       alloc_chrdev_region(&dev, 0, 1, "cdev_file")
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        cdev_init(&cdev_file,&fops);
 
         (cdev_add(&cdev_file,dev,1))
        }
        
        pr_info("Device Driver Insert...Done!!!\n");
        return 0;

	 timer_setup(&led_timer, timer_callback, 1); 


static void __exit led_exit(void)
{
	kfree(kbuff);
        cdev_del(&cdev_file);
        unregister_chrdev_region(dev, 1);
        pr_info("Device Driver Remove...Done!!!\n");
}
 
module_init(led_init);
module_exit(led_exit);
 
MODULE_LICENSE("GPL");
