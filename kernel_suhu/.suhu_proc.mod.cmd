savedcmd_kernel_suhu/suhu_proc.mod := printf '%s\n'   kernel_suhu/suhu_proc.o | awk '!x[$$0]++ { print("./"$$0) }' > kernel_suhu/suhu_proc.mod
