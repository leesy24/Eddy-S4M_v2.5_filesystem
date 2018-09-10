cmd_networking/ifconfig.o := /opt/lemonix/cdt/bin/arm-linux-gcc -Wp,-MD,networking/.ifconfig.o.d   -std=gnu99 -Iinclude -Ilibbb  -I/home/WORK/eddy-25/filesystem_2.5.1.1/src/busybox-1.5.0/libbb -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG  -D"BB_VER=KBUILD_STR(1.5.0)" -DBB_BT=AUTOCONF_TIMESTAMP  -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Os -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Wdeclaration-after-statement -Wno-pointer-sign    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(ifconfig)"  -D"KBUILD_MODNAME=KBUILD_STR(ifconfig)" -c -o networking/ifconfig.o networking/ifconfig.c

deps_networking/ifconfig.o := \
  networking/ifconfig.c \
    $(wildcard include/config/feature/ifconfig/slip.h) \
    $(wildcard include/config/slip.h) \
    $(wildcard include/config/feature/ipv6.h) \
    $(wildcard include/config/feature/ifconfig/broadcast/plus.h) \
    $(wildcard include/config/broadcast/plus.h) \
    $(wildcard include/config/feature/ifconfig/hw.h) \
    $(wildcard include/config/hw.h) \
    $(wildcard include/config/feature/ifconfig/memstart/ioaddr/irq.h) \
    $(wildcard include/config/memstart/ioaddr/irq.h) \
    $(wildcard include/config/feature/ifconfig/status.h) \
    $(wildcard include/config/status.h) \
    $(wildcard include/config/feature/clean/up.h) \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/net/if.h \
    $(wildcard include/config/c.h) \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/features.h \
    $(wildcard include/config/c99.h) \
    $(wildcard include/config/ix.h) \
    $(wildcard include/config/ix2.h) \
    $(wildcard include/config/ix199309.h) \
    $(wildcard include/config/ix199506.h) \
    $(wildcard include/config/en.h) \
    $(wildcard include/config/en/extended.h) \
    $(wildcard include/config/x98.h) \
    $(wildcard include/config/en2k.h) \
    $(wildcard include/config/gefile.h) \
    $(wildcard include/config/gefile64.h) \
    $(wildcard include/config/e/offset64.h) \
    $(wildcard include/config/.h) \
    $(wildcard include/config/d.h) \
    $(wildcard include/config/ile.h) \
    $(wildcard include/config/ntrant.h) \
    $(wildcard include/config/tify/level.h) \
    $(wildcard include/config/i.h) \
    $(wildcard include/config/ern/inlines.h) \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/cdefs.h \
    $(wildcard include/config/espaces.h) \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/wordsize.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/gnu/stubs.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/types.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/types.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/include/stddef.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/typesizes.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/time.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/endian.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/endian.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/select.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/select.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/sigset.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/time.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/sysmacros.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/pthreadtypes.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/socket.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/uio.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/uio.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/socket.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/include/limits.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/sockaddr.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/asm/socket.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/asm/sockios.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/net/if_arp.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/netinet/in.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/stdint.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/wchar.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/in.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/byteswap.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/netpacket/packet.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/net/ethernet.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/linux/if_ether.h \
    $(wildcard include/config/sysctl.h) \
  include/inet_common.h \
  include/platform.h \
    $(wildcard include/config///.h) \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/byteswap.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/arpa/inet.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/include/stdbool.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/mount.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/ioctl.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/ioctls.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/asm/ioctls.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/asm/ioctl.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/ioctl-types.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/ttydefaults.h \
  include/busybox.h \
  include/libbb.h \
    $(wildcard include/config/selinux.h) \
    $(wildcard include/config/locale/support.h) \
    $(wildcard include/config/feature/shadowpasswds.h) \
    $(wildcard include/config/lfs.h) \
    $(wildcard include/config/feature/buffers/go/on/stack.h) \
    $(wildcard include/config/buffer.h) \
    $(wildcard include/config/ubuffer.h) \
    $(wildcard include/config/feature/buffers/go/in/bss.h) \
    $(wildcard include/config/getopt/long.h) \
    $(wildcard include/config/route.h) \
    $(wildcard include/config/gunzip.h) \
    $(wildcard include/config/ktop.h) \
    $(wildcard include/config/feature/exec/prefer/applets.h) \
    $(wildcard include/config/busybox/exec/path.h) \
    $(wildcard include/config/feature/editing.h) \
    $(wildcard include/config/feature/editing/history.h) \
    $(wildcard include/config/ture/editing/savehistory.h) \
    $(wildcard include/config/feature/editing/savehistory.h) \
    $(wildcard include/config/feature/tab/completion.h) \
    $(wildcard include/config/feature/username/completion.h) \
    $(wildcard include/config/feature/editing/vi.h) \
    $(wildcard include/config/inux.h) \
    $(wildcard include/config/feature/devfs.h) \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/ctype.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/xlocale.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/dirent.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/dirent.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/posix1_lim.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/local_lim.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/linux/limits.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/errno.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/errno.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/linux/errno.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/asm/errno.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/asm-generic/errno.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/asm-generic/errno-base.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/fcntl.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/fcntl.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/stat.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/stat.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/inttypes.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/netdb.h \
    $(wildcard include/config/3/ascii/rules.h) \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/rpc/netdb.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/siginfo.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/netdb.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/setjmp.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/setjmp.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/signal.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/signum.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/sigaction.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/sigcontext.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/asm/sigcontext.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/sigstack.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/ucontext.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/procfs.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/time.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/user.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/sigthread.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/stdio.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/libio.h \
    $(wildcard include/config/a.h) \
    $(wildcard include/config/ar/t.h) \
    $(wildcard include/config//io/file.h) \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/_G_config.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/wchar.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/gconv.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/include/stdarg.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/stdio_lim.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/sys_errlist.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/stdlib.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/waitflags.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/waitstatus.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/alloca.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/string.h \
    $(wildcard include/config/ing/inlines.h) \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/strings.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/mman.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/mman.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/statfs.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/statfs.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/wait.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/resource.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/resource.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/termios.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/termios.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/unistd.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/posix_opt.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/environments.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/bits/confname.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/getopt.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/utime.h \
  include/pwd_.h \
    $(wildcard include/config/use/bb/pwd/grp.h) \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/pwd.h \
  include/grp_.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/grp.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/sys/param.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/linux/param.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/asm/param.h \
  include/xatonum.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/net/if_slip.h \
  /opt/lemonix/cdt/bin/../lib/gcc/arm-linux-gnueabi/4.1.1/../../../../arm-linux-gnueabi/include/linux/if_slip.h \

networking/ifconfig.o: $(deps_networking/ifconfig.o)

$(deps_networking/ifconfig.o):
