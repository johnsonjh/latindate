<!-- README.md -->
<!-- Copyright (c) 2025 Jeffrey H. Johnson -->
<!-- SPDX-License-Identifier: MIT-0 -->
<!-- vim: set expandtab cc=80 ft=markdown : -->
# latindate

This is a utility, written in C99, that displays a timestamp in Latin using
the Roman calendar and Roman numerals.

The output uses
[Roman numeral glyphs](https://en.wikipedia.org/wiki/Numerals_in_Unicode#Roman_numerals)
(from the
[Number Forms](https://en.wikipedia.org/wiki/Number_Forms)
[Unicode block](https://en.wikipedia.org/wiki/Unicode_block)) to
represent dates.

```
$ date
Thu Jun 26 10:53:02 PM EDT 2025

$ ddate
Today is Boomtime, the 31st day of Confusion in the YOLD 3191

$ latindate          
ultimum·​recognitum·​est·​ante·​diem·​Ⅵ·​kalendas·​iulias·​anno·​domini·​ⅯⅯⅩⅩⅤ
```

This translates as "Last revised on the sixth day before the
[kalends](https://en.wikipedia.org/wiki/Calends)
of July, in the year of our Lord 2025", or more succinctly,
"Last revised June 26 2025".

The Latin text output in lower case, with words delimited by interpunct (`·`)
characters, followed by Unicode zero-width space characters, for easier
post-processing.  (The output itself is intended to be rendered using small
capital glyphs.)

For an example of this utility in action, see the timestamps on the
[**Illuminationes**](https://johnsonjh.github.io/) web page.

Note that if you want to see generate Latin text for *arbitrary* dates,
[`libfaketime`](https://github.com/wolfcw/libfaketime) comes in handy:

```
$ faketime -m 2032-03-28 latindate
ultimum·​recognitum·​est·​die·​paschæ·​ante·​diem·​Ⅴ·​kalendas·​aprilis·​anno·​domini·​ⅯⅯⅩⅩⅩⅡ

$ faketime -m 1893-12-25 latindate
ultimum·​recognitum·​est·​die·​nativitatis·​domini·​ante·​diem·​Ⅷ·​kalendas·​ianuarias·​anno·​domini·​ⅯⅮⅭⅭⅭⅩⅭⅢ
```
