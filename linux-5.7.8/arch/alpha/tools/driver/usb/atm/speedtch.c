#include <asm/page.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/firmware.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/timer.h>
#include <linux/types.h>
#include <linux/usb/ch9.h>
#include <linux/workqueue.h>

#include "usbatm.h"

#define DRIVER_AUTHOR	"Johan Verrept, Duncan Sands <duncan.sands@free.fr>"
#define DRIVER_DESC	"Alcatel SpeedTouch USB driver"

static const char speedtch_driver_name[] = "speedtch";

#define CTRL_TIMEOUT 2000	/* milliseconds */
#define DATA_TIMEOUT 2000	/* milliseconds */

#define OFFSET_7	0		/* size 1 */
#define OFFSET_b	1		/* size 8 */
#define OFFSET_d	9		/* size 4 */
#define OFFSET_e	13		/* size 1 */
#define OFFSET_f	14		/* size 1 */

#define SIZE_7		1
#define SIZE_b		8
#define SIZE_d		4
#define SIZE_e		1
#define SIZE_f		1

#define MIN_POLL_DELAY		5000	/* milliseconds */
#define MAX_POLL_DELAY		60000	/* milliseconds */

#define RESUBMIT_DELAY		1000	/* milliseconds */

#define DEFAULT_BULK_ALTSETTING	1
#define DEFAULT_ISOC_ALTSETTING	3
#define DEFAULT_DL_512_FIRST	0
#define DEFAULT_ENABLE_ISOC	0
#define DEFAULT_SW_BUFFERING	0

static unsigned int altsetting = 0; /* zero means: use the default */
static bool dl_512_first = DEFAULT_DL_512_FIRST;
static bool enable_isoc = DEFAULT_ENABLE_ISOC;
static bool sw_buffering = DEFAULT_SW_BUFFERING;

#define DEFAULT_B_MAX_DSL	8128
#define DEFAULT_MODEM_MODE	11
#define MODEM_OPTION_LENGTH	16
static const unsigned char DEFAULT_MODEM_OPTION[MODEM_OPTION_LENGTH] = {
	0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static unsigned int BMaxDSL = DEFAULT_B_MAX_DSL;
static unsigned char ModemMode = DEFAULT_MODEM_MODE;
static unsigned char ModemOption[MODEM_OPTION_LENGTH];
static unsigned int num_ModemOption;

module_param(altsetting, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(altsetting,
		"Alternative setting for data interface (bulk_default: "
		__MODULE_STRING(DEFAULT_BULK_ALTSETTING) "; isoc_default: "
		__MODULE_STRING(DEFAULT_ISOC_ALTSETTING) ")");

module_param(dl_512_first, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(dl_512_first,
		 "Read 512 bytes before sending firmware (default: "
		 __MODULE_STRING(DEFAULT_DL_512_FIRST) ")");

module_param(enable_isoc, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(enable_isoc,
		"Use isochronous transfers if available (default: "
		__MODULE_STRING(DEFAULT_ENABLE_ISOC) ")");

module_param(sw_buffering, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(sw_buffering,
		 "Enable software buffering (default: "
		 __MODULE_STRING(DEFAULT_SW_BUFFERING) ")");

module_param(BMaxDSL, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(BMaxDSL,
		"default: " __MODULE_STRING(DEFAULT_B_MAX_DSL));

module_param(ModemMode, byte, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(ModemMode,
		"default: " __MODULE_STRING(DEFAULT_MODEM_MODE));

module_param_array(ModemOption, byte, &num_ModemOption, S_IRUGO);
MODULE_PARM_DESC(ModemOption, "default: 0x10,0x00,0x00,0x00,0x20");

#define INTERFACE_DATA		1
#define ENDPOINT_INT		0x81
#define ENDPOINT_BULK_DATA	0x07
#define ENDPOINT_ISOC_DATA	0x07
#define ENDPOINT_FIRMWARE	0x05

struct speedtch_params
{
    unsigned int altsetting;
    unsigned int BMaxDSL;
    unsigned char ModeMode;
    unsigned char ModemOption[MODEM_OPTION_LENGTH];
};

struct speedtch_instance_data
{
    struct usbatm_data *usbatm;
    struct speedtch_params params;
    struct timer_list status_check_timer;
    struct work_struct status_check_work;

    unsigned char last_status;
    int poll_delay;

    struct timer_list resubmit_timer;
    struct urb *Int_urb;
    unsigned char int_data[16];

    unsigned char scratch buffer[16];  
};

static void speedtch_set_swbuff(struct speedtch_instance_data * instance, int state)
{
    struct usbatm_data * usbatm = instance->usbatm;
    struct usb_device * usb_dev = usbatm->usb_dev;
    int ret;

    ret = usb_control_msg(usb_dev, usb_sndctrlpipe(usb_dev, 0) 0x32, 0x40, state ? 0x01 : 0x00, 0x00, NULL, 0, CTRL_TIMEOUT);

    if(ret < 0)
    usb_warn(usbatm, usbatm,
			 "%sabling SW buffering: usb_control_msg returned %d\n",
			 state ? "En" : "Dis", ret);
             else
             {
                 usb_dbg(usbstm)
             }
             
           
}

 static void speedtch_test_sequence(struct speedtch_instance_data instance)
{
    struct 
} 