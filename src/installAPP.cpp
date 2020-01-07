#include "installAPP.h"
#include "draw.h"
#include "includes.h"

static int loadScePaf() {
  static uint32_t argp[] = { 0x180000, -1, -1, 1, -1, -1 };

  int result = -1;

  uint32_t buf[4];
  buf[0] = sizeof(buf);
  buf[1] = (uint32_t)&result;
  buf[2] = -1;
  buf[3] = -1;

  return sceSysmoduleLoadModuleInternalWithArg(SCE_SYSMODULE_INTERNAL_PAF, sizeof(argp), argp, buf);
}

static int unloadScePaf() {
  uint32_t buf = 0;
  return sceSysmoduleUnloadModuleInternalWithArg(SCE_SYSMODULE_INTERNAL_PAF, 0, NULL, &buf);
}


char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

static int extract(const char* path) {
    const char *prg;
    const char *archive;
    struct zip *za;
    struct zip_file *zf;
    struct zip_stat sb;
    char buf[2000];
    int err;
    int i, len;
    int fd;
    long long sum;
    prg = path;

    archive = path;
    if ((za = zip_open(archive, 0, &err)) == NULL) {
        zip_error_to_str(buf, sizeof(buf), err, errno);
        draw_alert_message_time("error with zip", 1);
        return 1;
    }

    int sizeDone = 0;
    int sizeTotal = 0;

    for (i = 0; i < zip_get_num_entries(za, 0); i++) {
        if (zip_stat_index(za, i, 0, &sb) == 0) {
            sizeTotal += sb.size;
        }

    }

    for (i = 0; i < zip_get_num_entries(za, 0); i++) {
        if (zip_stat_index(za, i, 0, &sb) == 0) {
            //printf("==================\n");
            len = strlen(sb.name);
            //index info. E
            //printf("Name: [%s], ", sb.name);
            //printf("Size: [%llu], ", sb.size);
            //printf("mtime: [%u]\n", (unsigned int)sb.mtime);
            //printf("[%c], ", sb.name[len - 1]);
            //printf("\n");

            for(int x = 0; x < len; x++) {

                if(sb.name[x] == '/') {
                    char newFolder[x];;

                    memcpy(newFolder, sb.name, x);
                    newFolder[x] = '\0';

                    SceUID temp = sceIoDopen(concat("ux0:data/repoHubExtract/", newFolder));
                    if(temp < 0) {
                        sceIoDclose(temp);
                        sceIoMkdir(concat("ux0:data/repoHubExtract/", newFolder), 0777);
                    }
                    else sceIoDclose(temp);
                }
            }

            zf = zip_fopen_index(za, i, 0);
                if (!zf) {
                    //unable to open archive index, do something.
                    draw_alert_message_time("error with index", 1);
                }

                fd = sceIoOpen(concat("ux0:data/repoHubExtract/", sb.name), SCE_O_RDWR  | SCE_O_TRUNC  | SCE_O_CREAT, 0777);
                if (fd < 0) {
                    //ubable to open, do something.
                    draw_alert_message_time("error with new file", 1);
                }

                sum = 0;
                int skip = 0;
                while (sum != sb.size) {
                    len = zip_fread(zf, buf, 2000);
                    if (len < 0) {
                        //error, do something.
                        draw_alert_message_time("error writing maybe", 1);
                    }
                    sceIoWrite(fd, buf, len);
                    sum += len;


                    skip += 1;
                    if(skip > 20) skip = 0;

                    if(skip == 0) {
                        std::string nameString;
                        std::string fileString ;
                        std::string totalString;

                        nameString = "extracting: ";
                        nameString += sb.name;

                        fileString += "File progres: ";
                        fileString += std::to_string(byte_to_mb(sum));
                        fileString += " MB / ";
                        fileString += std::to_string(byte_to_mb(sb.size));
                        fileString += " MB";

                        totalString = "Total progress: ";
                        totalString += std::to_string(byte_to_mb(sizeDone));
                        totalString += " MB / ";
                        totalString += std::to_string(byte_to_mb(sizeTotal));
                        totalString += " MB";

                        draw_extract_progress(path, sb.name, sum, sb.size, sizeDone, sizeTotal);
                    }
                }
                sceIoClose(fd);
                zip_fclose(zf);


                sizeDone += sb.size;
        } else { }// unable to read archive file. . do something.
    }

    if (zip_close(za) == -1) {
        //error closing, do something.
        return 1;
    }

    return 0;
}

// Name is used for temporary folder so don't put any weird strings in there !
void installApp(const char* srcFolder , const char* name){
  //check permisions.
  char ebootPath[1024];
  snprintf(ebootPath, 1024, "%s/eboot.bin", srcFolder);
  SceUID fd = sceIoOpen(ebootPath, SCE_O_RDONLY, 0);

  bool check = true;

  if (fd >= 0) {
      char buffer[0x88];
      sceIoRead(fd, buffer, sizeof(buffer));
      sceIoClose(fd);

      // check authid and show warning.
      uint64_t authid = *(uint64_t *)(buffer + 0x80);
      if (authid != 0x2F00000000000002) {

          check = draw_yes_no_message("This package requests extended permissions.\nplease proceed at your own caution.\nWould you like to continue the install?");
      }//ask permision;
  }

  if(check) {
      draw_alert_message_time("installing", 1);
      // Start promoter stuff
      loadScePaf();
      sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_PROMOTER_UTIL);
      scePromoterUtilityInit();

      const char *copyFolderRoot = "ux0:data/repoHubInstall";

      char tmpFolder[255];
      snprintf(tmpFolder , 255 , "%s%s%s" , copyFolderRoot , hasEndSlash(copyFolderRoot) ? "" : "/", name);
      int copyResult = copyPath(srcFolder , tmpFolder);
      if(copyResult < 0){
        sceIoRmdir(tmpFolder);
        // End promoter stuff
        scePromoterUtilityExit();
        sceSysmoduleUnloadModuleInternal(SCE_SYSMODULE_INTERNAL_PROMOTER_UTIL);
        unloadScePaf();
        return;
      }

      generateHeadBin(tmpFolder);

      if (scePromoterUtilityPromotePkgWithRif(tmpFolder, 1) == 0)
      {
        //success, do something.
        draw_alert_message("install success.");
      }
      else
      {
        //failure, see above comment.
        draw_alert_message_time("install fail", 1);
      }

      // End promoter stuff
      scePromoterUtilityExit();
      sceSysmoduleUnloadModuleInternal(SCE_SYSMODULE_INTERNAL_PROMOTER_UTIL);
      unloadScePaf();
  }
}

void removeFolder(const char* path) {
    SceUID  dir;
    dir = sceIoDopen(path);
    if(dir >= 0) {
    	struct SceIoDirent entry;
        while(sceIoDread(dir, &entry) > 0) {
            SceUID temp;
            temp = sceIoDopen(concat(concat(path, "/"), entry.d_name));
            if(temp >= 0) {
                sceIoDclose(temp);
                removeFolder(concat(concat(path, "/"), entry.d_name));
            }
            else {
                sceIoDclose(temp);
                sceIoRemove(concat(concat(path, "/"), entry.d_name));
            }
        }
    }

    sceIoDclose(dir);
    if(sceIoRmdir(path) == 0) printf("removed %s\n", path);
}

int installVPK(const char* path) {
    //make sure direcotries are empty first. 
    removeFolder("ux0:data/repoHubExtract");
    removeFolder("ux0:data/repoHubInstall");
    
    sceIoMkdir( "ux0:data/repoHubExtract/" , 0777);
    sceIoMkdir( "ux0:data/repoHubInstall/" , 0777);

    draw_alert_message_time("extracting", 1);
    printf("\nExtracting zork\n");
    extract(path);

    draw_alert_message_time("installing", 1);
    printf(" Installing zork...\n");
    installApp("ux0:data/repoHubExtract/" , "zork");
    sceKernelDelayThread(1000*1000);

    removeFolder("ux0:data/repoHubExtract");
    removeFolder("ux0:data/repoHubInstall");
}
