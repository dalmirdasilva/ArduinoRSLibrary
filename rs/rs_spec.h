#include "rs.h"

#if VIRTUAL_ENVIROMENT == 1
#define RS_SPEC_DRIVER RS_DRIVER_VIRTUAL
#else
#define RS_SPEC_DRIVER RS_DRIVER_EXTERNAL_EEPROM
#endif

#ifndef rs_spec_printf
#define rs_spec_printf printf
#endif

#ifndef RS_SPEC_IGNORE_0

void format_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    if (op_r != RS_OP_RESULT_SUCCESS) {
        rs_spec_printf("(F) fomat spec failed. error: %d\n", op_r);
    } else {
        rs_spec_printf("(*) fomat spec passed.\n", RS_OP_RESULT_SUCCESS);
    }
}
#endif

#ifndef RS_SPEC_IGNORE_1

void mount_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    if (op_r != RS_OP_RESULT_SUCCESS) {
        rs_spec_printf("(F) mount spec failed. error: %d\n", op_r);
    } else {
        rs_spec_printf("(*) mount spec passed.\n", RS_OP_RESULT_SUCCESS);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_2

void umount_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    op_r = rs_umount(rs);
    if (op_r != RS_OP_RESULT_SUCCESS) {
        rs_spec_printf("(F) umount spec failed. error: %d\n", op_r);
    } else {
        rs_spec_printf("(*) umount spec passed.\n", RS_OP_RESULT_SUCCESS);
    }
}
#endif

#ifndef RS_SPEC_IGNORE_3

void alloc_resource_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_code_t rs_resource_code;
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    rs_resource_code = rs_alloc(rs);
    if (rs_resource_code == RS_NULL_RESOURCE_CODE) {
        rs_spec_printf("(F) alloc_resource spec failed. error: %d\n", op_r);
    } else {
        rs_spec_printf("(*) alloc_resource spec passed.\n", RS_OP_RESULT_SUCCESS);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_4

void try_to_alloc_resources_that_is_possible_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_code_t rs_resource_code[2];
    uint8_t i = 0;
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    for (; i < rs->resource_descriptor_count; i++) {
        rs_resource_code[0] = rs_alloc(rs);
    }
    rs_resource_code[1] = rs_alloc(rs);
    if (rs_resource_code[0] == (rs->resource_descriptor_count - 1) && rs_resource_code[1] == RS_NULL_RESOURCE_CODE) {
        rs_spec_printf("(*) try_to_alloc_resources_that_is_possible spec passed.\n", RS_OP_RESULT_SUCCESS);
    } else {
        rs_spec_printf("(F) try_to_alloc_resources_that_is_possible spec failed. error: %x\n", rs_resource_code[0]);
        while (1);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_5

void open_resource_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_code_t rs_resource_code;
    rs_resource_t resource;
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    rs_resource_code = rs_alloc(rs);
    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
    if (op_r != RS_OP_RESULT_SUCCESS) {
        rs_spec_printf("(F) open_resource spec failed. error: %d\n", op_r);
    } else {
        rs_spec_printf("(*) open_resource spec passed.\n", RS_OP_RESULT_SUCCESS);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_6

void write_resource_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_code_t rs_resource_code;
    rs_resource_t resource;
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    rs_resource_code = rs_alloc(rs);
    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = rs_write(rs, &resource, 0xaa);
    if (op_r != RS_OP_RESULT_SUCCESS) {
        rs_spec_printf("(F) write_resource spec failed. error: %d\n", op_r);
    } else {
        rs_spec_printf("(*) write_resource spec passed.\n", RS_OP_RESULT_SUCCESS);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_7

void rewind_resource_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_code_t rs_resource_code;
    rs_resource_t resource;
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    rs_resource_code = rs_alloc(rs);
    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = rs_write(rs, &resource, 0xAA);
    op_r = rs_rewind(rs, &resource);
    if (op_r != RS_OP_RESULT_SUCCESS) {
        rs_spec_printf("(F) rewind_resource spec failed. error: %d\n", op_r);
    } else {
        rs_spec_printf("(*) rewind_resource spec passed.\n", RS_OP_RESULT_SUCCESS);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_8

void read_resource_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_code_t rs_resource_code;
    rs_resource_t resource;
    unsigned char c[2];
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    rs_resource_code = rs_alloc(rs);
    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = rs_write(rs, &resource, 0x41);
    op_r = rs_write(rs, &resource, 0xA1);
    op_r = rs_rewind(rs, &resource);
    c[0] = rs_read(rs, &resource);
    c[1] = rs_read(rs, &resource);
    if (c[0] != 0x41 || c[1] != 0xA1) {
        rs_spec_printf("(F) read_resource spec failed. error: %x\n", c[0]);
    } else {
        rs_spec_printf("(*) read_resource spec passed.\n", RS_OP_RESULT_SUCCESS);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_9

void close_resource_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_code_t rs_resource_code;
    rs_resource_t resource;
    unsigned char c[2];
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    rs_resource_code = rs_alloc(rs);
    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = rs_close(rs, &resource);
    if (op_r != RS_OP_RESULT_SUCCESS) {
        rs_spec_printf("(F) close_resource spec failed. error: %x\n", op_r);
    } else {
        rs_spec_printf("(*) close_resource spec passed.\n", RS_OP_RESULT_SUCCESS);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_10

void try_read_when_end_of_resource_is_reached_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_code_t rs_resource_code;
    rs_resource_t resource;
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    rs_resource_code = rs_alloc(rs);
    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = rs_write(rs, &resource, 0x41);
    op_r = rs_rewind(rs, &resource);
    rs_read(rs, &resource);
    rs_read(rs, &resource);
    if (op_r == 0 && (rs_eor(&resource))) {
        rs_spec_printf("(*) try_read_when_end_of_resource_is_reached spec passed.\n", RS_OP_RESULT_SUCCESS);
    } else {
        rs_spec_printf("(F) try_read_when_end_of_resource_is_reached spec failed. error: %x\n", op_r);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_11

void try_read_when_resource_is_closed_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_code_t rs_resource_code;
    rs_resource_t resource;
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    rs_resource_code = rs_alloc(rs);
    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = rs_close(rs, &resource);
    rs_read(rs, &resource);
    if (op_r == 0 && (resource.flags | RS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_READ)) {
        rs_spec_printf("(*) try_read_when_resource_is_closed spec passed.\n", RS_OP_RESULT_SUCCESS);
    } else {
        rs_spec_printf("(F) try_read_when_resource_is_closed spec failed. error: %x\n", op_r);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_12

void seek_resource_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_code_t rs_resource_code;
    rs_resource_t resource;
    uint8_t i = 0;
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    rs_resource_code = rs_alloc(rs);
    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < 50; i++) {
        op_r = rs_write(rs, &resource, (i + 0x65));
    }
    op_r = rs_seek(rs, &resource, RS_SEEK_ORIGIN_BEGIN, 20);
    if (op_r == RS_OP_RESULT_SUCCESS) {
        rs_spec_printf("(*) seek_resource spec passed.\n", RS_OP_RESULT_SUCCESS);
    } else {
        rs_spec_printf("(F) seek_resource spec failed. error: %x\n", op_r);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_13

void random_read_resource_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_code_t rs_resource_code;
    rs_resource_t resource;
    uint8_t i = 0;
    unsigned char c[5], first_write_char = 0x65;
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    rs_resource_code = rs_alloc(rs);
    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < 255; i++) {
        op_r = rs_write(rs, &resource, (i + first_write_char));
    }
    op_r = rs_seek(rs, &resource, RS_SEEK_ORIGIN_BEGIN, 20);
    c[0] = rs_read(rs, &resource);
    op_r = rs_seek(rs, &resource, RS_SEEK_ORIGIN_CURRENT, 10);
    c[1] = rs_read(rs, &resource);
    op_r = rs_seek(rs, &resource, RS_SEEK_ORIGIN_BEGIN, 48);
    c[2] = rs_read(rs, &resource);
    op_r = rs_seek(rs, &resource, RS_SEEK_ORIGIN_CURRENT, 20);
    c[3] = rs_read(rs, &resource);
    op_r = rs_seek(rs, &resource, RS_SEEK_ORIGIN_BEGIN, 0);
    c[4] = rs_read(rs, &resource);
    if (c[0] == first_write_char + 20 &&
            c[1] == first_write_char + 31 &&
            c[2] == first_write_char + 48 &&
            c[3] == first_write_char + 69 &&
            c[4] == first_write_char + 0) {
        rs_spec_printf("(*) random_read_resource spec passed.\n", RS_OP_RESULT_SUCCESS);
    } else {
        rs_spec_printf("(F) random_read_resource spec failed. error: %x\n", op_r);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_14

void random_read_with_seek_resource_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_code_t rs_resource_code;
    rs_resource_t resource;
    uint8_t i = 0;
    unsigned char c[255];
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    rs_resource_code = rs_alloc(rs);
    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
    for (i = 0; i < 255; i++) {
        op_r = rs_write(rs, &resource, i);
    }
    rs_rewind(rs, &resource);

    for (i = 0; i < 255; i++) {
        rs_seek(rs, &resource, RS_SEEK_ORIGIN_BEGIN, i);
        c[i] = rs_read(rs, &resource);
        rs_read(rs, &resource);
        rs_read(rs, &resource);
        rs_read(rs, &resource);
    }

    op_r = rs_seek(rs, &resource, RS_SEEK_ORIGIN_BEGIN, 199);
    if ((i = rs_read(rs, &resource)) != 199) {
        rs_spec_printf("(F) random_read_with_seek_resource_spec spec failed. != 199\n", 0);
    }

    for (i = 0; i < 255; i++) {
        if (i != c[i]) {
            rs_spec_printf("(F) random_read_with_seek_resource_spec spec failed. error: %x\n", i);
        }
    }

    rs_spec_printf("(*) random_read_with_seek_resource_spec spec passed.\n", RS_OP_RESULT_SUCCESS);
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_15

void random_read_with_seek_opening_resource_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_code_t rs_resource_code;
    rs_resource_t resource;
    uint8_t i = 0;
    unsigned char c[255];
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    rs_resource_code = rs_alloc(rs);
    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
    for (i = 0; i < 255; i++) {
        op_r = rs_write(rs, &resource, i);
    }
    rs_close(rs, &resource);

    for (i = 0; i < 255; i++) {
        op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
        rs_seek(rs, &resource, RS_SEEK_ORIGIN_BEGIN, i);
        c[i] = rs_read(rs, &resource);
        rs_read(rs, &resource);
        rs_read(rs, &resource);
        rs_read(rs, &resource);
        rs_close(rs, &resource);
    }

    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = rs_seek(rs, &resource, RS_SEEK_ORIGIN_BEGIN, 199);
    if ((i = rs_read(rs, &resource)) != 199) {
        rs_spec_printf("(F) random_read_with_seek_opening_resource_spec spec failed. != 199\n", op_r);
    }
    rs_close(rs, &resource);

    for (i = 0; i < 255; i++) {
        if (i != c[i]) {
            rs_spec_printf("(F) random_read_with_seek_opening_resource_spec spec failed. error: %x\n", c[i]);
        }
    }

    rs_spec_printf("(*) random_read_with_seek_opening_resource_spec spec passed.\n", RS_OP_RESULT_SUCCESS);
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_16

void size_resource_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_code_t rs_resource_code;
    rs_resource_t resource;
    uint16_t i = 0;
    uint16_t size = 0xf40;
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    rs_resource_code = rs_alloc(rs);
    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < size; i++) {
        op_r = rs_write(rs, &resource, 0x65);
    }
    if (rs_size(&resource) == 0xf40) {
        rs_spec_printf("(*) size_resource spec passed.\n", RS_OP_RESULT_SUCCESS);
    } else {
        rs_spec_printf("(F) size_resource spec failed. error: %x\n", size);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_17

void tell_resource_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_code_t rs_resource_code;
    rs_resource_t resource;
    uint8_t i = 0;
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    rs_resource_code = rs_alloc(rs);
    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < 50; i++) {
        op_r = rs_write(rs, &resource, 0x65);
    }
    if (rs_tell(&resource) == 50) {
        rs_spec_printf("(*) tell_resource spec passed.\n", RS_OP_RESULT_SUCCESS);
    } else {
        rs_spec_printf("(F) tell_resource spec failed. error: %x\n", op_r);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_18

void tell_with_seek_resource_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_code_t rs_resource_code;
    rs_resource_t resource;
    uint8_t i = 0;
    rs_resource_size_t s[5];
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    rs_resource_code = rs_alloc(rs);
    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < 50; i++) {
        op_r = rs_write(rs, &resource, 0x65);
    }
    op_r = rs_seek(rs, &resource, RS_SEEK_ORIGIN_BEGIN, 20);
    s[0] = rs_tell(&resource);
    op_r = rs_seek(rs, &resource, RS_SEEK_ORIGIN_CURRENT, 10);
    s[1] = rs_tell(&resource);
    op_r = rs_seek(rs, &resource, RS_SEEK_ORIGIN_BEGIN, 48);
    s[2] = rs_tell(&resource);
    op_r = rs_seek(rs, &resource, RS_SEEK_ORIGIN_CURRENT, 20);
    s[3] = rs_tell(&resource);
    op_r = rs_seek(rs, &resource, RS_SEEK_ORIGIN_BEGIN, 0);
    s[4] = rs_tell(&resource);
    if (s[0] == 20 &&
            s[1] == 30 &&
            s[2] == 48 &&
            s[3] == 17 &&
            s[4] == 0) {
        rs_spec_printf("(*) tell_with_seek_resource spec passed.\n", RS_OP_RESULT_SUCCESS);
    } else {
        rs_spec_printf("(F) tell_with_seek_resource spec failed. error: %d\n", s[3]);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_19

void total_space_resource_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_code_t rs_resource_code;
    rs_resource_t resource;
    rs_resource_size_t total_space[2];
    uint16_t i = 0;
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    total_space[0] = rs_available_space(rs);
    rs_resource_code = rs_alloc(rs);
    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < rs->sizeof_cluster_data + 1; i++) {
        op_r = rs_write(rs, &resource, 0x65);
    }
    total_space[1] = rs_available_space(rs);
    if (total_space[0] - total_space[1] == (rs->sizeof_cluster_data * 2)) {
        rs_spec_printf("(*) total_space_resource spec passed.\n", RS_OP_RESULT_SUCCESS);
    } else {
        rs_spec_printf("(F) total_space_resource spec failed. error: %d != 50\n", total_space[0] - total_space[1]);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_20

void allocating_multi_format_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_t resource;
    uint8_t count = 3;
    uint8_t j, i;
    rs_resource_code_t rs_resource_code[3];
    uint8_t passed = 1;
    for (j = 0; j < count; j++) {
        rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
        op_r = rs_format(rs);
        op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
        rs_resource_code[j] = rs_alloc(rs);
        op_r = rs_open(rs, rs_resource_code[j], &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
        for (i = 0; i < 50; i++) {
            op_r = rs_write(rs, &resource, 0x65);
        }
        rs_close(rs, &resource);
    }
    for (j = 0; j < count; j++) {
        if (rs_resource_code[j] != 0) {
            rs_spec_printf("(F) allocating_multi_format spec failed %x\n", rs_resource_code[j]);
            passed = 0;
        }
    }
    if (passed) {
        rs_spec_printf("(*) allocating_multi_format spec passed\n", RS_OP_RESULT_SUCCESS);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_21

void read_only_mounting_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_t resource;
    rs_resource_code_t rs_resource_code;
    char error_msg[] = "(F) read_only_mounting spec failed. %d\n";
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_READ_ONLY);
    rs_resource_code = rs_alloc(rs);
    if (rs_resource_code == RS_NULL_RESOURCE_CODE) {
        rs_spec_printf(error_msg, op_r);
    }
    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_NORMAL);
    if (op_r != RS_OP_RESULT_SUCCESS) {
        rs_spec_printf(error_msg, op_r);
    }
    op_r = rs_write(rs, &resource, 0xaa);
    if (op_r == RS_OP_RESULT_ERROR_RESOURCE_READ_ONLY) {
        rs_spec_printf("(*) read_only_mounting spec passed.\n", RS_OP_RESULT_SUCCESS);
    } else {
        rs_spec_printf(error_msg, op_r);
    }
    rs_umount(rs);
}
#endif

#ifndef RS_SPEC_IGNORE_22

void read_only_opening_spec(rs_t *rs) {
    rs_op_result_t op_r;
    rs_resource_t resource;
    rs_resource_code_t rs_resource_code;
    char error_msg[] = "(F) read_only_opening spec failed. %d\n";
    rs_init_partition(rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    op_r = rs_format(rs);
    op_r = rs_mount(RS_SPEC_DRIVER, rs, RS_MOUNT_OPTION_NORMAL);
    rs_resource_code = rs_alloc(rs);
    if (rs_resource_code == RS_NULL_RESOURCE_CODE) {
        rs_spec_printf(error_msg, op_r);
    }
    op_r = rs_open(rs, rs_resource_code, &resource, RS_OPEN_RESOURCE_OPTION_READ_ONLY);
    if (op_r != RS_OP_RESULT_SUCCESS) {
        rs_spec_printf(error_msg, op_r);
    }
    op_r = rs_write(rs, &resource, 0xaa);
    if (op_r == RS_OP_RESULT_ERROR_RESOURCE_READ_ONLY) {
        rs_spec_printf("(*) read_only_opening spec passed.\n", RS_OP_RESULT_SUCCESS);
    } else {
        rs_spec_printf(error_msg, op_r);
    }
    rs_umount(rs);
}
#endif