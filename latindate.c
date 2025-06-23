#include <ctype.h>
#include <langinfo.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

/*****************************************************************************/

#define MaxLatinLength    101
#define MaxDayPartLength   17
#define MaxRomanYearLength  9
#define MaxRomanCountLength 3

/*****************************************************************************/

struct RomanMap {
  int val;
  const wchar_t * sym;
};

/*****************************************************************************/

static const struct RomanMap romanMap [] = {
  { 1000, L"Ⅿ" }, { 900, L"ⅭⅯ" }, { 500, L"Ⅾ" }, { 400, L"ⅭⅮ" },
  {  100, L"Ⅽ" }, {  90, L"ⅩⅭ" }, {  50, L"Ⅼ" }, {  40, L"ⅩⅬ" },
  {   10, L"Ⅹ" }, {   9, L"Ⅸ"  }, {   5, L"Ⅴ" }, {   4, L"Ⅳ"  },
  {    1, L"Ⅰ" }
};

/*****************************************************************************/

static bool
match_utf8(const char * s) {
  if (!s)
    return false;

  size_t len = strlen(s);

  if (len < 5)
    return false;

  for (size_t i = 0; i <= len - 5; ++i)
    if ('u' == tolower(s [i]) &&
        't' == tolower(s [i + 1]) &&
        'f' == tolower(s [i + 2]) &&
        '8' == tolower(s [i + 4]))
      return true;

  return false;
}

/*****************************************************************************/

static void
toRoman (int num, wchar_t * buf, size_t size) {
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
      } else {
        while (1 <= num && wcslen (buf) + 1 < size) {
          (void)wcscat (buf, L"Ⅰ");
          num -= 1;
        }
      }
    } else {
      while (num >= romanMap [i].val &&
             wcslen (buf) + wcslen (romanMap [i].sym) < size - 1) {
        (void)wcscat (buf, romanMap [i].sym);
        num -= romanMap [i].val;
      }
    }
}

/*****************************************************************************/

static void
buildLatinDate (wchar_t * output, size_t size) {
  const wchar_t * months [] = {
    L"ianuarias",  L"februarias", L"martias",   L"aprilis",
    L"maias",      L"iunias",     L"iulias",    L"augustas",
    L"septembres", L"octobres",   L"novembres", L"decembres"
  };

  time_t t        = time (NULL);
  struct tm * now = localtime (&t);

  int d     = now->tm_mday;
  int month = now->tm_mon;
  int year  = now->tm_year + 1900;

  int daysInMonth;

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
    markerMonth = months [(month + 1) % 12];
    if (2 == count)
      (void)wcscpy (dayPart, L"pridie");
    else
      (void)swprintf (dayPart, MaxDayPartLength,
                      L"ante·​diem·​%ls",
                      romanCount);
  }

  if (1 > year || 3999 < year) {
    (void)fprintf (stderr, "ERROR: Invalid year: must be >0 and <4000\n");
    exit(1);
  }

  toRoman (year, romanYear, MaxRomanYearLength);

  if (NULL == marker)
    (void)swprintf (output, size,
                    L"ultimum·​recognitum·​est·​%ls"
                     "·​%ls·​anno·​domini·​%ls\n",
                    dayPart, markerMonth, romanYear);
  else
    (void)swprintf (output, size,
                    L"ultimum·​recognitum·​est·​%ls·​%ls"
                     "·​%ls·​anno·​domini·​%ls\n",
                    dayPart, marker, markerMonth, romanYear);
}

/*****************************************************************************/

int
main (void)
{
  if (0 != setlocale (LC_ALL, "")) {
    char * codeset = nl_langinfo (CODESET);
    if (codeset) {
      if (!match_utf8 (codeset))
        (void)fprintf (stderr, "WARNING: Likely non-UTF-8 encoding: '%s'\n",
                       codeset);
    } else
      (void)fprintf (stderr, "WARNING: Failed to query encoding!\n");
  } else {
    (void)fprintf (stderr, "ERROR: Failed to setup locale!\n");
    exit(1);
  }

  wchar_t inscription [MaxLatinLength] = { 0 };
  buildLatinDate (inscription, MaxLatinLength);

  return wprintf (L"%ls", inscription);
}

/*****************************************************************************/
