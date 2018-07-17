/*
 * Copyright (c) 2018 bindh3x <os@bindh3x.io>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <stdio.h>
#include <getopt.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include "nicko.h"

static int flag;

#define NICKO_VERSION "1.1.0"

#define NICKO_FLAG_QUIET    (1 << 0)
#define NICKO_FLAG_TYPE     (1 << 1)
#define NICKO_FLAG_GROUP    (1 << 2)
#define NICKO_FLAG_FILENAME (1 << 3)

static const char *nicko_groups[] = {
  [NICKO_GROUP_SYSTEM]   = "system",
  [NICKO_GROUP_BIN]      = "binary",
  [NICKO_GROUP_ARCHIVE]  = "archive",
  [NICKO_GROUP_AUDIO]    = "audio",
  [NICKO_GROUP_DOCUMENT] = "document",
  [NICKO_GROUP_PM]       = "package manager file",
  [NICKO_GROUP_PICTURE]   = "picture",
  [NICKO_GROUP_VIDEO]    = "video",
  [NICKO_GROUP_UNSPECIFIED] = "unspecified"
};

static void error(const char *format, ...)
{
  va_list ap;

  fprintf(stderr, "error: ");
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);

  fputc('\n', stderr);
}

static void nicko_version(void)
{
  fprintf(stdout,
	  "nicko %s (compiled: %s %s)\n"
	  "libnicko %d.%d.%d\n",
	  NICKO_VERSION,
	 __DATE__, __TIME__,
	  LIBNICKO_VERSION_MAJOR,
	  LIBNICKO_VERSION_MINOR,
	  LIBNICKO_VERSION_PATCH);
}

static void nicko_usage(const char *filename)
{
  fprintf(stdout,
	  "Usage: %s [OPTIONS] FILE ...\n"
	  "\nOptions:\n"
	  "  -n  --name        Print file name as well.\n"
	  "  -t --type         Show only file type without file name.\n"
	  "  -q --quiet        Do not display output.\n"
	  "  -g --group        Print group instead of type.\n"

	  "\nGeneral Options:\n"
	  "  -v --version    Show version information and exit.\n"
	  "  -h --help       Show this message and exit.\n"
	  "\nCopyright (c) 2018 bindh3x <nicko@bindh3x.io>"
	  "\nSee <https://bindh3x.io/projects/nicko> for the full"
	  " documentation.\n", filename);
}

static int nicko_run(const char *filename)
{
  struct nicko_magic *m = NULL;

  if ((m = nicko(filename)) == NULL) {
    error("%s - %s", filename, strerror(errno));
    return 1;
  }

  if (m->name == NULL)
    return 1;

  if (flag & NICKO_FLAG_FILENAME)
    printf("%s: ", filename);

  if (flag & NICKO_FLAG_QUIET)
    goto end;
  else if (flag & NICKO_FLAG_TYPE)
    printf("%s\n", m->name);
  else if (flag & NICKO_FLAG_GROUP)
    printf("%s\n", nicko_groups[m->group]);

end:
  return m->type;
}

int main(int argc, char **argv)
{
  char **file = NULL;
  int ret = 0, opt = 0, opt_index = 0;
  static struct option long_options[] = {
    {"name",      no_argument, 0, 'n'},
    {"type",      no_argument, 0, 't'},
    {"quiet",     no_argument, 0, 'q'},
    {"group",     no_argument, 0, 'g'},
    {"version",   no_argument, 0, 'v'},
    {"help",      no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  if (argc < 2) {
    nicko_usage(argv[0]);
    return 1;
  }

  while ((opt = getopt_long(argc,
	  argv, "nghqtv", long_options, &opt_index)) != -1) {
    switch(opt) {
      case 't':
	flag |= NICKO_FLAG_TYPE;
	break;
      case 'q':
	flag |= NICKO_FLAG_QUIET;
	break;
      case 'g':
	flag |= NICKO_FLAG_GROUP;
	break;
      case 'n':
	flag |= NICKO_FLAG_FILENAME;
	break;
      case 'v':
	nicko_version();
	return 0;
      case 'h':
	nicko_usage(argv[0]);
	return 0;
    }
  }

  if (flag == NICKO_FLAG_FILENAME) {
    error("You must use -t/--type or -g/--group with -n/--name.");
    return 1;
  }
  else if (flag == 0)
    flag |= NICKO_FLAG_FILENAME | NICKO_FLAG_TYPE;

  file = argv;
  while (*++file)
    if (**file != 0x2d)
      ret = nicko_run(*file);

  return ret;
}
