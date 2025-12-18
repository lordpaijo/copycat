#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

char *usage = "Usage: copycat if=<files> of=<filename>.";

int main(int argc, char *argv[]) {
  char buf[4096];

  // 1. Parse arguments (if= and of=)
  const char **ifiles = malloc(argc * sizeof *ifiles);
  const char **ofiles = malloc(argc * sizeof *ofiles);
  int ifile_count = 0;
  int ofile_count = 0;
  
  for (int i = 1; i < argc; i++) {
    if (strncmp(argv[i], "if=", 3) == 0) 
      ifiles[ifile_count++] = argv[i] + 3;
    else if (strncmp(argv[i], "of=", 3) == 0)
      ofiles[ofile_count++] = argv[i] + 3;
    else {
      fprintf(stderr, "Unknown argument: %s\n%s\n", argv[i], usage);
      exit(1);
    }
  }
  
  if (ifile_count < 1 || ofile_count < 1) {
    fprintf(stderr, "Error: need at least one if= and one of=\n%s\n", usage);
    exit(1);
  }

  ssize_t bytes_read;
  int ofds[ofile_count];

  for (int i = 0; i < ofile_count; i++) {
    ofds[i] = open(ofiles[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (ofds[i] < 0) {
      perror(ofiles[i]);
      exit(1);
    }
  } 
 
  for (int i = 0; i < ifile_count; i++) {
    int ifd;
    ifd = open(ifiles[i], O_RDONLY);
    if (ifd < 0) {
      perror(ifiles[i]);
      break;
    }

    // 3. Copy loop (read → write)
    
    while ((bytes_read = read(ifd, buf, sizeof buf)) > 0) {
      for (int j = 0; j < ofile_count; j++) {
        write(ofds[j], buf, bytes_read);
      }
    }
    // 4.1 Cleanup ifd
    if (close(ifd) < 0) {
		  perror("close input");
		  exit(1);
	  }  
  }
  
  // 4.2 Cleanup ofd
  for (int i = 0; i < ofile_count; i++) {
    close(ofds[i]);
  }

  return 0;
}
