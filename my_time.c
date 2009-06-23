#include "my_time.h"

// ***************************************************
//  days_in_month
/*!
    @brief Return number of days in month in 
           selected year.

    @param int month Month

    @param int year Year 

    @return int Number of days.
*/
// ***************************************************
int days_in_month( int month, int year )
{
  int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  if( is_leap_year( year ) )
    days[1] = 29;

  return days[ month-1 ];
}

// ***************************************************
//  get_diff_in_time
/*!
    @brief Calculate difference between two unix timestamps.
           It does not matter which one you will pass
           as an first argument. 

    @param int start First timestamp. 

    @param int end Second timestamp.

    @return a_time Struct where we have stored difference
            in hours, minutes and seconds. 
            NOTE! This is meant that eg. 1h, 23min and 32sec.
            So, it is not same value in seconds, minutes 
            and hours!
*/
// ***************************************************
a_time get_diff_in_time( int start, int end )
{
  if( start < end )
  {
    int swap = start;
    start = end;
    end = swap;
  } 
    
  a_time my_time;
  float tmp;
  int hours, min, sec, tmpint;

  // Count difference between stamps in seconds and typecast it to float
  tmp = (float) start - end;

  // Minutes
  min = (int) tmp / 60;

  // Hours
  hours = (int) min / 60;
  min = (int) min % 60;

  // Seconds
  tmpint = (int) tmp;
  sec = tmpint % 60;

  my_time.hours = hours;
  my_time.min = min;
  my_time.sec = sec;

  return my_time;
}

// ***************************************************
//  is_leap_year
/*!
    @brief Checks if year is leap year.

    @param int year Year what we check

    @return 1 if year is a leap year, 
      0 if year is not a leap year.
*/
// ***************************************************
int is_leap_year( int year )
{
  // Only every fourth year can be leap year
  if( ( year % 4 ) == 0 )
  {
    // Now we must divide year with 100 so we can detect if
    // this is excatly one century, eg. 1100, 1200, 1300 etc.
    if( ( year % 100 ) == 0 )
    {
      // Normally every 4th year is leap year. There is still
      // an exception. If year is divided by 400 and we get
      // value where is "floating point", then the year is not
      // meant to be an leap year (1700, 1800, 1900).
      float modulus = (float) year / 400;

      // Is modulus value same than if we typecast it to int?
      // If it is not, then this year is not leap year.
      if(! ( (int) modulus == modulus ) )
        return 0;
    }
    return 1;
  }
  else
  {
    return 0;
  }
}

// ***************************************************
//  get_date
/*!
    @brief Get date from timestamp

    @param time_t date Unix timestamp

    @return a_date Date and time in structure.
*/
// ***************************************************
a_datetime get_date( time_t date )
{
  long int day_length = 60 * 60 * 24;
  a_datetime retval;

  time_t tmp_date = 0;

  // Length of one day
  int dayl = 60*60*24;

  // How many days ago was 1970-01-01?
  int days = date / dayl;

  // How many years are passed since 1970.
  int years = days / 365;
  int i;

  // We loop from 1970 to 1970 + number of years what are passed.
  int maxyears = years + 1970;

  // How many days are passed since 1970-01-01
  int days_total = 0;

  // Loop from 1970 to year we wanted
  for( i=1970; i < maxyears; i++ )
  {
    int num_days = 365;

    // In leap year there is 366 days.
    if( is_leap_year( i ) )
      num_days = 366;

    // Add number of days in year i to total counter
    days_total += num_days;
    tmp_date += num_days * day_length;
  }

  // In this stage we have a value how many days are gone
  // from 1970 to YEAR-01-01. It is in days_total variable.
  // Searh from the beginning of the YEAR to end of that year,
  // and search which month and which day is what we are looking for.
  for( i=0; i < 12; i++ )
  {
    // Number of days of month we are looping
    int tmp = days_in_month( i+1, maxyears );

    // If we add number of days in this month to total sum
    // of days, and if we get greater value than days we
    // are looking for, then we have found the right month.
    if( days_total + tmp > days )
    {
      int i2;

      // Loop all days in this month and add them to days_total
      // counter. When we get value which equals to DAYS, then
      // we have found the correct date!
      for( i2=0; i2 < tmp; i2++ )
      {
        if( days_total == days )
        {
          // Get difference from day start to timestamp value
          a_time diff = get_diff_in_time( tmp_date, date );

          retval.year = maxyears;
          retval.month = i+1;
          retval.date = i2+1;
          retval.hours = diff.hours;
          retval.min = diff.min;
          retval.sec = diff.sec;

          return retval;
        }

        days_total++;
        tmp_date += day_length;
      }
    }
    
    days_total += tmp;
    tmp_date += tmp * day_length;
  }

  // We should never go this far... but just in case.
  return retval;
}
