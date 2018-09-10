#!/bin/sh
# Cheesy hacky location of additional link libraries.

LIB_PATH=${COMPILER_HOME}/target

locate_library() { [ ! "$1*" = "`echo $1*`" ]; }
find_func() { egrep $1 $2 >/dev/null; }

if find_func hosts_access tcpwrap.o; then
  echo "-lwrap";
  locate_library /lib/libnsl.so && echo "-lnsl";
fi

# Look for PAM (done weirdly due to distribution bugs (e.g. Debian) or the
# crypt library.
if find_func pam_start sysdeputil.o; then
  locate_library ${LIB_PATH}/lib/libpam.so.0 && echo "${LIB_PATH}/lib/libpam.so.0";
  locate_library ${LIB_PATH}/usr/lib/libpam.so && echo "-lpam";
  # HP-UX ends shared libraries with .sl
  locate_library ${LIB_PATH}/usr/lib/libpam.sl && echo "-lpam";
  # AIX ends shared libraries with .a
  locate_library ${LIB_PATH}/usr/lib/libpam.a && echo "-lpam";
else
  locate_library /lib/libcrypt.so && echo "-lcrypt";
  locate_library /usr/lib/libcrypt.so && echo "-lcrypt";
fi

# Look for the dynamic linker library. Needed by older RedHat when
# you link in PAM
locate_library ${LIB_PATH}/lib/libdl.so && echo "-ldl";

# Look for libsocket. Solaris needs this.
locate_library ${LIB_PATH}/lib/libsocket.so && echo "-lsocket";

# Look for libnsl. Solaris needs this.
locate_library ${LIB_PATH}/lib/libnsl.so && echo "-lnsl";

# Look for libresolv. Solaris needs this.
locate_library ${LIB_PATH}/lib/libresolv.so && echo "-lresolv";

# Look for libutil. Older FreeBSD need this for setproctitle().
locate_library ${LIB_PATH}/usr/lib/libutil.so && echo "-lutil";

# For older HP-UX...
locate_library ${LIB_PATH}/usr/lib/libsec.sl && echo "-lsec";

# Look for libcap (capabilities)
locate_library ${LIB_PATH}/lib/libcap.so.1 && echo "${LIB_PATH}/lib/libcap.so.1";
locate_library ${LIB_PATH}/usr/lib/libcap.so && echo "-lcap";

# Solaris needs this for nanosleep()..
locate_library ${LIB_PATH}/lib/libposix4.so && echo "-lposix4";
locate_library ${LIB_PATH}/usr/lib/libposix4.so && echo "-lposix4";

# Tru64 (nanosleep)
locate_library ${LIB_PATH}/usr/shlib/librt.so && echo "-lrt";

# Solaris sendfile
locate_library ${LIB_PATH}/usr/lib/libsendfile.so && echo "-lsendfile";

# OpenSSL
if find_func SSL_library_init ssl.o; then
  echo "-lssl -lcrypto";
fi

exit 0;

