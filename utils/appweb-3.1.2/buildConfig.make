#   
#   buildConfig.make -- Build configuration file.
#   
#   WARNING: DO NOT EDIT. This file is generated by configure.
#   
#   Use ./configure --help for available options.
#   
################################################################################
#
#   Key Product Settigns
#
BLD_APPWEB_PRODUCT=1
BLD_PRODUCT=appweb
BLD_NAME=Embedthis Appweb
BLD_VERSION=3.1.2
BLD_NUMBER=1
BLD_MAJOR_VERSION=3
BLD_MINOR_VERSION=1
BLD_PATCH_VERSION=2
BLD_TYPE=RELEASE
BLD_TUNE=MPR_TUNE_SIZE
BLD_DEFAULTS=standard
BLD_COMPONENTS= ar cc make nm ranlib doxygen regexp mpr matrixssl openssl ssl sqlite ejs php appweb
BLD_EMBEDTHIS=0

#
#   Other Product Settings
#
BLD_COMPANY=Embedthis
BLD_DEBUG=0
BLD_DIRS=build projects obj lib modules bin src test all doc samples package releases
BLD_CLEAN_INSTALL=0
BLD_LICENSE=gpl
BLD_COMMERCIAL=0

#
#   Host and Build System Settings.
#
BLD_HOST_SYSTEM=arm-s3c6410-linux
BLD_BUILD_SYSTEM=i686-pc-linux
BLD_CROSS=1

#
#   Host System Settings 
#
BLD_HOST_OS=LINUX
BLD_HOST_CPU_ARCH=MPR_CPU_ARM
BLD_HOST_CPU=arm
BLD_HOST_CPU_UPPER=ARM
BLD_HOST_CPU_MODEL=
BLD_HOST_DIST=Unknown
BLD_HOST_DIST_VER=Unknown
BLD_HOST_UNIX=1
BLD_HOST_WIN=0

#
#   Build System Settings for Build Tools
#
BLD_BUILD_OS=LINUX
BLD_BUILD_CPU_ARCH=MPR_CPU_IX86
BLD_BUILD_CPU=i686
BLD_BUILD_CPU_UPPER=I686
BLD_BUILD_CPU_MODEL=i686
BLD_BUILD_UNIX=1
BLD_BUILD_WIN=0

#
#   System and Installation Directories
#
BLD_ROOT_PREFIX=/
BLD_PREFIX=/usr/appWeb
BLD_BIN_PREFIX=/usr/appWeb/local/bin
BLD_CFG_PREFIX=/etc/appweb
BLD_DOC_PREFIX=/usr/appWeb/share/doc/appweb
BLD_INC_PREFIX=/usr/appWeb/include/appweb
BLD_LIB_PREFIX=/usr/appWeb/lib/appweb
BLD_LOG_PREFIX=/var/log/appweb
BLD_MAN_PREFIX=/usr/appWeb/share/man
BLD_MOD_PREFIX=/usr/appWeb/lib/appweb/modules
BLD_SAM_PREFIX=/usr/appWeb/share/appweb/samples
BLD_SRC_PREFIX=/usr/appWeb/src/appweb-3.1.2
BLD_WEB_PREFIX=/var/web

#
#   Directories for native building
#
BLD_BIN_NAME=bin/arm-s3c6410-linux
BLD_LIB_NAME=lib/arm-s3c6410-linux
BLD_OBJ_NAME=obj/arm-s3c6410-linux
BLD_MOD_NAME=modules/arm-s3c6410-linux
BLD_INC_NAME=src/include

#
#   Standard Feature Selection
#
BLD_FEATURE_ASSERT=1
BLD_FEATURE_CMD=1
BLD_FEATURE_COMPLETE_NATIVE=1
BLD_FEATURE_COMPLETE_CROSS=0
BLD_FEATURE_DOC=1
BLD_FEATURE_DEVICE=
BLD_FEATURE_FLOATING_POINT=1
BLD_FEATURE_LEGACY_API=0
BLD_FEATURE_MULTITHREAD=0
BLD_FEATURE_NUM_TYPE=double
BLD_FEATURE_POLL=1
BLD_FEATURE_SAMPLES=0
BLD_FEATURE_TEST=0
BLD_FEATURE_VMALLOC=1
BLD_FEATURE_STATIC=1

#
#   Extended Feature Selection
#
BLD_FEATURE_ACCESS_LOG=1
BLD_FEATURE_ANGEL=1
BLD_FEATURE_AUTH_DIGEST=1
BLD_FEATURE_AUTH=1
BLD_FEATURE_AUTH_FILE=1
BLD_FEATURE_AUTH_PAM=0
BLD_FEATURE_CGI=1
BLD_FEATURE_CHUNK=1
BLD_FEATURE_DIR=1
BLD_FEATURE_EJS_ALL_IN_ONE=1
BLD_FEATURE_EJS_AUTO_COMPILE=1
BLD_FEATURE_EJS_CROSS_COMPILER=1
BLD_FEATURE_EJS_DB=1
BLD_FEATURE_EJS_DOC=0
BLD_FEATURE_EJS_E4X=1
BLD_FEATURE_EJS_LANG=EJS_LANG_FIXED
BLD_FEATURE_EJS_WEB=1
BLD_FEATURE_EGI=1
BLD_FEATURE_CONFIG=template/standard
BLD_FEATURE_CONFIG_PARSE=1
BLD_FEATURE_FILE=1
BLD_FEATURE_HTTP=1
BLD_FEATURE_HTTP_CLIENT=1
BLD_FEATURE_LOG=1
BLD_FEATURE_NET=1
BLD_FEATURE_NUM_TYPE_DOUBLE=1
BLD_FEATURE_RANGE=1
BLD_FEATURE_RUN_AS_SERVICE=1
BLD_FEATURE_SEND=1
BLD_FEATURE_UPLOAD=1
BLD_FEATURE_XML=1

BLD_HTTP_PORT=80
BLD_SSL_PORT=4443


#
#   File extensions 
#
BLD_BUILD_ARCH=.a
BLD_BUILD_EXE=
BLD_BUILD_OBJ=.o
BLD_BUILD_PIOBJ=.o
BLD_BUILD_CLASS=.class
BLD_BUILD_SHLIB=.so
BLD_BUILD_SHOBJ=.so
BLD_HOST_ARCH=.a
BLD_HOST_EXE=
BLD_HOST_OBJ=.o
BLD_HOST_PIOBJ=.o
BLD_HOST_CLASS=.class
BLD_HOST_SHLIB=.so
BLD_HOST_SHOBJ=.so
#
#   Select Native or cross development flags
#
BUILD_NATIVE ?= 1
BUILD_CROSS ?= 1

ifneq ($(BUILDING_CROSS),1)

	#
	#	Configuration for Native Compilation on the Build System
	#
	BUILD_NATIVE_OR_COMPLETE_CROSS=1
	BUILD_CROSS_OR_COMPLETE_NATIVE=1

	#
	#   O/S and CPU settings
	#
	LINUX=1
	BLD_OS=LINUX
	BLD_CPU_ARCH=MPR_CPU_IX86
	BLD_CPU=i686
	BLD_CPU_UPPER=I686
	BLD_CPU_MODEL=i686
	BLD_DIST=ubuntu
	BLD_DIST_VER=8.10
	BLD_UNIX_LIKE=1
	BLD_WIN_LIKE=0

	#
	#   Compiler and linker flags
	#
	BLD_CFLAGS=
	BLD_DFLAGS=
	BLD_IFLAGS=
	BLD_LDFLAGS=
	BLD_JFLAGS=
	BLD_CPPFLAGS=

	#
	#   File extensions
	#
	BLD_ARCH=.a
	BLD_EXE=
	BLD_CLASS=.class
	BLD_SHLIB=.so
	BLD_SHOBJ=.so
	BLD_LIB=.a
	BLD_OBJ=.o
	BLD_PIOBJ=.o

	#
	#   Output directories 
	#
	BLD_TOOLS_DIR=${BLD_TOP}/build/bin
	BLD_BIN_DIR=${BLD_TOP}/bin
	BLD_OBJ_DIR=${BLD_TOP}/obj
	BLD_MOD_DIR=${BLD_TOP}/modules
	BLD_LIB_DIR=${BLD_TOP}/lib
	BLD_INC_DIR=${BLD_TOP}/src/include
	BLD_ABS_BIN_DIR=/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/bin
	BLD_ABS_LIB_DIR=/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/lib
	BLD_ABS_OBJ_DIR=/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/obj
	BLD_ABS_MOD_DIR=/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/modules
	BLD_ABS_INC_DIR=/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/src/include

	#
	#   Native Compilation Features
	#
    BLD_FEATURE_ROMFS=0

	#
	#   Setup environment variables
	#
	export PATH:=/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/bin:/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/lib:/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/lib:/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/build/bin:/sbin:/usr/sbin:::$(PATH)

	#
	#   AR
	#
	BLD_FEATURE_AR=1
	BLD_AR=/usr/bin/ar
	BLD_AR_BUILTIN=0
	BLD_AR_WITH=1

	#
	#   CC
	#
	BLD_FEATURE_CC=1
	BLD_CC=/usr/bin/cc
	BLD_CC_BUILTIN=0
	BLD_CC_WITH=1
	BLD_CC_CYGWIN=0
	BLD_CC_DIAB=0
	BLD_CC_DYN_LOAD=1
	BLD_CC_MTUNE=1
	BLD_CC_MMU=1
	BLD_CC_STACK_PROTECTOR=1

	#
	#   MAKE
	#
	BLD_FEATURE_MAKE=1
	BLD_MAKE=/usr/bin/make
	BLD_MAKE_BUILTIN=0
	BLD_MAKE_WITH=1

	#
	#   NM
	#
	BLD_FEATURE_NM=1
	BLD_NM=/usr/bin/nm
	BLD_NM_BUILTIN=0
	BLD_NM_WITH=1

	#
	#   RANLIB
	#
	BLD_FEATURE_RANLIB=1
	BLD_RANLIB=/usr/bin/ranlib
	BLD_RANLIB_BUILTIN=0
	BLD_RANLIB_WITH=1

	#
	#   DOXYGEN
	#
	BLD_FEATURE_DOXYGEN=0

	#
	#   REGEXP
	#
	BLD_FEATURE_REGEXP=1
	BLD_REGEXP=src/regexp
	BLD_REGEXP_BUILTIN=1
	BLD_REGEXP_LIBS=pcre
	BLD_REGEXP_WITH=1

	#
	#   MPR
	#
	BLD_FEATURE_MPR=1
	BLD_MPR=src/mpr
	BLD_MPR_BUILTIN=1
	BLD_MPR_LIBS=mpr pcre
	BLD_MPR_OPTIONAL_DEPENDENCIES=regexp
	BLD_MPR_WITH=1

	#
	#   MATRIXSSL
	#
	BLD_FEATURE_MATRIXSSL=0

	#
	#   OPENSSL
	#
	BLD_FEATURE_OPENSSL=0

	#
	#   SSL
	#
	BLD_FEATURE_SSL=0

	#
	#   SQLITE
	#
	BLD_FEATURE_SQLITE=1
	BLD_SQLITE=src/db/sqlite
	BLD_SQLITE_BUILTIN=1
	BLD_SQLITE_DEPENDENCIES=mpr
	BLD_SQLITE_LIBS=sqlite3 mpr pcre
	BLD_SQLITE_WITH=1

	#
	#   EJS
	#
	BLD_FEATURE_EJS=1
	BLD_EJS=src/ejs
	BLD_EJS_BUILTIN=1
	BLD_EJS_DEPENDENCIES=mpr
	BLD_EJS_LIBS=ajs sqlite3 mpr pcre mpr pcre
	BLD_EJS_OPTIONAL_DEPENDENCIES=ssl sqlite
	BLD_EJS_WITH=1

	#
	#   PHP
	#
	BLD_FEATURE_PHP=0

	#
	#   APPWEB
	#
	BLD_FEATURE_APPWEB=1
	BLD_APPWEB=src/appweb
	BLD_APPWEB_BUILTIN=1
	BLD_APPWEB_DEPENDENCIES=mpr
	BLD_APPWEB_LIBS=appweb ejs ajs sqlite3 mpr pcre mpr pcre mpr pcre
	BLD_APPWEB_OPTIONAL_DEPENDENCIES=ssl ejs
	BLD_APPWEB_WITH=1



else

	#
	#	Configuration for Cross-Compilation for the Target
	#
	BUILD_NATIVE_OR_COMPLETE_CROSS=0
	BUILD_CROSS_OR_COMPLETE_NATIVE=1

	#
	#   O/S and CPU settings
	#
	LINUX=1
	BLD_OS=LINUX
	BLD_CPU_ARCH=MPR_CPU_ARM
	BLD_CPU=arm
	BLD_CPU_UPPER=ARM
	BLD_CPU_MODEL=
	BLD_DIST=Unknown
	BLD_DIST_VER=Unknown
	BLD_UNIX_LIKE=1
	BLD_WIN_LIKE=0

	#
	#   Compiler and linker flags
	#
	BLD_CFLAGS=
	BLD_DFLAGS=
	BLD_IFLAGS=
	BLD_LDFLAGS=
	BLD_JFLAGS=
	BLD_CPPFLAGS=

	#
	#   File extensions
	#
	BLD_ARCH=.a
	BLD_EXE=
	BLD_CLASS=.class
	BLD_SHLIB=.so
	BLD_SHOBJ=.so
	BLD_LIB=.a
	BLD_OBJ=.o
	BLD_PIOBJ=.o

	#
	#   Output directories 
	#
	BLD_TOOLS_DIR=${BLD_TOP}/build/bin
	BLD_BIN_DIR=${BLD_TOP}/bin/arm-s3c6410-linux
	BLD_OBJ_DIR=${BLD_TOP}/obj/arm-s3c6410-linux
	BLD_MOD_DIR=${BLD_TOP}/modules/arm-s3c6410-linux
	BLD_LIB_DIR=${BLD_TOP}/lib/arm-s3c6410-linux
	BLD_INC_DIR=${BLD_TOP}/src/include
	BLD_ABS_BIN_DIR=/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/bin/arm-s3c6410-linux
	BLD_ABS_LIB_DIR=/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/lib/arm-s3c6410-linux
	BLD_ABS_OBJ_DIR=/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/obj/arm-s3c6410-linux
	BLD_ABS_MOD_DIR=/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/modules/arm-s3c6410-linux
	BLD_ABS_INC_DIR=/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/src/include

	#
	#   Cross Compilation Features
	#
    BLD_FEATURE_ROMFS=0

	#
	#   Setup environment variables
	#
	export PATH:=/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/bin:/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/lib:/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/lib:/home/bhsong/develop/svn/ldd6410/utils/appweb-3.1.2/build/bin:/sbin:/usr/sbin:::$(PATH)

	#
	#   AR
	#
	BLD_FEATURE_AR=1
	BLD_AR=/usr/local/arm/4.2.2-eabi/usr/bin/arm-linux-ar
	BLD_AR_BUILTIN=0
	BLD_AR_WITH=1

	#
	#   CC
	#
	BLD_FEATURE_CC=1
	BLD_CC=/usr/local/arm/4.2.2-eabi/usr/bin/arm-linux-gcc
	BLD_CC_BUILTIN=0
	BLD_CC_WITH=1
	BLD_CC_CYGWIN=0
	BLD_CC_DIAB=0
	BLD_CC_DYN_LOAD=1
	BLD_CC_MTUNE=0
	BLD_CC_MMU=1
	BLD_CC_STACK_PROTECTOR=1

	#
	#   MAKE
	#
	BLD_FEATURE_MAKE=1
	BLD_MAKE=/usr/bin/make
	BLD_MAKE_BUILTIN=0
	BLD_MAKE_WITH=1

	#
	#   NM
	#
	BLD_FEATURE_NM=1
	BLD_NM=/usr/bin/nm
	BLD_NM_BUILTIN=0
	BLD_NM_WITH=1

	#
	#   RANLIB
	#
	BLD_FEATURE_RANLIB=1
	BLD_RANLIB=/usr/local/arm/4.2.2-eabi/usr/bin/arm-linux-ranlib
	BLD_RANLIB_BUILTIN=0
	BLD_RANLIB_WITH=1

	#
	#   DOXYGEN
	#
	BLD_FEATURE_DOXYGEN=0

	#
	#   REGEXP
	#
	BLD_FEATURE_REGEXP=1
	BLD_REGEXP=src/regexp
	BLD_REGEXP_BUILTIN=1
	BLD_REGEXP_LIBS=pcre
	BLD_REGEXP_WITH=1

	#
	#   MPR
	#
	BLD_FEATURE_MPR=1
	BLD_MPR=src/mpr
	BLD_MPR_BUILTIN=1
	BLD_MPR_LIBS=mpr pcre
	BLD_MPR_OPTIONAL_DEPENDENCIES=regexp
	BLD_MPR_WITH=1

	#
	#   MATRIXSSL
	#
	BLD_FEATURE_MATRIXSSL=0

	#
	#   OPENSSL
	#
	BLD_FEATURE_OPENSSL=0

	#
	#   SSL
	#
	BLD_FEATURE_SSL=0

	#
	#   SQLITE
	#
	BLD_FEATURE_SQLITE=1
	BLD_SQLITE=src/db/sqlite
	BLD_SQLITE_BUILTIN=1
	BLD_SQLITE_DEPENDENCIES=mpr
	BLD_SQLITE_LIBS=sqlite3 mpr pcre
	BLD_SQLITE_WITH=1

	#
	#   EJS
	#
	BLD_FEATURE_EJS=1
	BLD_EJS=src/ejs
	BLD_EJS_BUILTIN=1
	BLD_EJS_DEPENDENCIES=mpr
	BLD_EJS_LIBS=ajs sqlite3 mpr pcre mpr pcre
	BLD_EJS_OPTIONAL_DEPENDENCIES=ssl sqlite
	BLD_EJS_WITH=1

	#
	#   PHP
	#
	BLD_FEATURE_PHP=0

	#
	#   APPWEB
	#
	BLD_FEATURE_APPWEB=1
	BLD_APPWEB=src/appweb
	BLD_APPWEB_BUILTIN=1
	BLD_APPWEB_DEPENDENCIES=mpr
	BLD_APPWEB_LIBS=appweb ejs ajs sqlite3 mpr pcre mpr pcre mpr pcre
	BLD_APPWEB_OPTIONAL_DEPENDENCIES=ssl ejs
	BLD_APPWEB_WITH=1


endif

EXPORT_OBJECTS ?= 1
ifeq ($(EXPORT_OBJECTS),0)
  BLD_OBJ_DIR := .
endif

