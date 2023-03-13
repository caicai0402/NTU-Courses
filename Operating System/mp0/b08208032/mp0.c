#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

int occurence(char *string, char key){
  int len = strlen(string), count = 0;

  for(int i = 0; i < len; i++){
    if(string[i] == key)
      count ++;
  }

  return count;
}

void
traverse(char *path, int *dir_num, int *file_num, char key)
{
  int fd;
  struct stat st;
  
  if((fd = open(path, 0)) < 0){
    fprintf(2, "mp0: cannot open %s\n", path);
    close(fd);
    return ;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "mp0: cannot stat %s 1\n", path);
    close(fd);
    return ;
  }

  switch(st.type){
    case T_FILE:
      (*file_num) ++;
      printf("%s %d\n", path, occurence(path, key));
      break;

    case T_DIR:
      (*dir_num) ++;
      printf("%s %d\n", path, occurence(path, key));

      struct dirent de;
      char *p = path + strlen(path);
      *p++ = '/';

      while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
          continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(path, &st) < 0){
          printf("mp0: cannot stat %s 2\n", path);
          continue;
        }
        traverse(path, dir_num, file_num, key);
        }
        break;
  }

  close(fd);
  return ;
}

int
main(int argc, char *argv[])
{

  if(argc != 3){
    fprintf(2, "Usage: mp0 <root_directory> <key>\n");
    exit(1);
  }

  int pid, c2p[2], dir_num = -1, file_num = 0;;

  if(pipe(c2p) < 0){
    fprintf(2, "pipe failed\n");
    exit(1);
  }

  if((pid = fork()) == 0){
    close(c2p[0]);

    int fd;
    if((fd = open(argv[1], 0)) < 0)
      fprintf(2, "%s [error opening dir]\n", argv[1]);
    else{
      struct stat st;
      if(fstat(fd, &st) < 0)
        fprintf(2, "%s [error opening dir]\n", argv[1]);
      else{
        if(st.type == T_FILE)
          fprintf(2, "%s [error opening dir]\n", argv[1]);
        else{
          char buf[128] = {0};
          strcpy(buf, argv[1]);
          traverse(buf, &dir_num, &file_num, argv[2][0]);
        }
      }
    }
    close(fd);
    write(c2p[1], &dir_num, sizeof(int));
    write(c2p[1], &file_num, sizeof(int));
    close(c2p[1]);
    exit(0);
  }
  else{
    close(c2p[1]);    
    read(c2p[0], &dir_num, sizeof(int));
    read(c2p[0], &file_num, sizeof(int));
    close(c2p[0]);
    printf("\n%d directories, %d files\n", dir_num < 0? 0 : dir_num, file_num);
  }

  exit(0);
}
