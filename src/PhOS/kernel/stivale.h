#ifndef STIVALE_H
#define STIVALE_H
#include <stivale2.h>

#include "common.h"

typedef struct stivale2_anchor               stivale2_anchor_t;
typedef struct stivale2_tag                  stivale2_tag_t;
typedef struct stivale2_header               stivale2_header_t;
typedef struct stivale2_header_tag_any_video stivale2_header_tag_any_video_t;
typedef struct stivale2_header_tag_framebuffer
                                            stivale2_header_tag_framebuffer_t;
typedef struct stivale2_header_tag_terminal stivale2_header_tag_terminal_t;
typedef struct stivale2_header_tag_smp      stivale2_header_tag_smp_t;
typedef struct stivale2_struct              stivale2_struct_t;
typedef struct stivale2_struct_tag_pmrs     stivale2_struct_tag_pmrs_t;
typedef struct stivale2_struct_tag_cmdline  stivale2_struct_tag_cmdline_t;
typedef struct stivale2_mmap_entry          stivale2_mmap_entry_t;
typedef struct stivale2_struct_tag_memmap   stivale2_struct_tag_memmap_t;
typedef struct stivale2_struct_tag_framebuffer
                                            stivale2_struct_tag_framebuffer_t;
typedef struct stivale2_struct_tag_edid     stivale2_struct_tag_edid_t;
typedef struct stivale2_struct_tag_textmode stivale2_struct_tag_textmode_t;
typedef struct stivale2_stivale2_struct_tag_modulesmodule
                                            stivale2_struct_tag_modules_t;
typedef struct stivale2_struct_tag_rsdp     stivale2_struct_tag_rsdp_t;
typedef struct stivale2_struct_tag_epoch    stivale2_struct_tag_epoch_t;
typedef struct stivale2_struct_tag_firmware stivale2_struct_tag_firmware_t;
typedef struct stivale2_struct_tag_efi_system_table
    stivale2_struct_tag_efi_system_table_t;
typedef struct stivale2_struct_tag_kernel_file
    stivale2_struct_tag_kernel_file_t;
typedef struct stivale2_struct_tag_kernel_slide
                                          stivale2_struct_tag_kernel_slide_t;
typedef struct stivale2_struct_tag_smbios stivale2_struct_tag_smbios_t;
typedef struct stivale2_smp_info          stivale2_smp_info_t;
typedef struct stivale2_struct_tag_smp    stivale2_struct_tag_smp_t;
typedef struct stivale2_struct_tag_pxe_server_info
    stivale2_struct_tag_pxe_server_info_t;
typedef struct stivale2_struct_tag_mmio32_uart
                                    stivale2_struct_tag_mmio32_uart_t;
typedef struct stivale2_struct_vmap stivale2_struct_vmap_t;

extern void* stivale2GetTag(stivale2_struct_t* bootloader_data, uint64_t id);

#endif // STIVALE_H