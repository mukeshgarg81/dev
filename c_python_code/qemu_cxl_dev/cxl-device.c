#include "hw/pci/pci.h"
#include "qemu/log.h"

typedef struct CXLDevice {
    PCIDevice parent_obj;
    MemoryRegion bar; // BAR memory region
    uint32_t doorbell; // Doorbell register

    /* Add your device-specific state here */
} CXLDevice;

static uint64_t cxl_bar_read(void *opaque, hwaddr offset, unsigned size) {
    CXLDevice *cdev = opaque;
    return memory_region_read(&cdev->bar, offset, size);
}

static void cxl_bar_write(void *opaque, hwaddr offset, uint64_t value, unsigned size) {
    CXLDevice *cdev = opaque;
    memory_region_write(&cdev->bar, offset, value, size);
}

static uint64_t cxl_doorbell_read(void *opaque, hwaddr offset, unsigned size) {
    CXLDevice *cdev = opaque;
    return cdev->doorbell;
}

static void cxl_doorbell_write(void *opaque, hwaddr offset, uint64_t value, unsigned size) {
    CXLDevice *cdev = opaque;
    cdev->doorbell = value;

    /* Handle doorbell interrupt or other action here */
}

static void cxl_device_realize(PCIDevice *dev, Error **errp) {
    CXLDevice *cdev = CXL_DEVICE(dev);
    
    /* Initialize BAR and doorbell memory regions */
    memory_region_init_io(&cdev->bar, OBJECT(cdev), &cxl_bar_ops, cdev, "cxl-bar", 0x1000);
    
    /* Register memory regions */
    pci_register_bar(dev, 0, PCI_BASE_ADDRESS_MEM_PREFETCH, &cdev->bar);
    /* Add other BARs as needed */

    /* Register doorbell MMIO */
    memory_region_init_io(&cdev->doorbell, OBJECT(cdev), &cxl_doorbell_ops, cdev, "cxl-doorbell", sizeof(uint32_t));
    pci_register_bar(dev, 1, PCI_BASE_ADDRESS_SPACE_IO, &cdev->doorbell);
}

static Property cxl_device_properties[] = {
    /* Define your device properties here */
    DEFINE_PROP_END_OF_LIST(),
};

static void cxl_device_class_init(ObjectClass *klass, void *data) {
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);

    k->realize = cxl_device_realize;
    k->props = cxl_device_properties;
}

static const TypeInfo cxl_device_info = {
    .name          = "cxl-device",
    .parent        = TYPE_PCI_DEVICE,
    .instance_size = sizeof(CXLDevice),
    .class_init    = cxl_device_class_init,
};

static void cxl_device_register_types(void) {
    type_register_static(&cxl_device_info);
}

type_init(cxl_device_register_types)


