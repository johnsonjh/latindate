/*****************************************
 * latindate.c                           *
 * Copyright (c) 2025 Jeffrey H. Johnson *
 * SPDX-License-Identifier: MIT-0        *
 * vim: set expandtab cc=80 :            *
 *****************************************/

/***************************************************************************
 * This file contains UTF-8 ligatures, interpuncts, and zero-width spaces! *
 ***************************************************************************/

#include <ctype.h>
#include <langinfo.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

/******************************************************************************/

#define MaxLatinLength    128
#define MaxDayPartLength   32
#define MaxRomanYearLength 16
#define MaxRomanCountLength 8

/******************************************************************************/

struct RomanMap
{
  int val;
  const wchar_t * sym;
};

/******************************************************************************/

static const struct RomanMap romanMap [] =
{
  { 1000, L"Ⅿ" }, { 900, L"ⅭⅯ" }, { 500, L"Ⅾ" }, { 400, L"ⅭⅮ" },
  {  100, L"Ⅽ" }, {  90, L"ⅩⅭ" }, {  50, L"Ⅼ" }, {  40, L"ⅩⅬ" },
  {   10, L"Ⅹ" }, {   9, L"Ⅸ"  }, {   5, L"Ⅴ" }, {   4, L"Ⅳ"  },
  {    1, L"Ⅰ" }
};

/******************************************************************************/

static bool
match_utf8 (const char * s)
{
  if (!s)
    return false;

  size_t len = strlen (s);

  if (4 <= len)
    for (size_t i = 0; i <= len - 4; ++i)
      if ('u' == tolower (s [i])     &&
          't' == tolower (s [i + 1]) &&
          'f' == tolower (s [i + 2]) &&
          '8' == s [i + 3])
        return true;

  if (5 <= len)
    for (size_t i = 0; i <= len - 5; ++i)
      if ('u' == tolower (s [i])     &&
          't' == tolower (s [i + 1]) &&
          'f' == tolower (s [i + 2]) &&
          '8' == s [i + 4])
        return true;

  return false;
}

/******************************************************************************/

static void
toRoman (int num, wchar_t * buf, size_t size)
{
  if (num < 13) {
    static const wchar_t * directMap [13] = {
      L"",
      L"Ⅰ", L"Ⅱ", L"Ⅲ", L"Ⅳ", L"Ⅴ", L"Ⅵ",
      L"Ⅶ", L"Ⅷ", L"Ⅸ", L"Ⅹ", L"Ⅺ", L"Ⅻ"
    };
    (void)wcsncpy (buf, directMap [num], size - 1);
    buf [size - 1] = L'\0';

    return;
  }

  buf [0] = L'\0';

  for (size_t i = 0; i < sizeof (romanMap) / sizeof (romanMap [0]); i++)
    if (1 == romanMap [i].val) {
      if (num < 4) {
        static const wchar_t * onesMap [4] = { L"", L"Ⅰ", L"Ⅱ", L"Ⅲ" };
        (void)wcscat (buf, onesMap [num]);
        num = 0;
      } else while (1 <= num && wcslen (buf) + 1 < size) {
          (void)wcscat (buf, L"Ⅰ");
          num -= 1;
        }
    } else while (num >= romanMap [i].val &&
                  wcslen (buf) + wcslen (romanMap [i].sym) < size - 1) {
        (void)wcscat (buf, romanMap [i].sym);
        num -= romanMap [i].val;
      }
}

/******************************************************************************/

static int
isEaster (int day, int month, int year)
{
    int a = year % 19;
    int b = year / 100;
    int c = year % 100;
    int d = b / 4;
    int e = b % 4;
    int f = (b + 8) / 25;
    int g = (b - f + 1) / 3;
    int h = (19 * a + b - d - g + 15) % 30;
    int i = c / 4;
    int k = c % 4;
    int l = (32 + 2 * e + 2 * i - h - k) % 7;
    int m = (a + 11 * h + 22 * l) / 451;

    int easterMonth =   (h + l - 7 * m + 114) / 31;
    int easterDay   = ( (h + l - 7 * m + 114) % 31 ) + 1;

    return (day == easterDay && month + 1 == easterMonth);
}

/******************************************************************************/

static void
buildLatinDate (wchar_t * output, size_t size)
{
  wchar_t * holiday = L"";
  const wchar_t * months [] = {
    L"ianuarias",  L"februarias", L"martias",   L"aprilis",
    L"maias",      L"iunias",     L"iulias",    L"augustas",
    L"septembres", L"octobres",   L"novembres", L"decembres"
  };

  time_t t = time (NULL);
  struct tm * now = localtime (&t);

  int d     = now->tm_mday;
  int month = now->tm_mon;
  int year  = now->tm_year + 1900;

  int daysInMonth;

  if (1582 < year && isEaster (d, month, year))
    holiday = L"die·​paschæ·​";
  else if (25 == d && 11 == month)
    holiday = L"die·​nativitatis·​domini·​";

  switch (month) {
    case 1:
      daysInMonth =
          (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
      break;

    case 0: case 2: case 4: case 6: case 7: case 9: case 11:
      daysInMonth = 31;
      break;

    default:
      daysInMonth = 30;
  }

  int nones = (2 == month || 4 == month || 6 == month || 9 == month) ? 7 : 5;
  int ides  = (2 == month || 4 == month || 6 == month || 9 == month) ? 15 : 13;

  wchar_t dayPart    [MaxDayPartLength]    = L"";
  wchar_t romanCount [MaxRomanCountLength] = L"";
  wchar_t romanYear  [MaxRomanYearLength]  = L"";

  const wchar_t * marker      = NULL;
  const wchar_t * markerMonth = NULL;

  if (1 == d) {
    (void)wcscpy (dayPart, L"kalendae");
    markerMonth = months [month];
  } else if (d < nones) {
    int count = nones - d + 1;
    toRoman (count, romanCount, MaxRomanCountLength);
    marker      = L"nonas";
    markerMonth = months [month];
    if (2 == count)
      (void)wcscpy (dayPart, L"pridie");
    else
      (void)swprintf (dayPart, MaxDayPartLength,
                      L"ante·​diem·​%ls",
                      romanCount);
  } else if (d == nones) {
    (void)wcscpy (dayPart, L"nonas");
    markerMonth = months [month];
  } else if (d < ides) {
    int count = ides - d + 1;
    toRoman (count, romanCount, MaxRomanCountLength);
    marker      = L"idus";
    markerMonth = months [month];
    if (2 == count)
      (void)wcscpy (dayPart, L"pridie");
    else
      (void)swprintf (dayPart, MaxDayPartLength,
                      L"ante·​diem·​%ls",
                      romanCount);
  } else if (d == ides) {
    (void)wcscpy (dayPart, L"idus");
    markerMonth = months [month];
  } else {
    int count = daysInMonth - d + 2;
    toRoman (count, romanCount, MaxRomanCountLength);
    marker      = L"kalendas";
    markerMonth = months [ (month + 1) % 12];
    if (2 == count)
      (void)wcscpy (dayPart, L"pridie");
    else
      (void)swprintf (dayPart, MaxDayPartLength,
                      L"ante·​diem·​%ls",
                      romanCount);
  }

  if (1 > year || 3999 < year) {
    (void)fprintf (stderr, "ERROR: Invalid year: must be >0 and <4000\n");
    exit (1);
  }

  toRoman (year, romanYear, MaxRomanYearLength);

  if (NULL == marker)
    (void)swprintf (output, size,
                    L"ultimum·​recognitum·​est·​%ls%ls"
                    L"·​%ls·​anno·​domini·​%ls\n",
                    holiday, dayPart, markerMonth, romanYear);
  else
    (void)swprintf (output, size,
                    L"ultimum·​recognitum·​est·​%ls%ls·​%ls"
                    L"·​%ls·​anno·​domini·​%ls\n",
                    holiday, dayPart, marker, markerMonth, romanYear);
}

/******************************************************************************/

int
main (void)
{
  if (0 != setlocale (LC_ALL, "")) {
    char * codeset = nl_langinfo (CODESET);
    if (codeset)
      if (!match_utf8 (codeset)) {
        (void)fprintf (stderr, "WARNING: Likely non-UTF-8 encoding: '%s'\n",
                       codeset);
        (void)fflush (stdout);
      }
  } else {
    (void)fprintf (stderr, "ERROR: Failed to setup locale!\n");
    exit (1);
  }

  wchar_t inscription [MaxLatinLength] = { 0 };
  buildLatinDate (inscription, MaxLatinLength);

  return !wprintf (L"%ls", inscription);
}

/******************************************************************************/
