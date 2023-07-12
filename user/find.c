#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"

char*                   //this function is same as the function in xv6.user/ls.c
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void 
find(char *path,char *filename)                       //this function is same as the function in xv6.user/ls.c,modified from ls()
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    if(!strcmp(fmtname(path),filename))               //Here are some changes: if the filename is same as the name it wants to find,just output it
    {
        fprintf(1, "%s %d %d %d\n", buf, st.type, st.ino, st.size);
    }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      fprintf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        fprintf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      if (!strcmp(de.name,filename))
      {
        fprintf(1, "%s %d %d %d\n", buf, st.type, st.ino, st.size);
      }
      if(st.type == T_DIR && strcmp(de.name,".")!=0 && strcmp(de.name,"..")!=0)
      {
        find(buf,filename);
      }

    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc!=3)
  {
    write(2,"parameters don't match\n",strlen("parameters don't match\n"));
    exit(1);
  }
  find(argv[1],argv[2]);
  exit(0);
}