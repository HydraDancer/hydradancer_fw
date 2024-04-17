#ifndef USB2_DEVICE_DESCRIPTOR_H
#define USB2_DEVICE_DESCRIPTOR_H

#include "wch-ch56x-lib/USBDevice/usb_descriptors.h"

const uint8_t* usb2_device_configs[1];

struct usb2_descriptors
{
	USB_DEV_DESCR usb_device_descr;
	struct __PACKED
	{
		USB_CFG_DESCR usb_cfg_descr;
		USB_ITF_DESCR usb_itf_descr;
		USB_ENDP_DESCR usb_endp_descr_1;
		USB_ENDP_DESCR usb_endp_descr_1_tx;
		USB_ENDP_DESCR usb_endp_descr_2;
		USB_ENDP_DESCR usb_endp_descr_2_tx;
		USB_ENDP_DESCR usb_endp_descr_3;
		USB_ENDP_DESCR usb_endp_descr_3_tx;
		USB_ENDP_DESCR usb_endp_descr_4;
		USB_ENDP_DESCR usb_endp_descr_4_tx;
		USB_ENDP_DESCR usb_endp_descr_5;
		USB_ENDP_DESCR usb_endp_descr_5_tx;
		USB_ENDP_DESCR usb_endp_descr_6;
		USB_ENDP_DESCR usb_endp_descr_6_tx;
		USB_ENDP_DESCR usb_endp_descr_7;
		USB_ENDP_DESCR usb_endp_descr_7_tx;
	} other_descr;
} usb2_descriptors;

void init_usb2_descriptors(void);

void init_usb2_descriptors(void)
{
	usb2_descriptors.usb_device_descr = (USB_DEV_DESCR){
		.bLength = 0x12,
		.bDescriptorType = 0x01, // device descriptor type
		.bcdUSB = 0x0200, //usb2.0
		.bDeviceClass = 0x00,
		.bDeviceSubClass = 0x00,
		.bDeviceProtocol = 0x00,
		.bMaxPacketSize0 = 64,
		.bcdDevice = 0x0001,
		.idVendor = 0x610b, // https://github.com/obdev/v-usb/blob/master/usbdrv/usb-ids-for-free.txt
		.idProduct = 0x4653,
		.iProduct = 0x01,
		.iManufacturer = 0x00,
		.iSerialNumber = 0x00,
		.bNumConfigurations = 0x01
	};

	usb2_descriptors.other_descr.usb_cfg_descr = (USB_CFG_DESCR){
		.bLength = 0x09,
		.bDescriptorType = 0x02,
		.wTotalLength = sizeof(usb2_descriptors.other_descr),
		.bNumInterfaces = 0x01,
		.bConfigurationValue = 0x01,
		.iConfiguration = 0x00,
		.bmAttributes = 0xa0, // supports remote wake-up
		.MaxPower = 0x64 // 200ma
	};

	usb2_descriptors.other_descr.usb_itf_descr = (USB_ITF_DESCR){
		.bLength = 0x09,
		.bDescriptorType = 0x04,
		.bInterfaceNumber = 0x00,
		.bAlternateSetting = 0x00,
		.bNumEndpoints = 0x0e,
		.bInterfaceClass = 0xff, //vendor-specific
		.bInterfaceSubClass = 0xff,
		.bInterfaceProtocol = 0xff,
		.iInterface = 0x00
	};

	usb2_descriptors.other_descr.usb_endp_descr_1 = (USB_ENDP_DESCR){
		.bLength = 0x07,
		.bDescriptorType = 0x05,
		.bEndpointAddress = (ENDPOINT_DESCRIPTOR_ADDRESS_OUT | 0x01) & ENDPOINT_DESCRIPTOR_ADDRESS_MASK,
		.bmAttributes = ENDPOINT_DESCRIPTOR_BULK_TRANSFER,
		.wMaxPacketSizeL = 0x00,
		.wMaxPacketSizeH = 0x02, //512 bytes
		.bInterval = 255 // max NAK rate
	};

	usb2_descriptors.other_descr.usb_endp_descr_1_tx = (USB_ENDP_DESCR){
		.bLength = 0x07,
		.bDescriptorType = 0x05,
		.bEndpointAddress = (ENDPOINT_DESCRIPTOR_ADDRESS_IN | 0x01) & ENDPOINT_DESCRIPTOR_ADDRESS_MASK,
		.bmAttributes = ENDPOINT_DESCRIPTOR_BULK_TRANSFER,
		.wMaxPacketSizeL = 0x00,
		.wMaxPacketSizeH = 0x02, //512 bytes
		.bInterval = 0
	};

	usb2_descriptors.other_descr.usb_endp_descr_2 = (USB_ENDP_DESCR){
		.bLength = 0x07,
		.bDescriptorType = 0x05,
		.bEndpointAddress = (ENDPOINT_DESCRIPTOR_ADDRESS_OUT | 0x02) & ENDPOINT_DESCRIPTOR_ADDRESS_MASK,
		.bmAttributes = ENDPOINT_DESCRIPTOR_BULK_TRANSFER,
		.wMaxPacketSizeL = 0x00,
		.wMaxPacketSizeH = 0x02, //512 bytes
		.bInterval = 255 // max NAK rate
	};

	usb2_descriptors.other_descr.usb_endp_descr_2_tx = (USB_ENDP_DESCR){
		.bLength = 0x07,
		.bDescriptorType = 0x05,
		.bEndpointAddress = (ENDPOINT_DESCRIPTOR_ADDRESS_IN | 0x02) & ENDPOINT_DESCRIPTOR_ADDRESS_MASK,
		.bmAttributes = ENDPOINT_DESCRIPTOR_BULK_TRANSFER,
		.wMaxPacketSizeL = 0x00,
		.wMaxPacketSizeH = 0x02, //512 bytes
		.bInterval = 0
	};

	usb2_descriptors.other_descr.usb_endp_descr_3 = (USB_ENDP_DESCR){
		.bLength = 0x07,
		.bDescriptorType = 0x05,
		.bEndpointAddress = (ENDPOINT_DESCRIPTOR_ADDRESS_OUT | 0x03) & ENDPOINT_DESCRIPTOR_ADDRESS_MASK,
		.bmAttributes = ENDPOINT_DESCRIPTOR_BULK_TRANSFER,
		.wMaxPacketSizeL = 0x00,
		.wMaxPacketSizeH = 0x02, //512 bytes
		.bInterval = 255 // max NAK rate
	};

	usb2_descriptors.other_descr.usb_endp_descr_3_tx = (USB_ENDP_DESCR){
		.bLength = 0x07,
		.bDescriptorType = 0x05,
		.bEndpointAddress = (ENDPOINT_DESCRIPTOR_ADDRESS_IN | 0x03) & ENDPOINT_DESCRIPTOR_ADDRESS_MASK,
		.bmAttributes = ENDPOINT_DESCRIPTOR_BULK_TRANSFER,
		.wMaxPacketSizeL = 0x00,
		.wMaxPacketSizeH = 0x02, //512 bytes
		.bInterval = 0
	};

	usb2_descriptors.other_descr.usb_endp_descr_4 = (USB_ENDP_DESCR){
		.bLength = 0x07,
		.bDescriptorType = 0x05,
		.bEndpointAddress = (ENDPOINT_DESCRIPTOR_ADDRESS_OUT | 0x04) & ENDPOINT_DESCRIPTOR_ADDRESS_MASK,
		.bmAttributes = ENDPOINT_DESCRIPTOR_BULK_TRANSFER,
		.wMaxPacketSizeL = 0x00,
		.wMaxPacketSizeH = 0x02, //512 bytes
		.bInterval = 255 // max NAK rate
	};

	usb2_descriptors.other_descr.usb_endp_descr_4_tx = (USB_ENDP_DESCR){
		.bLength = 0x07,
		.bDescriptorType = 0x05,
		.bEndpointAddress = (ENDPOINT_DESCRIPTOR_ADDRESS_IN | 0x04) & ENDPOINT_DESCRIPTOR_ADDRESS_MASK,
		.bmAttributes = ENDPOINT_DESCRIPTOR_BULK_TRANSFER,
		.wMaxPacketSizeL = 0x00,
		.wMaxPacketSizeH = 0x02, //512 bytes
		.bInterval = 0
	};

	usb2_descriptors.other_descr.usb_endp_descr_5 = (USB_ENDP_DESCR){
		.bLength = 0x07,
		.bDescriptorType = 0x05,
		.bEndpointAddress = (ENDPOINT_DESCRIPTOR_ADDRESS_OUT | 0x05) & ENDPOINT_DESCRIPTOR_ADDRESS_MASK,
		.bmAttributes = ENDPOINT_DESCRIPTOR_BULK_TRANSFER,
		.wMaxPacketSizeL = 0x00,
		.wMaxPacketSizeH = 0x02, //512 bytes
		.bInterval = 255 // max NAK rate
	};

	usb2_descriptors.other_descr.usb_endp_descr_5_tx = (USB_ENDP_DESCR){
		.bLength = 0x07,
		.bDescriptorType = 0x05,
		.bEndpointAddress = (ENDPOINT_DESCRIPTOR_ADDRESS_IN | 0x05) & ENDPOINT_DESCRIPTOR_ADDRESS_MASK,
		.bmAttributes = ENDPOINT_DESCRIPTOR_BULK_TRANSFER,
		.wMaxPacketSizeL = 0x00,
		.wMaxPacketSizeH = 0x02, //512 bytes
		.bInterval = 0
	};

	usb2_descriptors.other_descr.usb_endp_descr_6 = (USB_ENDP_DESCR){
		.bLength = 0x07,
		.bDescriptorType = 0x05,
		.bEndpointAddress = (ENDPOINT_DESCRIPTOR_ADDRESS_OUT | 0x06) & ENDPOINT_DESCRIPTOR_ADDRESS_MASK,
		.bmAttributes = ENDPOINT_DESCRIPTOR_BULK_TRANSFER,
		.wMaxPacketSizeL = 0x00,
		.wMaxPacketSizeH = 0x02, //512 bytes
		.bInterval = 255 // max NAK rate
	};

	usb2_descriptors.other_descr.usb_endp_descr_6_tx = (USB_ENDP_DESCR){
		.bLength = 0x07,
		.bDescriptorType = 0x05,
		.bEndpointAddress = (ENDPOINT_DESCRIPTOR_ADDRESS_IN | 0x06) & ENDPOINT_DESCRIPTOR_ADDRESS_MASK,
		.bmAttributes = ENDPOINT_DESCRIPTOR_BULK_TRANSFER,
		.wMaxPacketSizeL = 0x00,
		.wMaxPacketSizeH = 0x02, //512 bytes
		.bInterval = 0
	};

	usb2_descriptors.other_descr.usb_endp_descr_7 = (USB_ENDP_DESCR){
		.bLength = 0x07,
		.bDescriptorType = 0x05,
		.bEndpointAddress = (ENDPOINT_DESCRIPTOR_ADDRESS_OUT | 0x07) & ENDPOINT_DESCRIPTOR_ADDRESS_MASK,
		.bmAttributes = ENDPOINT_DESCRIPTOR_BULK_TRANSFER,
		.wMaxPacketSizeL = 0x00,
		.wMaxPacketSizeH = 0x02, //512 bytes
		.bInterval = 255 // max NAK rate
	};

	usb2_descriptors.other_descr.usb_endp_descr_7_tx = (USB_ENDP_DESCR){
		.bLength = 0x07,
		.bDescriptorType = 0x05,
		.bEndpointAddress = (ENDPOINT_DESCRIPTOR_ADDRESS_IN | 0x07) & ENDPOINT_DESCRIPTOR_ADDRESS_MASK,
		.bmAttributes = ENDPOINT_DESCRIPTOR_BULK_TRANSFER,
		.wMaxPacketSizeL = 0x00,
		.wMaxPacketSizeH = 0x02, //512 bytes
		.bInterval = 0
	};

	usb2_device_configs[0] = (uint8_t*)&usb2_descriptors.other_descr;
}
#endif
