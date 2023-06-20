#ifndef __NVS__H
#define __NVS__H

#define NVS_PARTITION			storage_partition
#define NVS_PARTITION_DEVICE	FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET	FIXED_PARTITION_OFFSET(NVS_PARTITION)
#define ADDRESS_ID 1

int nvs_initialise(struct nvs_fs *fs);
size_t nvs_write_data(struct nvs_fs *fs, uint16_t id, void *data, size_t nvs_len);

#endif