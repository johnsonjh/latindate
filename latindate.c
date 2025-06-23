#include <stdio.h>
#include <string.h>
#include <time.h>

struct RomanMap {
  int val;
  const char *sym;
};

static const struct RomanMap romanMap[] ={
  { 1000, "Ⅿ" }, { 900, "ⅭⅯ" }, { 500, "Ⅾ" }, { 400, "ⅭⅮ" },
  {  100, "Ⅽ" }, {  90, "ⅩⅭ" }, {  50, "Ⅼ" }, {  40, "ⅩⅬ" },
  {   10, "Ⅹ" }, {   9,  "Ⅸ" }, {   5, "Ⅴ" }, {   4,  "Ⅳ" },
  {    1, "Ⅰ" }
};

static void
toRoman (int num, char *buf)
{
  int i = 0;

  while (num > 0 && i < sizeof (romanMap) / sizeof (romanMap[0])) {
    while (num >= romanMap[i].val) {
      (void)strcat (buf, romanMap[i].sym);
      num -= romanMap[i].val;
    }
    ++i;
  }
}

static void
buildLatinDate (char *output)
{
  const char *months[] = {
    "ianuarias",  "februarias", "martias",   "aprilis",
    "maias",      "iunias",     "iulias",    "augustas",
    "septembres", "octobres",   "novembres", "decembres"
  };

  time_t t = time (NULL);
  struct tm *now = localtime (&t);

  int d = now->tm_mday;
  int currentMonth = now->tm_mon;
  int year = now->tm_year + 1900;

  int daysInMonth;

  switch (currentMonth)
  {
  case 0: case 2: case 4:
  case 6: case 7: case 9:
  case 11:
    daysInMonth = 31;
    break;

  case 3: case 5:
  case 8: case 10:
    daysInMonth = 30;
    break;

  case 1:
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
      daysInMonth = 29;
    else
      daysInMonth = 28;
    break;

  default:
    daysInMonth = 30;
  }

  int nones, ides;

  if (currentMonth == 2 || currentMonth == 4 ||
      currentMonth == 6 || currentMonth == 9) {
    nones = 7;
    ides = 15;
  } else {
    nones = 5;
    ides = 13;
  }

  char dayPart[128] = "";
  char romanCount[64] = "";

  const char *marker = NULL;
  const char *markerMonth = NULL;

  if (d == 1) {
    (void)strcpy (dayPart, "kalendae");
    markerMonth = months[currentMonth];
  } else if (d < nones) {
    int count = nones - d + 1;
    marker = "nonas";
    markerMonth = months[currentMonth];
    if (count == 2) {
      (void)strcpy (dayPart, "pridie");
    } else {
      toRoman (count, romanCount);
      (void)sprintf (dayPart, "ante·​diem·​%s", romanCount);
    }
  } else if (d == nones) {
    (void)strcpy (dayPart, "nonas");
    markerMonth = months[currentMonth];
  } else if (d < ides) {
    int count = ides - d + 1;
    marker = "idus";
    markerMonth = months[currentMonth];
    if (count == 2)
      (void)strcpy (dayPart, "pridie");
    else {
      toRoman (count, romanCount);
      (void)sprintf (dayPart, "ante·​diem·​%s", romanCount);
    }
  } else if (d == ides) {
    (void)strcpy (dayPart, "idus");
    markerMonth = months[currentMonth];
  } else {
    int count = daysInMonth - d + 2;
    marker = "kalendas";
    markerMonth = months[(currentMonth + 1) % 12];
    if (count == 2)
      (void)strcpy (dayPart, "pridie");
    else {
      toRoman (count, romanCount);
      (void)sprintf (dayPart, "ante·​diem·​%s", romanCount);
    }
  }

  char romanYear[64] = "";

  if (year < 1)
    year = 1;

  if (year > 3999)
    year = 3999;

  toRoman (year, romanYear);

  if (marker == NULL)
    (void)sprintf (output, "ultimum·​recognitum·​est·​%s·​"
                           "%s·​anno·​domini·​%s\n",
                   dayPart, markerMonth, romanYear);
  else
    (void)sprintf (output, "ultimum·​recognitum·​est·​%s·​"
                           "%s·​%s·​anno·​domini·​%s\n",
                   dayPart, marker, markerMonth, romanYear);
}

int
main (void)
{
  char inscription[512] = "";

  buildLatinDate (inscription);

  return fprintf (stdout, "%s", inscription);
}
