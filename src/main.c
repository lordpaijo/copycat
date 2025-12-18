#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define VPRINT(...) \
  do { if (verbose) fprintf(stderr, "[V] " __VA_ARGS__); } while (0)

void print_help(const char *prgonm);

int main(int argc, char *argv[]) {
  char buf[4096];

  // 1. Parse arguments (if= and of=)
  const char **ifiles = malloc(argc * sizeof *ifiles);
  const char **ofiles = malloc(argc * sizeof *ofiles);
  int ifile_count = 0;
  int ofile_count = 0;
  int verbose = 0;
  
  for (int i = 1; i < argc; i++) {
    if (strncmp(argv[i], "if=", 3) == 0) 
      ifiles[ifile_count++] = argv[i] + 3;
    else if (strncmp(argv[i], "of=", 3) == 0)
      ofiles[ofile_count++] = argv[i] + 3; 
    else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0)
      verbose = 1;
    else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      print_help("copycat");
      exit(0);
    }
    else {
      print_help("copycat");
      exit(1);
    }
  }
  
  if (ifile_count < 1 || ofile_count < 1) {
    print_help("copycat");
    exit(1);
  }

  ssize_t bytes_read;
  int ofds[ofile_count];

  for (int i = 0; i < ofile_count; i++) {
    VPRINT("Opening output file: %s\n", ofiles[i]);
    ofds[i] = open(ofiles[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (ofds[i] < 0) {
      perror(ofiles[i]);
      exit(1);
    }
  } 
 
  for (int i = 0; i < ifile_count; i++) {
    int ifd;
    VPRINT("Opening input file: %s\n", ifiles[i]);
    ifd = open(ifiles[i], O_RDONLY);
    if (ifd < 0) {
      perror(ifiles[i]);
      break;
    }

    // 3. Copy loop (read → write)
    while ((bytes_read = read(ifd, buf, sizeof buf)) > 0) { 
      size_t total_bytes = 0;
      for (int j = 0; j < ofile_count; j++) {    
        total_bytes += bytes_read;
        ssize_t n = write(ofds[j], buf, bytes_read);
        if (n != bytes_read) {
          perror("write");
          exit(1);
        }
        VPRINT("Copied %zu bytes from %s\n", total_bytes, ifiles[i]);
        total_bytes = 0;
      }
    }

    // 4.1 Cleanup ifd
    VPRINT("Finished copying. Closing input files.\n");
    if (close(ifd) < 0) {
		  perror("close input");
		  exit(1);
	  }  
  }
  
  // 4.2 Cleanup ofd (close write descriptors first)
  for (int i = 0; i < ofile_count; i++) {
    VPRINT("Finished copying. Closing output files.\n");
    if (close(ofds[i]) < 0) {
      perror("close output");
      exit(1);
    }
  }

  // 5. Print output file contents to stdout
  for (int i = 0; i < ofile_count; i++) {
    VPRINT("Printing contents of %s to stdout\n", ofiles[i]);
    int fd = open(ofiles[i], O_RDONLY);
    if (fd < 0) {
      perror(ofiles[i]);
      continue;
    }

    printf("%s:\n", ofiles[i]);
    while ((bytes_read = read(fd, buf, sizeof buf)) > 0) {
      ssize_t written = write(STDOUT_FILENO, buf, bytes_read);
      if (written < 0) {
        perror("write stdout");
        close(fd);
        exit(1);
      }
    }

    if (bytes_read < 0) {
      perror("read output");
    }

    close(fd);
  }

  free(ifiles);
  free(ofiles);
  return 0;
}

void print_help(const char *prognm) {
  printf(
    "Usage: %s if=<files> of=<filename>.\n\n"
    "A rewriting of cat input-file > output-file shell command.\n\n"
    "Options:\n"
    "  if  (input file(s))    Give the program file source(s).\n"
    "  of  (output file(s))   Give the program output destination(s).\n"
    "  -h, --help             Display this help message.\n"
    "  -v, --verbose          Show detailed progress information.\n",
    prognm
  );
}

