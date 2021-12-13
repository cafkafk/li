/* lc - light command: a brightness setter
 * Copyright (C) 2020 - 2021 Christina (cafkafk)
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

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  DIR *d;
  struct dirent *dir;

  if (argc == 2)
    if ((!(strcmp(argv[1], "--help"))) || (!(strcmp(argv[1], "help"))) ||
        (!(strcmp(argv[1], "-h")))) {
      printf("Usage: li <brighness>\n");
      exit(EXIT_SUCCESS);
    }
  if (argc != 2)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
                  "not enough arguments - try running help");
  else if (atoi(argv[1]) == 0 || atoi(argv[1]) > 100)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
                  "invalid brightness [1-100]");

  char *pre = "/sys/class/backlight/";
  char basepath[MAXPATHLENGTH] = {0};
  char maxpath[MAXPATHLENGTH] = {0};
  char brightpath[MAXPATHLENGTH] = {0};

  strcpy(basepath, pre);

  d = opendir(pre);

  if (d == NULL)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
                  "opendir(path) failed");

  if ((dir = readdir(d)) == NULL)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "readdir(d) failed");

  if ((dir = readdir(d)) == NULL)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "readdir(d) failed");

  if ((dir = readdir(d)) == NULL) {
    fprintf(stderr, "%s", "No compatible display found.\n");
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "readdir(d) failed");
  }

  if (closedir(d))
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
                  "closedir(d) failed");

  strncat(basepath, dir->d_name, MAXPATHLENGTH - strlen(pre) - 1);
  strncat(maxpath, basepath, MAXPATHLENGTH - 1);
  strncat(brightpath, basepath, MAXPATHLENGTH - 1);

  strcat(maxpath, "/max_brightness");
  strcat(brightpath, "/brightness");

  FILE *m_fp = fopen(maxpath, "r");
  if (m_fp == NULL)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
                  "fopen(maxpath, \"r\")");

  FILE *b_fp = fopen(brightpath, "w");
  if (b_fp == NULL)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
                  "fopen(brightpath, \"w\")");

  int m_b;

  if (0 >= fscanf(m_fp, "%d", &m_b))
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
                  "fscanf(m_fp...) failed");

  if (!fprintf(b_fp, "%d", atoi(argv[1]) * (m_b / 100)))
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
                  "fprintf(b_fp...) failed");

  if (fclose(m_fp)) {
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
                  "fclose(m_fp) failed");
  }

  if (fclose(b_fp)) {
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
                  "fclose(b_fp) failed");
  }

  return EXIT_SUCCESS;
}
