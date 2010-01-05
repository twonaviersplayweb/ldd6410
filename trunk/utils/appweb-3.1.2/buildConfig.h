/*  
 *  buildConfig.h -- Build configuration file.
 *  
 *  WARNING: DO NOT EDIT. This file is generated by configure.
 *
 *  Use "./configure --help" for available options.
 */ 

    #define BLD_APPWEB_PRODUCT 1
    #define BLD_PRODUCT "appweb"
    #define BLD_NAME "Embedthis Appweb"
    #define BLD_VERSION "3.1.2"
    #define BLD_NUMBER "1"
    #define BLD_MAJOR_VERSION "3"
    #define BLD_MINOR_VERSION "1"
    #define BLD_PATCH_VERSION "2"
    #define BLD_TYPE "RELEASE"
    #define BLD_TUNE MPR_TUNE_SIZE
    #define BLD_DEFAULTS "standard"
    #define BLD_COMPONENTS " ar cc make nm ranlib doxygen regexp mpr matrixssl openssl ssl sqlite ejs php appweb"
    #define BLD_EMBEDTHIS 0

    #define BLD_COMPANY "Embedthis"
    #define BLD_DEBUG 0
    #define BLD_DIRS "build projects obj lib modules bin src test all doc samples package releases"
    #define BLD_CLEAN_INSTALL "0"
    #define BLD_LICENSE "gpl"
    #define BLD_COMMERCIAL "0"

    #define BLD_HOST_SYSTEM "arm-s3c6410-linux"
    #define BLD_BUILD_SYSTEM "i686-pc-linux"
    #define BLD_CROSS "1"

    #define BLD_HOST_OS "LINUX"
    #define BLD_HOST_CPU_ARCH MPR_CPU_ARM
    #define BLD_HOST_CPU "arm"
    #define BLD_HOST_CPU_UPPER "ARM"
    #define BLD_HOST_CPU_MODEL ""
    #define BLD_HOST_DIST "Unknown"
    #define BLD_HOST_DIST_VER "Unknown"
    #define BLD_HOST_UNIX 1
    #define BLD_HOST_WIN 0

    #define BLD_BUILD_OS "LINUX"
    #define BLD_BUILD_CPU_ARCH MPR_CPU_IX86
    #define BLD_BUILD_CPU "i686"
    #define BLD_BUILD_CPU_UPPER "I686"
    #define BLD_BUILD_CPU_MODEL "i686"
    #define BLD_BUILD_UNIX 1
    #define BLD_BUILD_WIN 0

    #define BLD_ROOT_PREFIX "/"
    #define BLD_PREFIX "/usr/appWeb"
    #define BLD_BIN_PREFIX "/usr/appWeb/local/bin"
    #define BLD_CFG_PREFIX "/etc/appweb"
    #define BLD_DOC_PREFIX "/usr/appWeb/share/doc/appweb"
    #define BLD_INC_PREFIX "/usr/appWeb/include/appweb"
    #define BLD_LIB_PREFIX "/usr/appWeb/lib/appweb"
    #define BLD_LOG_PREFIX "/var/log/appweb"
    #define BLD_MAN_PREFIX "/usr/appWeb/share/man"
    #define BLD_MOD_PREFIX "/usr/appWeb/lib/appweb/modules"
    #define BLD_SAM_PREFIX "/usr/appWeb/share/appweb/samples"
    #define BLD_SRC_PREFIX "/usr/appWeb/src/appweb-3.1.2"
    #define BLD_WEB_PREFIX "/var/web"

    #define BLD_BIN_NAME "bin/arm-s3c6410-linux"
    #define BLD_LIB_NAME "lib/arm-s3c6410-linux"
    #define BLD_OBJ_NAME "obj/arm-s3c6410-linux"
    #define BLD_MOD_NAME "modules/arm-s3c6410-linux"
    #define BLD_INC_NAME "src/include"

    #define BLD_FEATURE_ASSERT 1
    #define BLD_FEATURE_CMD 1
    #define BLD_FEATURE_COMPLETE_NATIVE 1
    #define BLD_FEATURE_COMPLETE_CROSS 0
    #define BLD_FEATURE_DOC 1
    #define BLD_FEATURE_DEVICE 
    #define BLD_FEATURE_FLOATING_POINT 1
    #define BLD_FEATURE_LEGACY_API 0
    #define BLD_FEATURE_MULTITHREAD 0
    #define BLD_FEATURE_NUM_TYPE double
    #define BLD_FEATURE_POLL 1
    #define BLD_FEATURE_SAMPLES 0
    #define BLD_FEATURE_TEST 0
    #define BLD_FEATURE_VMALLOC 1
    #define BLD_FEATURE_STATIC 1

    #define BLD_FEATURE_ACCESS_LOG 1
    #define BLD_FEATURE_ANGEL 1
    #define BLD_FEATURE_AUTH_DIGEST 1
    #define BLD_FEATURE_AUTH 1
    #define BLD_FEATURE_AUTH_FILE 1
    #define BLD_FEATURE_AUTH_PAM 0
    #define BLD_FEATURE_CGI 1
    #define BLD_FEATURE_CHUNK 1
    #define BLD_FEATURE_DIR 1
    #define BLD_FEATURE_EJS_ALL_IN_ONE 1
    #define BLD_FEATURE_EJS_AUTO_COMPILE 1
    #define BLD_FEATURE_EJS_CROSS_COMPILER 1
    #define BLD_FEATURE_EJS_DB 1
    #define BLD_FEATURE_EJS_DOC 0
    #define BLD_FEATURE_EJS_E4X 1
    #define BLD_FEATURE_EJS_LANG EJS_LANG_FIXED
    #define BLD_FEATURE_EJS_WEB 1
    #define BLD_FEATURE_EGI 1
    #define BLD_FEATURE_CONFIG template/standard
    #define BLD_FEATURE_CONFIG_PARSE 1
    #define BLD_FEATURE_FILE 1
    #define BLD_FEATURE_HTTP 1
    #define BLD_FEATURE_HTTP_CLIENT 1
    #define BLD_FEATURE_LOG 1
    #define BLD_FEATURE_NET 1
    #define BLD_FEATURE_NUM_TYPE_DOUBLE 1
    #define BLD_FEATURE_RANGE 1
    #define BLD_FEATURE_RUN_AS_SERVICE 1
    #define BLD_FEATURE_SEND 1
    #define BLD_FEATURE_UPLOAD 1
    #define BLD_FEATURE_XML 1

    #define BLD_HTTP_PORT 80
    #define BLD_SSL_PORT 4443


    #define BLD_BUILD_ARCH ".a"
    #define BLD_BUILD_EXE ""
    #define BLD_BUILD_OBJ ".o"
    #define BLD_BUILD_PIOBJ ".o"
    #define BLD_BUILD_CLASS ".class"
    #define BLD_BUILD_SHLIB ".so"
    #define BLD_BUILD_SHOBJ ".so"
    #define BLD_HOST_ARCH ".a"
    #define BLD_HOST_EXE ""
    #define BLD_HOST_OBJ ".o"
    #define BLD_HOST_PIOBJ ".o"
    #define BLD_HOST_CLASS ".class"
    #define BLD_HOST_SHLIB ".so"
    #define BLD_HOST_SHOBJ ".so"
#if !BUILDING_CROSS

    #define 	BUILD_NATIVE_OR_COMPLETE_CROSS 1
    #define 	BUILD_CROSS_OR_COMPLETE_NATIVE 1

    #define 	LINUX 1
    #define 	BLD_OS "LINUX"
    #define 	BLD_CPU_ARCH MPR_CPU_IX86
    #define 	BLD_CPU "i686"
    #define 	BLD_CPU_UPPER "I686"
    #define 	BLD_CPU_MODEL "i686"
    #define 	BLD_DIST "ubuntu"
    #define 	BLD_DIST_VER "8.10"
    #define 	BLD_UNIX_LIKE 1
    #define 	BLD_WIN_LIKE 0

    #define 	BLD_CFLAGS ""
    #define 	BLD_DFLAGS ""
    #define 	BLD_IFLAGS ""
    #define 	BLD_LDFLAGS ""
    #define 	BLD_JFLAGS ""
    #define 	BLD_CPPFLAGS ""

    #define 	BLD_ARCH ".a"
    #define 	BLD_EXE ""
    #define 	BLD_CLASS ".class"
    #define 	BLD_SHLIB ".so"
    #define 	BLD_SHOBJ ".so"
    #define 	BLD_LIB ".a"
    #define 	BLD_OBJ ".o"
    #define 	BLD_PIOBJ ".o"

    #define 	BLD_TOOLS_DIR "${BLD_TOP}/build/bin"
    #define 	BLD_BIN_DIR ${BLD_TOP}/bin
    #define 	BLD_OBJ_DIR ${BLD_TOP}/obj
    #define 	BLD_MOD_DIR ${BLD_TOP}/modules
    #define 	BLD_LIB_DIR ${BLD_TOP}/lib
    #define 	BLD_INC_DIR ${BLD_TOP}/src/include
    #define 	BLD_ABS_BIN_DIR "/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/bin"
    #define 	BLD_ABS_LIB_DIR "/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/lib"
    #define 	BLD_ABS_OBJ_DIR "/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/obj"
    #define 	BLD_ABS_MOD_DIR "/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/modules"
    #define 	BLD_ABS_INC_DIR "/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/src/include"

    #define     BLD_FEATURE_ROMFS 0

    #define 	BLD_FEATURE_AR 1
    #define 	BLD_AR "/usr/bin/ar"
    #define 	BLD_AR_BUILTIN 0
    #define 	BLD_AR_WITH "1"

    #define 	BLD_FEATURE_CC 1
    #define 	BLD_CC "/usr/bin/cc"
    #define 	BLD_CC_BUILTIN 0
    #define 	BLD_CC_WITH "1"
    #define 	BLD_CC_CYGWIN 0
    #define 	BLD_CC_DIAB 0
    #define 	BLD_CC_DYN_LOAD 1
    #define 	BLD_CC_MTUNE 1
    #define 	BLD_CC_MMU 1
    #define 	BLD_CC_STACK_PROTECTOR 1

    #define 	BLD_FEATURE_MAKE 1
    #define 	BLD_MAKE "/usr/bin/make"
    #define 	BLD_MAKE_BUILTIN 0
    #define 	BLD_MAKE_WITH "1"

    #define 	BLD_FEATURE_NM 1
    #define 	BLD_NM "/usr/bin/nm"
    #define 	BLD_NM_BUILTIN 0
    #define 	BLD_NM_WITH "1"

    #define 	BLD_FEATURE_RANLIB 1
    #define 	BLD_RANLIB "/usr/bin/ranlib"
    #define 	BLD_RANLIB_BUILTIN 0
    #define 	BLD_RANLIB_WITH "1"

    #define 	BLD_FEATURE_DOXYGEN 0

    #define 	BLD_FEATURE_REGEXP 1
    #define 	BLD_REGEXP "src/regexp"
    #define 	BLD_REGEXP_BUILTIN 1
    #define 	BLD_REGEXP_LIBS "pcre"
    #define 	BLD_REGEXP_WITH "1"

    #define 	BLD_FEATURE_MPR 1
    #define 	BLD_MPR "src/mpr"
    #define 	BLD_MPR_BUILTIN 1
    #define 	BLD_MPR_LIBS "mpr pcre"
    #define 	BLD_MPR_OPTIONAL_DEPENDENCIES "regexp"
    #define 	BLD_MPR_WITH "1"

    #define 	BLD_FEATURE_MATRIXSSL 0

    #define 	BLD_FEATURE_OPENSSL 0

    #define 	BLD_FEATURE_SSL 0

    #define 	BLD_FEATURE_SQLITE 1
    #define 	BLD_SQLITE "src/db/sqlite"
    #define 	BLD_SQLITE_BUILTIN 1
    #define 	BLD_SQLITE_DEPENDENCIES "mpr"
    #define 	BLD_SQLITE_LIBS "sqlite3 mpr pcre"
    #define 	BLD_SQLITE_WITH "1"

    #define 	BLD_FEATURE_EJS 1
    #define 	BLD_EJS "src/ejs"
    #define 	BLD_EJS_BUILTIN 1
    #define 	BLD_EJS_DEPENDENCIES "mpr"
    #define 	BLD_EJS_LIBS "ajs sqlite3 mpr pcre mpr pcre"
    #define 	BLD_EJS_OPTIONAL_DEPENDENCIES "ssl sqlite"
    #define 	BLD_EJS_WITH "1"

    #define 	BLD_FEATURE_PHP 0

    #define 	BLD_FEATURE_APPWEB 1
    #define 	BLD_APPWEB "src/appweb"
    #define 	BLD_APPWEB_BUILTIN 1
    #define 	BLD_APPWEB_DEPENDENCIES "mpr"
    #define 	BLD_APPWEB_LIBS "appweb ejs ajs sqlite3 mpr pcre mpr pcre mpr pcre"
    #define 	BLD_APPWEB_OPTIONAL_DEPENDENCIES "ssl ejs"
    #define 	BLD_APPWEB_WITH "1"



#else

    #define 	BUILD_NATIVE_OR_COMPLETE_CROSS 1
    #define 	BUILD_CROSS_OR_COMPLETE_NATIVE 1

    #define 	LINUX 1
    #define 	BLD_OS "LINUX"
    #define 	BLD_CPU_ARCH MPR_CPU_ARM
    #define 	BLD_CPU "arm"
    #define 	BLD_CPU_UPPER "ARM"
    #define 	BLD_CPU_MODEL ""
    #define 	BLD_DIST "Unknown"
    #define 	BLD_DIST_VER "Unknown"
    #define 	BLD_UNIX_LIKE 1
    #define 	BLD_WIN_LIKE 0

    #define 	BLD_CFLAGS ""
    #define 	BLD_DFLAGS ""
    #define 	BLD_IFLAGS ""
    #define 	BLD_LDFLAGS ""
    #define 	BLD_JFLAGS ""
    #define 	BLD_CPPFLAGS ""

    #define 	BLD_ARCH ".a"
    #define 	BLD_EXE ""
    #define 	BLD_CLASS ".class"
    #define 	BLD_SHLIB ".so"
    #define 	BLD_SHOBJ ".so"
    #define 	BLD_LIB ".a"
    #define 	BLD_OBJ ".o"
    #define 	BLD_PIOBJ ".o"

    #define 	BLD_TOOLS_DIR "${BLD_TOP}/build/bin"
    #define 	BLD_BIN_DIR ${BLD_TOP}/bin/arm-s3c6410-linux
    #define 	BLD_OBJ_DIR ${BLD_TOP}/obj/arm-s3c6410-linux
    #define 	BLD_MOD_DIR ${BLD_TOP}/modules/arm-s3c6410-linux
    #define 	BLD_LIB_DIR ${BLD_TOP}/lib/arm-s3c6410-linux
    #define 	BLD_INC_DIR ${BLD_TOP}/src/include
    #define 	BLD_ABS_BIN_DIR "/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/bin/arm-s3c6410-linux"
    #define 	BLD_ABS_LIB_DIR "/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/lib/arm-s3c6410-linux"
    #define 	BLD_ABS_OBJ_DIR "/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/obj/arm-s3c6410-linux"
    #define 	BLD_ABS_MOD_DIR "/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/modules/arm-s3c6410-linux"
    #define 	BLD_ABS_INC_DIR "/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/src/include"

    #define     BLD_FEATURE_ROMFS 0

    #define 	BLD_FEATURE_AR 1
    #define 	BLD_AR "/usr/local/arm/4.2.2-eabi/usr/bin/arm-linux-ar"
    #define 	BLD_AR_BUILTIN 0
    #define 	BLD_AR_WITH "1"

    #define 	BLD_FEATURE_CC 1
    #define 	BLD_CC "/usr/local/arm/4.2.2-eabi/usr/bin/arm-linux-gcc"
    #define 	BLD_CC_BUILTIN 0
    #define 	BLD_CC_WITH "1"
    #define 	BLD_CC_CYGWIN 0
    #define 	BLD_CC_DIAB 0
    #define 	BLD_CC_DYN_LOAD 1
    #define 	BLD_CC_MTUNE 0
    #define 	BLD_CC_MMU 1
    #define 	BLD_CC_STACK_PROTECTOR 1

    #define 	BLD_FEATURE_MAKE 1
    #define 	BLD_MAKE "/usr/bin/make"
    #define 	BLD_MAKE_BUILTIN 0
    #define 	BLD_MAKE_WITH "1"

    #define 	BLD_FEATURE_NM 1
    #define 	BLD_NM "/usr/bin/nm"
    #define 	BLD_NM_BUILTIN 0
    #define 	BLD_NM_WITH "1"

    #define 	BLD_FEATURE_RANLIB 1
    #define 	BLD_RANLIB "/usr/local/arm/4.2.2-eabi/usr/bin/arm-linux-ranlib"
    #define 	BLD_RANLIB_BUILTIN 0
    #define 	BLD_RANLIB_WITH "1"

    #define 	BLD_FEATURE_DOXYGEN 0

    #define 	BLD_FEATURE_REGEXP 1
    #define 	BLD_REGEXP "src/regexp"
    #define 	BLD_REGEXP_BUILTIN 1
    #define 	BLD_REGEXP_LIBS "pcre"
    #define 	BLD_REGEXP_WITH "1"

    #define 	BLD_FEATURE_MPR 1
    #define 	BLD_MPR "src/mpr"
    #define 	BLD_MPR_BUILTIN 1
    #define 	BLD_MPR_LIBS "mpr pcre"
    #define 	BLD_MPR_OPTIONAL_DEPENDENCIES "regexp"
    #define 	BLD_MPR_WITH "1"

    #define 	BLD_FEATURE_MATRIXSSL 0

    #define 	BLD_FEATURE_OPENSSL 0

    #define 	BLD_FEATURE_SSL 0

    #define 	BLD_FEATURE_SQLITE 1
    #define 	BLD_SQLITE "src/db/sqlite"
    #define 	BLD_SQLITE_BUILTIN 1
    #define 	BLD_SQLITE_DEPENDENCIES "mpr"
    #define 	BLD_SQLITE_LIBS "sqlite3 mpr pcre"
    #define 	BLD_SQLITE_WITH "1"

    #define 	BLD_FEATURE_EJS 1
    #define 	BLD_EJS "src/ejs"
    #define 	BLD_EJS_BUILTIN 1
    #define 	BLD_EJS_DEPENDENCIES "mpr"
    #define 	BLD_EJS_LIBS "ajs sqlite3 mpr pcre mpr pcre"
    #define 	BLD_EJS_OPTIONAL_DEPENDENCIES "ssl sqlite"
    #define 	BLD_EJS_WITH "1"

    #define 	BLD_FEATURE_PHP 0

    #define 	BLD_FEATURE_APPWEB 1
    #define 	BLD_APPWEB "src/appweb"
    #define 	BLD_APPWEB_BUILTIN 1
    #define 	BLD_APPWEB_DEPENDENCIES "mpr"
    #define 	BLD_APPWEB_LIBS "appweb ejs ajs sqlite3 mpr pcre mpr pcre mpr pcre"
    #define 	BLD_APPWEB_OPTIONAL_DEPENDENCIES "ssl ejs"
    #define 	BLD_APPWEB_WITH "1"


#endif
