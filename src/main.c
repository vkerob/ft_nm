#include "../includes/ft_nm.h"

int global_uninit_var;        // .bss, global (B)
static int local_uninit_var;  // .bss, local (b)

int global_init_var = 42;     // .data, global (D)
static int local_init_var = 24; // .data, local (d)

void global_func() {          // .text, global (T)
    ft_printf("Global function\n");
}

static void local_func() {    // .text, local (t)
    ft_printf("Local function\n");
}

int page_size;

int main(int argc, const char **argv)
{
	int 		i = 1;
	int			fd;
	char		*default_file = "a.out";
	struct stat	file_stat;
	bool		retval = true;

    page_size = getpagesize();

	if (argc == 1)
	{
		display_files("a.out");
	}
	
	i = 1;
	while (i < argc)
	{
		display_files(argv[i]);
		i++;
	}

	exit (retval);
    return retval;
}


// ➜  ft_nm git:(main) ✗ nm ft_nm     
// 000000000040037c r __abi_tag
// 0000000000404034 B __bss_start
// 0000000000404034 b completed.0
// 0000000000404020 D __data_start
// 0000000000404020 W data_start
// 0000000000401080 t deregister_tm_clones
// 0000000000401070 T _dl_relocate_static_pie
// 00000000004010f0 t __do_global_dtors_aux
// 0000000000403e08 d __do_global_dtors_aux_fini_array_entry
// 0000000000404028 D __dso_handle
// 0000000000403e10 d _DYNAMIC
// 0000000000404034 D _edata
// 0000000000404040 B _end
// 0000000000401c00 T _fini
// 0000000000401120 t frame_dummy
// 0000000000403e00 d __frame_dummy_init_array_entry
// 00000000004022e8 r __FRAME_END__
// 0000000000401ae0 T ft_adress
// 0000000000401160 T ft_check
// 00000000004012e0 t ft_forest
// 0000000000401210 T ft_parcours
// 00000000004016b0 T ft_printf
// 0000000000401850 T ft_putchar_ct
// 0000000000401930 T ft_putnbr_ct_1
// 0000000000401a10 T ft_putnbr_ct_2
// 0000000000401b60 t ft_putnbr_lunsigned_base_ct
// 0000000000401880 T ft_putstr_ct
// 0000000000401aa0 T ft_strlen
// 0000000000401130 T global_func
// 0000000000404030 D global_init_var
// 0000000000404000 d _GLOBAL_OFFSET_TABLE_
// 0000000000404038 B global_uninit_var
//                  w __gmon_start__
// 0000000000402054 r __GNU_EH_FRAME_HDR
// 0000000000401000 T _init
// 0000000000402000 R _IO_stdin_used
//                  U __libc_start_main@GLIBC_2.34
// 0000000000401150 T main
// 00000000004010b0 t register_tm_clones
// 0000000000401040 T _start
// 0000000000404038 D __TMC_END__
//                  U write@GLIBC_2.2.5