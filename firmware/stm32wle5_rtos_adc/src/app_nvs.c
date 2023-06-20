#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/random/rand32.h>

#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#include "app_nvs.h"

int nvs_initialise(struct nvs_fs *fs) {
	struct flash_pages_info info;
	int ret;

	fs->flash_device = NVS_PARTITION_DEVICE;
	if (!device_is_ready(fs->flash_device)) {
		printk("Flash device %s is not ready\n", fs->flash_device->name);
		return;
	}
	fs->offset = NVS_PARTITION_OFFSET;
	ret = flash_get_page_info_by_offs(fs->flash_device, fs->offset, &info);
	if (ret) {
		printk("Unable to get page info\n");
		return;
	}
	fs->sector_size = info.size;
	fs->sector_count = 3U;

	ret = nvs_mount(fs);
	if (ret) {
		printk("Flash Init failed\n");
		return;
	}

#ifdef NVS_CLEAR
	ret = nvs_clear(fs);
	if (ret) {
		printk("Flash Clear failed\n");
		return;
	} else {
		printk("Cleared NVS from flash\n");
	}
#endif
    return 0;
}

size_t nvs_write_data(struct nvs_fs *fs, uint16_t id, void *data, size_t nvs_len) {
    int ret;
    ret = nvs_read(&fs, id, &data, nvs_len);
    if (ret > 0) {
		printk("Id: %d, Address: %s\n", ADDRESS_ID, data);
    }
    else {
        printk("No address found, adding %s at id %d\n", data, ADDRESS_ID);
        nvs_write(&fs, id, &data, nvs_len);
    }
    return 0;
}