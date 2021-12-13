/* lc - light command: a brightness setter
 * Copyright (C) 2020 - 2021  Christina Sørensen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define MAXPATHLENGTH 128

#include <assert.h> // for assert
#include <errno.h>  // for errno, strerror
#include <stdio.h>  // for printf, etc...
#include <stdlib.h> // for EXIT_FAILURE, EXIT_SUCCESS, etc...
#include <string.h> // for strcat, strcpy
#include <error.h> // for strcat, strcpy
#include <dirent.h>

int main(int argc, char *argv[]) {
  // CHECK INPUT
  if (argc == 2)
    if ((!(strcmp(argv[1], "--help"))) || (!(strcmp(argv[1], "help"))) ||
        (!(strcmp(argv[1], "-h")))) {
      printf("Usage: li <brighness>\n");
      printf("Where brightness is a integer percentage of max(e.g. 1-100)\n");
      exit(EXIT_SUCCESS)
    }
  if (argc != 2)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "not enough arguments - try running help");
  else if (atoi(argv[1]) == 0 || atoi(argv[1]) > 100)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "invalid brightness [1-100]");

  // INITIALIZE PATH VARS
  char *pre = "/sys/class/backlight/";
  char basepath[MAXPATHLENGTH] = {0};
  char maxpath[MAXPATHLENGTH] = {0};
  char brightpath[MAXPATHLENGTH] = {0};
  strcpy(basepath, pre); // This is safe. PRE is of known length

  // VALIDATE USER INPUT LENGTH
  DIR *d;
  struct dirent *dir;
  d = opendir(pre);

  if(d == NULL)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "opendir(path) failed");

  // FIXME THIS IS UGLY
  if((dir=readdir(d)) == NULL)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "readdir(d) failed");

  if((dir=readdir(d)) == NULL)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "readdir(d) failed");

  if((dir=readdir(d)) == NULL)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "readdir(d) failed");

  closedir(d);

  printf("%s\n", dir->d_name);

  strncat(basepath, dir->d_name, MAXPATHLENGTH - strlen(pre) - 1);
  strncat(maxpath, basepath, MAXPATHLENGTH - 1);
  strncat(brightpath, basepath, MAXPATHLENGTH - 1);

  printf("%s\n", basepath);
  printf("%s\n", maxpath);
  printf("%s\n", brightpath);

  // SET PATHS FOR BACKLIGHT FILE I/O
  strcat(maxpath,
         "/max_brightness"); // Not really safe, but not exploitable either.
  strcat(brightpath, "/brightness"); // Will just crash if length exceeds

  // OPEN FILES FOR I/O
  FILE *m_fp = fopen(maxpath, "r");
  if (m_fp == NULL)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "fopen(maxpath, \"r\")");

  FILE *b_fp = fopen(brightpath, "w");
  if (b_fp == NULL)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "fopen(brightpath, \"w\")");

  // READ MAX BRIGHTNESS
  int m_b;

  if(0 >= fscanf(m_fp, "%d", &m_b))
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "fscanf(m_fp...) failed");

  // CALCULATE TARGET BRIGHTNESS AS % OF MAX
  if (!fprintf(b_fp, "%d", atoi(argv[2]) * (m_b / 100)))
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "fprintf(b_fp...) failed");

  // CLOSE FILES
  if(fclose(m_fp)) {
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "fclose(m_fp) failed");
  }

  if(fclose(b_fp)) {
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "fclose(b_fp) failed");
  }

  return EXIT_SUCCESS;
}
