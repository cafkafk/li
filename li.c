/* li - backLIght changer: a brightness setter
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

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int main(int argc, char *argv[]) {
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

  DIR *backlight_dir;

  struct dirent *backlight_dirent;

  FILE *max_brightness_file;
  FILE *brightness_file;

  char *prefix = "/sys/class/backlight/";
  char basepath[PATH_MAX] = {0};
  char maxpath[PATH_MAX] = {0};
  char brightpath[PATH_MAX] = {0};

  int max_brightness;

  long long buffer_full_indicator = 0;

  buffer_full_indicator = snprintf(basepath, PATH_MAX - 1, "%s", prefix);
  if (0 > buffer_full_indicator || buffer_full_indicator > PATH_MAX - 1 )
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "snprintf() failed: %s", strerror(errno));

  backlight_dir = opendir(prefix);

  if (backlight_dir == NULL)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "opendir(path) failed");

  // We first read in . and ..
  for (int i = 0; i < 3; i++) {
    errno = 0; // sentinel
    if (((backlight_dirent = readdir(backlight_dir)) == NULL) || errno) {
      if(i==2) // not failure on . or ..
        fprintf(stderr, "%s", "No compatible display found.\n");
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "readdir(backlight_dir) failed: %s", strerror(errno));
    }
  }

  if (closedir(backlight_dir))
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "closedir(backlight_dir) failed");

  buffer_full_indicator = snprintf(basepath, PATH_MAX - strlen(prefix) - 1, "%s", backlight_dirent->d_name);
  if (0 > buffer_full_indicator || buffer_full_indicator > PATH_MAX - strlen(prefix) - 1)
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "snprintf() failed: %s", strerror(errno));

  buffer_full_indicator = snprintf(maxpath, PATH_MAX - 1, "%s", basepath);
  if (0 > buffer_full_indicator || buffer_full_indicator > PATH_MAX - 1 )
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "snprintf() failed: %s", strerror(errno));

  buffer_full_indicator = snprintf(brightpath, PATH_MAX - 1, "%s", basepath);
  if (0 > buffer_full_indicator || buffer_full_indicator > PATH_MAX - 1 )
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "snprintf() failed: %s", strerror(errno));

  buffer_full_indicator = snprintf(maxpath, PATH_MAX - 1, "%s", "/max_brightness");
  if (0 > buffer_full_indicator || buffer_full_indicator > PATH_MAX - 1 )
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "snprintf() failed: %s", strerror(errno));

  buffer_full_indicator = snprintf(brightpath, PATH_MAX - 1, "%s", "/brightness");
  if (0 > buffer_full_indicator || buffer_full_indicator > PATH_MAX - 1 )
      error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "snprintf() failed: %s", strerror(errno));

  max_brightness_file = fopen(maxpath, "r");

  if (max_brightness_file == NULL)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
                  "fopen(maxpath, \"r\")");

  brightness_file = fopen(brightpath, "w");

  if (brightness_file == NULL)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
                  "fopen(brightpath, \"w\")");


  if (0 >= fscanf(max_brightness_file, "%d", &max_brightness))
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
                  "fscanf(max_brightness_file...) failed");

  if (!fprintf(brightness_file, "%d", atoi(argv[1]) * (max_brightness / 100)))
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
                  "fprintf(brightness_file...) failed");

  if (fclose(max_brightness_file)) {
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
                  "fclose(max_brightness_file) failed");
  }

  if (fclose(brightness_file)) {
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
                  "fclose(brightness_file) failed");
  }

  return EXIT_SUCCESS;
}
