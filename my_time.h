// This is needed for time_t
#include <time.h>

#ifndef __my_time__
#define __my_time__

//! Structure for storing times
typedef struct
{
  //! Hour value
  int hours;

  //! Minutes
  int min;

  //! Seconds
  int sec;
} a_time;

//! Structure for storing dates
typedef struct
{
  //! Year
  int year;

  //! Month
  int month;

  //! Date of month
  int date;
} a_date;

//! Structure for storing dates and times
typedef struct
{
  //! Year
  int year;

  //! Month
  int month;

  //! Date of month
  int date;

  //! Hour value
  int hours;

  //! Minutes
  int min;

  //! Seconds
  int sec;
} a_datetime;

a_time get_diff_in_time( int start, int end );
int is_leap_year( int year );
int days_in_month( int month, int year );
a_datetime get_date( time_t date );
#endif
