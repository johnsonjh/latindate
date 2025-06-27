<!-- README.md -->
<!-- Copyright (c) 2025 Jeffrey H. Johnson -->
<!-- SPDX-License-Identifier: MIT-0 -->
<!-- vim: set expandtab cc=80 ft=markdown : -->
# latindate

This is a utility, written in C99, that displays a timestamp in Latin using
the inclusive counting style of the
[Roman calendar](https://en.m.wikipedia.org/wiki/Roman_calendar)
(and with Roman numerals displayed properly using the
[Roman numeral glyphs](https://en.wikipedia.org/wiki/Numerals_in_Unicode#Roman_numerals)
from the
[Number Forms](https://en.wikipedia.org/wiki/Number_Forms)
[Unicode block](https://en.wikipedia.org/wiki/Unicode_block)).

It is regularly tested on IBM AIX and Linux (using both glibc and musl).

```
$ date
Thu Jun 26 10:53:02 PM EDT 2025

$ latindate          
ultimum·​recognitum·​est·​ante·​diem·​Ⅵ·​kalendas·​iulias·​anno·​domini·​ⅯⅯⅩⅩⅤ
```

The text translated to English reads…

> Last revised on the sixth day before the
> [kalends](https://en.wikipedia.org/wiki/Calends)
> of July in the year of our Lord 2025

…or, more succinctly:

> Last revised June 26 2025

The Latin text is output in lower case, with words delimited by interpunct
(`·`) characters, followed by Unicode zero-width space characters, for easier
post-processing.  Ligatures are used where appropriate.  Easter and Christmas
have specific handling.

If you want to see generate the Latin text for *arbitrary* dates,
[`libfaketime`](https://github.com/wolfcw/libfaketime) comes in handy:

```
$ faketime -m 2032-03-28 latindate
ultimum·​recognitum·​est·​die·​paschæ·​ante·​diem·​Ⅴ·​kalendas·​aprilis·​anno·​domini·​ⅯⅯⅩⅩⅩⅡ
```

The output itself is intended to be rendered using small capital glyphs, but
if you prefer regular capitals, you can leverage `tr` for the conversion:

```
faketime -m 2032-03-28 latindate | tr '[:lower:]' '[:upper:]' | tr 'æ' 'Æ'
ULTIMUM·​RECOGNITUM·​EST·​DIE·​PASCHÆ·​ANTE·​DIEM·​Ⅴ·​KALENDAS·​APRILIS·​ANNO·​DOMINI·​ⅯⅯⅩⅩⅩⅡ
```

For an example of this program in action, check out the timestamps on the
[**Illuminationes**](https://johnsonjh.github.io/) web page.
