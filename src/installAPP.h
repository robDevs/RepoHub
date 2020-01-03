#ifndef _INSTALLER_
#define _INSTALLER_

#include <psp2/promoterutil.h>
#include <psp2/sysmodule.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/io/stat.h>
#include <psp2/io/fcntl.h>
#include <psp2/ctrl.h>
#include <psp2/io/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "file.h"
#include <psp2/io/dirent.h>

#include "headgen.h"
#include "file.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <zip.h>

#include <stdlib.h>
#include <string.h>

#include <psp2/appmgr.h>

static int loadScePaf();
static int unloadScePaf();
extern char* concat(const char *s1, const char *s2);
static int extract(const char* path);
extern void installApp(const char* srcFolder , const char* name );
extern void removeFolder(const char* path);
extern int installVPK(const char* path);

#endif
