#ifndef _MAILBOX_INTERFACE_
#define _MAILBOX_INTERFACE_

#include "rpi_mailbox_interface.h"
#include "rpi_mailbox.h"

static int pt[8192] __attribute__((aligned(16)));
static int pt_index = 0;

void rpi_property_init() {
    pt[PT_OSIZE] = 12;
    pt[PT_OREQUEST_OR_RESPONSE] = 0;
    pt_index = 2;
    pt[pt_index] = 0;
}

void rpi_property_add_tag(rpi_mailbox_tag_t tag, ...) {
    va_list vl;
    va_start(vl, tag);

    pt[pt_index++] = tag;

    switch (tag) {
        case TAG_GET_FIRMWARE_VERSION:
        case TAG_GET_BOARD_MODEL:
        case TAG_GET_BOARD_REVISION:
        case TAG_GET_BOARD_MAC_ADDRESS:
        case TAG_GET_BOARD_SERIAL:
        case TAG_GET_ARM_MEMORY:
        case TAG_GET_VC_MEMORY:
        case TAG_GET_DMA_CHANNELS:
            /* Provide an 8-byte buffer for the response */
            pt[pt_index++] = 8;
            pt[pt_index++] = 0; /* Request */
            pt_index += 2;
            break;

        case TAG_GET_CLOCKS:
        case TAG_GET_COMMAND_LINE:
            /* Provide a 256-byte buffer */
            pt[pt_index++] = 256;
            pt[pt_index++] = 0; /* Request */
            pt_index += 256 >> 2;
            break;

        case TAG_ALLOCATE_BUFFER:
            pt[pt_index++] = 8;
            pt[pt_index++] = 0; /* Request */
            pt[pt_index++] = va_arg(vl, int);
            pt_index += 1;
            break;

        case TAG_GET_PHYSICAL_SIZE:
        case TAG_SET_PHYSICAL_SIZE:
        case TAG_TEST_PHYSICAL_SIZE:
        case TAG_GET_VIRTUAL_SIZE:
        case TAG_SET_VIRTUAL_SIZE:
        case TAG_TEST_VIRTUAL_SIZE:
        case TAG_GET_VIRTUAL_OFFSET:
        case TAG_SET_VIRTUAL_OFFSET:
            pt[pt_index++] = 8;
            pt[pt_index++] = 0; /* Request */

            if ((tag == TAG_SET_PHYSICAL_SIZE) ||
                (tag == TAG_SET_VIRTUAL_SIZE) ||
                (tag == TAG_SET_VIRTUAL_OFFSET) ||
                (tag == TAG_TEST_PHYSICAL_SIZE) ||
                (tag == TAG_TEST_VIRTUAL_SIZE)) {
                pt[pt_index++] = va_arg(vl, int); /* Width */
                pt[pt_index++] = va_arg(vl, int); /* Height */
            } else {
                pt_index += 2;
            }
            break;

        case TAG_GET_ALPHA_MODE:
        case TAG_SET_ALPHA_MODE:
        case TAG_GET_DEPTH:
        case TAG_SET_DEPTH:
        case TAG_GET_PIXEL_ORDER:
        case TAG_SET_PIXEL_ORDER:
        case TAG_GET_PITCH:
            pt[pt_index++] = 4;
            pt[pt_index++] = 0; /* Request */

            if ((tag == TAG_SET_DEPTH) ||
                (tag == TAG_SET_PIXEL_ORDER) ||
                (tag == TAG_SET_ALPHA_MODE)) {
                /* Colour Depth, bits-per-pixel \ Pixel Order State */
                pt[pt_index++] = va_arg(vl, int);
            } else {
                pt_index += 1;
            }
            break;

        case TAG_GET_OVERSCAN:
        case TAG_SET_OVERSCAN:
            pt[pt_index++] = 16;
            pt[pt_index++] = 0; /* Request */

            if ((tag == TAG_SET_OVERSCAN)) {
                pt[pt_index++] = va_arg(vl, int); /* Top pixels */
                pt[pt_index++] = va_arg(vl, int); /* Bottom pixels */
                pt[pt_index++] = va_arg(vl, int); /* Left pixels */
                pt[pt_index++] = va_arg(vl, int); /* Right pixels */
            } else {
                pt_index += 4;
            }
            break;

        default:
            /* Unsupported tags, just remove the tag from the list */
            pt_index--;
            break;
    }

    pt[pt_index] = 0;

    va_end(vl);
}

int rpi_property_process() {
    int result;

    /* Fill in the size of the buffer */
    pt[PT_OSIZE] = (pt_index + 1) << 2;
    pt[PT_OREQUEST_OR_RESPONSE] = 0;

    rpi_mailbox0write(MB0_TAGS_ARM_TO_VC, (unsigned int)pt);

    result = rpi_mailbox0read(MB0_TAGS_ARM_TO_VC);

    return result;
}

rpi_mailbox_property_t *rpi_property_get(rpi_mailbox_tag_t tag) {
    static rpi_mailbox_property_t property;
    int *tag_buffer = NULL;

    property.tag = tag;

    /* Get the tag from the buffer. Start at the first tag position  */
    int index = 2;

    while (index < (pt[PT_OSIZE] >> 2)) {
        /* printf( "Test Tag: [%d] %8.8X\r\n", index, pt[index] ); */
        if (pt[index] == tag) {
            tag_buffer = &pt[index];
            break;
        }

        /* Progress to the next tag if we haven't yet discovered the tag */
        index += (pt[index + 1] >> 2) + 3;
    }

    /* Return NULL of the property tag cannot be found in the buffer */
    if (tag_buffer == NULL)
        return NULL;

    /* Return the required data */
    property.byte_length = tag_buffer[T_ORESPONSE] & 0xFFFF;
    memcpy(property.data.buffer_8, &tag_buffer[T_OVALUE], property.byte_length);

    return &property;
}

#endif
