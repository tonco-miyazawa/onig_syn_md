# onig_syn_md.c

This is a program to print the data used in [oniguruma/doc/SYNTAX.md](https://github.com/kkos/oniguruma/blob/master/doc/SYNTAX.md) .
This will read option values ​​from Oniguruma installed on your system.
Please see the following link for instructions on how to use it:\
[Oniguruma #315 - Print the table in 'SYNTAX.md'](https://github.com/kkos/oniguruma/issues/315#issue-2719589814)

**Oniguruma (Owner: K.Kosako) [https://github.com/kkos/oniguruma](https://github.com/kkos/oniguruma)**\
**SYNTAX.md (Author: seanofw) [https://github.com/seanofw](https://github.com/seanofw)**

### version 0005
- Changed the Style of 'Syntax option values' table.

### version 0004
- Added 'syntax option values' table.
  
### version 0003
- Added new option : `ONIG_SYN_ESC_P_WITH_ONE_CHAR_PROP (1U<<15)  /* \pL */`
- Shows the current mode (`1`, `2`, or `3`).
- Changed '`Syntax Flags (syn)`' to '`Syntax Flags (behavior)`' .
- Changed the code style to look like Oniguruma's.  
- small fix (typo etc..)

### version 0002
- Changed the code style to look like Oniguruma's.
- PRINT_DEBUG_INFO : Displays the option value in eight digits.
  
### version 0001
- Prototype (Dec 5, 2024)
