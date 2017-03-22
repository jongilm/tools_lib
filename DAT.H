char *month_names[12];
long dmy_to_day(int day, int month, long year, int julian);
void day_to_dmy( long day, int *d, int *m, long *y, int julian);

char *islam_month_name[12];
long islam_dmy_to_day(int day, int month, long year);
void islam_day_to_dmy(long day, int *d, int *m, long *y);
