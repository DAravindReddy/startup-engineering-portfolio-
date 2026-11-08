# Device Tree Structure & Hardware Bindings (DTS/DTB)

**Author:** Aravind Reddy  
**Organization:** Kernelwise Labs  

---

## 1. Overview of Flattened Device Trees (FDT)

On ARM and non-x86 architectures without auto-discoverable buses (like ACPI or PCI), the Linux kernel relies on Device Trees (`.dts` source compiled to `.dtb` binary by `dtc`) to discover board peripherals, base registers, IRQ vectors, and clock trees.

---

## 2. Practical Device Tree Snippets

### I2C Sensor Node (MPU6050 Accelerometer / Gyroscope)
```dts
&i2c1 {
    status = "okay";
    clock-frequency = <400000>; /* Fast-mode 400 kHz */

    mpu6050@68 {
        compatible = "invensense,mpu6050";
        reg = <0x68>;
        interrupt-parent = <&gpio1>;
        interrupts = <17 IRQ_TYPE_EDGE_RISING>;
        mount-matrix = "1", "0", "0",
                       "0", "1", "0",
                       "0", "0", "1";
    };
};
```

### SPI Flash Memory Node
```dts
&spi0 {
    status = "okay";

    flash@0 {
        compatible = "spansion,s25fl256s1", "jedec,spi-nor";
        reg = <0>; /* Chip Select 0 */
        spi-max-frequency = <50000000>; /* 50 MHz */
        m25p,fast-read;

        partitions {
            compatible = "fixed-partitions";
            #address-cells = <1>;
            #size-cells = <1>;

            partition@0 {
                label = "u-boot-spl";
                reg = <0x00000000 0x00040000>;
                read-only;
            };

            partition@40000 {
                label = "u-boot";
                reg = <0x00040000 0x00100000>;
            };
        };
    };
};
```

---

## 3. Kernel Driver Binding

When the kernel loads a driver, the driver's `of_device_id` table is matched against the DT node's `compatible` property:
```c
static const struct of_device_id kw_sensor_of_match[] = {
    { .compatible = "kernelwise,sensor-node-v1", },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, kw_sensor_of_match);
```
Upon a successful string match, the driver's `probe()` callback is invoked with the device's hardware resources.
