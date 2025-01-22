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

int main()
{
    return 0;
}
