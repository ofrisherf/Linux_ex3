
/*  
 *  hello-1.c - The simplest kernel module.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>	/* for put_user */
#include <linux/random.h>

int get_random_num(int max);
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define DEVICE_NAME "cardsdev"
static int Major;		/* Major number assigned to our device driver */

static char nums[] = {
   'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'
};

static char suits[] = {
   'c', 's', 'd', 'h'
};


static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

int get_random_num(int max)
{
    unsigned int i;
    int *ip;

    ip = &i;
    get_random_bytes(&i, sizeof i);

    return (i%max);
}

static int device_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    return 0;
}

static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
    return 0;
}

static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
			   char *buffer,	/* buffer to fill with data */
			   size_t length,	/* length of the buffer     */
			   loff_t * offset)
{
    printk(KERN_INFO "start");

    // Get two random numbers
    int rand1 = get_random_num(4*13);
    int rand2 = get_random_num(4*13);
    
    // In case the numbers are the same
    while (rand1 == rand2)
    {
        rand2 = get_random_num(4*13);
    }

    int suit1 = rand1%4;
    int num1 = rand1/4;
    int suit2 = rand2%4;
    int num2 = rand2/4;

    put_user(nums[num1], buffer++);
    put_user(suits[suit1], buffer++);
    
    put_user(nums[num2], buffer++);
    put_user(suits[suit2], buffer++);

    return 4;
}

/*
 * This function is called when the module is loaded
 */
int init_module(void)
{
        Major = register_chrdev(0, DEVICE_NAME, &fops);

	if (Major < 0) {
	  printk(KERN_ALERT "Registering char device failed with %d\n", Major);
	  return Major;
	}

	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");

	return 0;
}

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
	/* 
	 * Unregister the device 
	 */
	unregister_chrdev(Major, DEVICE_NAME);
}


