/*
 *  edep.c - Embedthis Dependency Program. Generate C, C++ and Java Dependencies
 *
 *  Copyright (c) All Rights Reserved. See details at the end of the file.
 */

/******************************* Documentation ********************************/
/*
 *  usage:  edep [-q] [-I includes] files...
 */
/********************************* Includes ***********************************/
#include    "posix.h"

/********************************* Defines ************************************/

#define MAX_INC     128                     /* Max include directories */
#define MAX_DEPEND  4096                    /* Max dependencies */

/********************************** Locals ************************************/

static char     *dependencies[MAX_DEPEND];
static int      finished;
static FILE     *fp;    
static char     *includeDir[MAX_INC];
static int      numDependencies;
static int      numIncludeDir;
static char     *program;
static int      quiet;

/**************************** Forward Declarations ****************************/

static char     *mprGetBaseName(char *name);
static int      mprGetDirName(char *buf, int bufsize, char *path);
static void     openSignals();
static int      findDependencies(FILE *fp, char *fname);
static int      depSort(const void *p1, const void *p2);
static char     *mapExtension(char *path);
static void     catchInterrupt(int signo);
static char     *mapDelimiters(char *s);

/************************************ Code ************************************/

int main(int argc, char *argv[])
{
    time_t      now;
    struct tm   *t;
    char        path[2048], cwd[2048], dirName[2048], parent[2048];
    char        *ext, *previous, *cp, *argp;
    int         errors, j, i, levels, upLevel, incJava, nextArg;

    errors = 0;
    program = mprGetBaseName(argv[0]);
    quiet = 0;

    for (nextArg = 1; nextArg < argc; nextArg++) {
        argp = argv[nextArg];
        if (*argp != '-') {
            break;
        }
        if (strncmp(argp, "-I", 2) == 0) {
            if (numIncludeDir >= MAX_INC) {
                fprintf(stderr, "Too many include directories\n");
                exit(1);
            }
            includeDir[numIncludeDir++] = strdup(&argp[2]);

        } else if (strcmp(argp, "-q") == 0) {
            quiet++;
        }
    }
    if (errors) {
        fprintf(stderr, "%s: usage: [-q] [-I includeDir] files...\n", program);
        exit(2);
    }

    includeDir[numIncludeDir++] = strdup(".");
#if !BLD_WIN_LIKE && !_WIN32
    includeDir[numIncludeDir++] = strdup("/usr/include");
#endif

    openSignals();
    if ((fp = fopen("make.newdep", "w")) == 0) {
        fprintf(stderr, "Cant open make.newdep\n");
        exit(255);
    }

    now = time(0);
    t = localtime(&now);
    fprintf(fp, "#\n#   .makedep -- Makefile dependencies. Generated by edep.\n#\n\n");

    getcwd(cwd, sizeof(cwd));
    mapDelimiters(cwd);

    fprintf(fp, "all: compile\n\n");

    fprintf(fp, "BLD_TOP := ");
    strcpy(dirName, cwd);

    for (i = 0, levels = 0; *dirName; i++) {
        sprintf(path, "%s/build/make/make.rules", dirName);
        if (access(path, R_OK) == 0) {
            break;
        }
        mprGetDirName(parent, sizeof(parent), dirName);
        strcpy(dirName, parent);
        if (i > 0) {
            fprintf(fp, "/..");
        } else {
            fprintf(fp, "..");
        }
        levels++;
    }
    if (i == 0) {
        fprintf(fp, ".");
    }
    fprintf(fp, "\n");

    /*
     *  Extract a src path for Java. Go up N levels less one for the "java" directory.
     */
    strcpy(dirName, cwd);
    cp = &dirName[strlen(dirName) - 1];
    for (upLevel = 1; upLevel < levels && cp > dirName; cp--) {
        if (*cp == '/') {
            upLevel++;
        }
    }

    if (levels > 1) {
        if (cp[0] != '\0' && cp[1] != '\0' && cp[2] != '\0') {
            cp += 2;
        }
        if (*cp == '\0') {
            cp = ".";
        }
        fprintf(fp, "SRC_PATH := %s\n\n", cp);

    } else {
        fprintf(fp, "SRC_PATH := .\n\n");
    }

    fprintf(fp, "#\n#   Read the build configuration settings and make variable definitions.\n#\n"); 
    fprintf(fp, "include $(BLD_TOP)/buildConfig.make\n\n");

    incJava = 0;
    for (i = nextArg; i < argc; i++) {
        if (strstr(argv[i], ".java") != 0 && strcmp(argv[i], "*.java") != 0) {
            incJava++;
            /* fprintf(fp, "include $(BLD_TOP)/build/make/make.java\n"); */
            break;
        }
    }
    if (incJava) {
        fprintf(fp, "HAS_JAVA = 1\n\n");
    }

    fprintf(fp, "SRC =");
    for (i = nextArg; i < argc; i++) {
        if (access(argv[i], R_OK) != 0) {
            continue;
        }
        strncpy(path, argv[i], sizeof(path));
        fprintf(fp, " \\\n\t%s", mprGetBaseName(path));
    }
    fprintf(fp, "\n\n");

    fprintf(fp, "PROCESSED_SRC =");
    for (i = nextArg; i < argc; i++) {
        if (access(argv[i], R_OK) != 0) {
            continue;
        }
        strncpy(path, argv[i], sizeof(path));
        ext = mapExtension(path);
        if (strcmp(ext, ".java") == 0) {
            fprintf(fp, " \\\n\t$(BLD_OBJ_DIR)/src/$(SRC_PATH)/%s", 
                mprGetBaseName(argv[i]));
        }
    }
    fprintf(fp, "\n\n");

    fprintf(fp, "OBJECTS =");
    for (i = nextArg; i < argc; i++) {
        if (access(argv[i], R_OK) != 0) {
            continue;
        }
        strncpy(path, argv[i], sizeof(path));
        ext = mapExtension(path);
        if (strcmp(ext, ".java") == 0) {
            fprintf(fp, " \\\n\t$(BLD_OBJ_DIR)/classes/$(SRC_PATH)/%s", 
                mprGetBaseName(path));
        } else {
            fprintf(fp, " \\\n\t$(BLD_OBJ_DIR)/%s", mprGetBaseName(path));
        }
    }
    fprintf(fp, "\n");

    for (i = nextArg; !finished && i < argc; i++) {
        if (*argv[i] == '*') {
            continue;
        }
        strcpy(path, argv[i]);
        ext = mapExtension(path);
        if (strcmp(ext, ".java") == 0) {
            fprintf(fp, "\n$(BLD_OBJ_DIR)/classes/$(SRC_PATH)/%s: ", 
                mprGetBaseName(path));
        } else {
            fprintf(fp, "\n$(BLD_OBJ_DIR)/%s: ", mprGetBaseName(path));
        }

        numDependencies = 0;
        findDependencies(fp, argv[i]);
        qsort(dependencies, numDependencies, sizeof(char*), depSort);

        previous = "";
        for (j = 0; j < numDependencies; j++) {
            if (strcmp(previous, dependencies[j]) != 0) {
                fprintf(fp, " \\\n\t%s", dependencies[j]);
            }
            previous = dependencies[j];
        }
        for (j = 0; j < numDependencies; j++) {
            free(dependencies[j]);
        }
        fprintf(fp, "\n");
    }

    fprintf(fp, "\n#\n# Read the Makefile rules\n#\n");
    fprintf(fp, "include $(BLD_TOP)/build/make/make.rules\n\n");

    fprintf(fp, "ifeq ($(BUILDING_CROSS),1)\n");
    fprintf(fp, "   include $(BLD_TOP)/build/make/make.$(BLD_HOST_OS)\n");
    fprintf(fp, "else\n");
    fprintf(fp, "   include $(BLD_TOP)/build/make/make.$(BLD_BUILD_OS)\n");
    fprintf(fp, "endif\n\n");

    fclose(fp);

    unlink("make.dep");
    unlink(".makedep");
    if (rename("make.newdep", ".makedep") != 0) {
        fprintf(stderr, "Cant rename make.newdep to .makedep\n");
        exit(255);
    }

    return 0;
}



/*
 *  Do the C and C++ dependencies
 */
static int findDependencies(FILE *fp, char *fname)
{
    FILE    *ifp;
    char    path[2048], buf[8192], dirName[2048];
    char    *cp, *ep;
    int     line, i, j;

    if ((ifp = fopen(fname, "r")) == 0) {
        if (!quiet) {
            fprintf(stderr, "Cant open %s\n", fname);
        }
        return -1;
    }

    for (line = 0; ! feof(ifp); line++) {
        if (fgets(buf, sizeof(buf), ifp) == 0)
            break;
        cp = buf;
        if (*cp++ != '#') {
            continue;
        }
        while (*cp == '\t' || *cp == ' ') {
            cp++;
        }
        if (*cp != 'i' || strncmp(cp, "include", 7)) {
            continue;
        }
        cp += 7;

        while (*cp == '\t' || *cp == ' ' || *cp == '\"') {
            cp++;
        }
        
        /*
         *  Skip system headers
         */
        if (*cp == '<') {
            continue;
        }

        ep = cp;
        while (isalnum((int) *ep) || *ep == '_' || *ep == '.' || *ep == '/' || 
                *ep == '-') {
            ep++;
        }
        *ep = '\0';

        strcpy(buf, cp);
        if (buf[0] == '/' || (buf[0] == '.' && buf[1] == '.')) {
            if (access(buf, R_OK) < 0) {
                if (!quiet) {
                    fprintf(stderr, "Cant find include %s\n", buf);
                }
                continue;
            }

        } else {
            /*
             *  First search relative to the including file
             */
            mprGetDirName(dirName, sizeof(dirName), fname);
            if (*dirName) {
                sprintf(path, "%s/%s", dirName, buf);
            } else {
                strcpy(path, dirName);
            }
            if (access(path, R_OK) < 0) {
                for (j = 0; j < numIncludeDir; j++) {
                    sprintf(path, "%s/%s", includeDir[j], buf);
                    if (access(path, R_OK) == 0) {
                        break;
                    }
                }
                if (j == numIncludeDir) {
                    if (!quiet) {
                        fprintf(stderr, "Cant find include %s in %s at %d\n", 
                            buf, fname, line);
                    }
                    continue;
                }
            }
        }

        if (numDependencies >= MAX_DEPEND) {
            fprintf(stderr, "Too many dependencies\n");
        } else {
            for (i = 0; i < numDependencies; i++) {
                if (strcmp(path, dependencies[i]) == 0)
                    break;
            }
            if (i == numDependencies) {
                dependencies[numDependencies++] = strdup(path);
                findDependencies(fp, path);
            }
        }
    }
    fclose(ifp);

    return 0;
}



static int depSort(const void *p1, const void *p2)
{
    char    *s1, *s2;

    s1 = *(char**) p1;
    s2 = *(char**) p2;

    return strcmp(s1, s2);
}



static char *mapDelimiters(char *s)
{
    char    *p;

    if (s == 0) {
        return 0;
    }
    for (p = s; *p; p++) {
        if (*p == '\\') {
            *p = '/';
        } else if (*p == ':') {
            s = &p[1];
        }
    }
    return s;
}



static char *mapExtension(char *path)
{
    static char ext[16];
    char        *cp;
    char        *object;

    object = "$(BLD_OBJ)";
    if ((cp = strrchr(path, '.'))) {
        strcpy(ext, cp);
        if (strcmp(cp, ".c") == 0) {
            strcpy(cp, object);
        } else if (strcmp(cp, ".cpp") == 0) {
            strcpy(cp, object);
        } else if (strcmp(cp, ".java") == 0) {
            strcpy(cp, ".class");
        } else if (strcmp(cp, ".cs") == 0) {
            strcpy(cp, "$(BLD_LIB)");
        }
    }
    return ext;
}



/*
 *  Return the last portion of a pathname
 */
static char *mprGetBaseName(char *name)
{
    char *cp;

    cp = strrchr(name, '/');

    if (cp == 0) {
        cp = strrchr(name, '\\');
        if (cp == 0) {
            return name;
        }
    } 
    if (cp == name) {
        if (cp[1] == '\0') {
            return name;
        }
    } else {
        if (cp[1] == '\0') {
            return "";
        }
    }
    return &cp[1];
}



/*
 *  Return the directory portion of a pathname into the users buffer.
 */
int mprGetDirName(char *buf, int bufsize, char *path)
{
    char    *cp;
    int     dlen;

    cp = strrchr(path, '/');
    if (cp == 0) {
#if BLD_WIN_LIKE
        cp = strrchr(path, '\\');
        if (cp == 0)
#endif
        {
            buf[0] = '\0';
            return 0;
        }
    }

    if (cp == path && cp[1] == '\0') {
        strcpy(buf, ".");
        return 0;
    }

    dlen = cp - path;
    if (dlen < bufsize) {
        if (dlen == 0) {
            dlen++;
        }
        memcpy(buf, path, dlen);
        buf[dlen] = '\0';
        return 0;
    }
    return -1;
}



/*
 *  Initialize signals
 */
static void openSignals() 
{
#if !BLD_WIN_LIKE && !_WIN32
    struct sigaction    act;

    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    act.sa_handler = catchInterrupt;
    sigaction(SIGINT, &act, 0);
#endif
}



static void catchInterrupt(int signo)
{
    finished++;
}

/*
 *  @copy   default
 *  
 *  Copyright (c) Embedthis Software LLC, 2003-2009. All Rights Reserved.
 *  Copyright (c) Michael O'Brien, 1993-2009. All Rights Reserved.
 *  
 *  This software is distributed under commercial and open source licenses.
 *  You may use the GPL open source license described below or you may acquire 
 *  a commercial license from Embedthis Software. You agree to be fully bound 
 *  by the terms of either license. Consult the LICENSE.TXT distributed with 
 *  this software for full details.
 *  
 *  This software is open source; you can redistribute it and/or modify it 
 *  under the terms of the GNU General Public License as published by the 
 *  Free Software Foundation; either version 2 of the License, or (at your 
 *  option) any later version. See the GNU General Public License for more 
 *  details at: http://www.embedthis.com/downloads/gplLicense.html
 *  
 *  This program is distributed WITHOUT ANY WARRANTY; without even the 
 *  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 *  
 *  This GPL license does NOT permit incorporating this software into 
 *  proprietary programs. If you are unable to comply with the GPL, you must
 *  acquire a commercial license to use this software. Commercial licenses 
 *  for this software and support services are available from Embedthis 
 *  Software at http://www.embedthis.com 
 *
 *  @end
 */
