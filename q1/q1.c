#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/slab.h>
#include<linux/fs.h>
#include<linux/module.h>
#include<linux/wait.h>
#include<linux/semaphore.h>

#define mem_size 50
dev_t dev
wait_queue_flag=0;
uint32_t write=0;
uint32_t read=0;
uint32_t kbuff[50];
static struct class *dev_class;
static struct cdev file_cdev;
static struct semaphore sem;
struct  wait_queue_head_t wait_que_my;
static struct task_struct wait_thread;

DECLARE_WAITQUEUE(wait_que,2)

static int      __init file_init(void);
static void     __exit file_exit(void);
static int      file_open(struct inode *inode, struct file *file);
static int      file_release(struct inode *inode, struct file *file);
static ssize_t  file_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  file_write(struct file *filp, const char *buf, size_t len, loff_t * off);

static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = file_read,
    .write      = file_write,
    .open       = file_open,
    .release    = file_release,
};

static int wait_function(void *unused)

        while(1) {
                pr_info("Waiting For Event...\n");
                wait_event_interruptible(wait_queue_my, wait_queue_flag != 0 );
                if(wait_queue_flag == 1) {
                        pr_info("Event Came From Exit Function\n");
                        return 0;
                }

                if(wait_queue_flag == 2) {
                        pr_info("Event Came From Read Function - %d\n", read++);

                }
                if(wait_queue_flag == 3)
                {
                        pr_info("Event Came From write Function - %d\n", write++);
                        wait_queue_flag = 0;
                        
		}
                wait_queue_flag = 0;
        }
                                    
static int file_open(struct inode *inode, struct file *file)
{
        pr_info("Driver Open Function Called...!!!\n");
        return 0;
}

static int file_release(struct inode *inode, struct file *file)
{
        pr_info("Driver Release Function Called...!!!\n");
        return 0;
}

static ssize_t file_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        pr_info("Driver Read Function Called...!!!\n");
	up(&sem);
	 wait_queue_flag = 2;
       	copy_to_user(buf,(uint32_t)kbuff,sizeof(kbuff));
	wake_up_process(&wait_que_my);
	printk("sucessfully transferred to user\n");
        return 0;
}

static ssize_t file_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        pr_info("Driver Write Function Called...!!!\n");
	down(&sem);
	wait_queue_flag=3;
	copy_from_user((uint32_t)kbuff,buf,sizeof(buf));
	wait_event_interruptible(&wait_que_my);
	wake_up_process(&wait_que_my);

        return len;
}

static int __init file_init(void)
{
        if((alloc_chrdev_region(&dev, 0, 2, "file_Dev")) <0)
	{
                pr_err("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

        cdev_init(&file_cdev,&fops);

        if((cdev_add(&file_cdev,dev,1)) < 0)
	{
            pr_err("Cannot add the device to the system\n");
            goto r_class;
        }

        if(IS_ERR(dev_class = class_create(THIS_MODULE,"file_class")))
	{
            pr_err("Cannot create the struct class\n");
            goto r_class;
        }

        if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"file_device")))
	{
            pr_err("Cannot create the Device 1\n");
            goto r_device;
        }
        pr_info("Device Driver Insert...Done!!!\n");
      return 0;

r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}
 wait_thread = kthread_create(wait_function, NULL, "myThread");
        if (wait_thread) {
                pr_info("Thread Created done\n");
        } else
                pr_info("Thread creation not done\n");

        pr_info("Device Driver Inserted!\n");
        return 0;
	sema_init(&sem,1);

static void __exit file_exit(void)
{
        wake_up_interruptible(&wait_que_my);
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&file_cdev);
        unregister_chrdev_region(dev, 2);
        pr_info("Device Driver Removed\n");
}

module_init(file_init);
module_exit(file_exit);

MODULE_LICENSE("GPL");

