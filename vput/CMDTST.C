/* cmdtest - test command.c, also print time/date fields */

#include "printf.h"

int mydate[3];  /* day, month, year */

struct datime {
        unsigned date;
        char hour;
        char minute;
} dtaccess, dtupdate;

/* modays - return days in given month and year */
modays(month, year)
int month;
int year;
{
        int days;
        
        switch(month)
        {
                /* 30 days hath september, april, june and november */
                case 9:
                case 4:
                case 6:
                case 11:
                        days = 30;
                        break;

                case 2:
                        days = is_leap(year) ? 29 : 28;
                        break;

                /* all the rest have 31! */
                default:
                        days = 31;
                        break;
        }
        
        return days;
}

/* return TRUE if year is a leap year */
is_leap(year) {
        return (((year%4==0)&&((year%100)!=0)) || ((year%400)==0));
}

/* takes a CP/M 3 day count (from directory entry) and
** computes day, month and year based on definition that
** 1/1/1978 is day 1.
*/
dodate(days, date)
int days;
int date[];
{
        int yyyy, dd, m, mm;
        
        yyyy = 1978;
        dd = days;
        
        while ((dd>365) && ((dd!=366) || !is_leap(yyyy))) {
                dd -= is_leap(yyyy) ? 366 : 365;
                ++yyyy;
        }

        /* have year & days in the year; convert to dd/mm/yyyy */
        mm = 1;
        while (dd > (m=modays(mm, yyyy))) {
                dd -= m;
                ++mm;
        }
        date[0] = dd;
        date[1] = mm;
        date[2] = yyyy;
}


main(argc, argv)
int argc;
char *argv[];
{
    int i, j;
    char fcb[36];
    char *c;

    command(&argc, &argv);

    for (i=0; i<argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
        makfcb(argv[i], fcb);
        bdos(102, fcb);

        /* access date in bytes 24-27 */
        c = (char *) &dtaccess;
        for (j=24; j<=27; j++)
            *c++ = fcb[j];
        printf("Access: %x, %x, %x\n", dtaccess.date, dtaccess.hour,
             dtaccess.minute);
                                
        /* update date in bytes 28-31 */
        c = (char *) &dtupdate;
        for (j=28; j<=31; j++)
            *c++ = fcb[j];
        printf("Update: %x, %x, %x\n", dtupdate.date, dtupdate.hour,
             dtupdate.minute);

        /* get file size */
        bdos(35,fcb);
        printf("File size: %2x %2x %2x\n", fcb[33], fcb[34], fcb[35]);

    }
}
      