#include "db_sort.h"

void DB_Sort::sort(Student **s_ptr, int student_count, sort_by attribute)
{
    sort(s_ptr, attribute, 0, student_count - 1);
}

void DB_Sort::sort(Student **s_ptr, sort_by attribute, int low, int high)
{
    if (high <= low)
        return;                                 //if done, exit

    int j;

    switch (attribute)                          //get index of dividing item
    {
    case ( F_NAME ):
        j = partition_by_f_name(s_ptr, low, high);
        break;
    case ( L_NAME ):
        j = partition_by_l_name(s_ptr, low, high);
        break;
    case ( TIME_COMPLETE ):
        j = partition_by_hours_complete(s_ptr, low, high);
        break;
    case ( TIME_REQUIRED ):
        j = partition_by_hours_required(s_ptr, low, high);
        break;
    case ( TIME_INCOMPLETE ):
        j = partition_by_hours_incomplete(s_ptr, low, high);
        break;
    case ( STATUS ):
        j = partition_by_status(s_ptr, low, high);
        break;
    default:
        //if no attribute given, give up and quit because the call was lazy and incomplete
        return;
    }


    sort(s_ptr, attribute, low, j - 1);                    //sort left side (low side)
    sort(s_ptr, attribute, j + 1, high);                   //sort right side (high side)

}

int DB_Sort::partition_by_f_name(Student **s_ptr, int low, int high)
{
    //partitions into s_ptr[low ... i-1], s_ptr[j], and a[i+1 ... high]


    int i = low;                                 //left scan index
    int j = high + 1;                            //right scan index

    Student *v = s_ptr[low];                     //partitioning item
    Student *temp;

    while (true)
    {
        //Scan right, scan left, check for scan complete, and exchange

        while( compare_names(s_ptr[++i]->get_first_name(), v->get_first_name()) )
            if( i == high )  break;
        while( compare_names( v->get_first_name(), s_ptr[--j]->get_first_name() ) )
            if( j == low)    break;
        if( i >= j )  break;

        temp = s_ptr[i];
        s_ptr[i] = s_ptr[j];
        s_ptr[j] = temp;
    }

    s_ptr[low] = s_ptr[j];
    s_ptr[j]   = v;

    return j;
}

int DB_Sort::partition_by_l_name(Student **s_ptr, int low, int high)
{
    //partitions into s_ptr[low ... i-1], s_ptr[j], and a[i+1 ... high]


    int i = low;                                 //left scan index
    int j = high + 1;                            //right scan index
                                  //name iterator
    Student *v = s_ptr[low];                     //partitioning item
    Student *temp;

    while (true)
    {
        //Scan right, scan left, check for scan complete, and exchange

        while( compare_names(s_ptr[++i]->get_last_name(), v->get_last_name()) )
            if( i == high )  break;
        while( compare_names( v->get_last_name(), s_ptr[--j]->get_last_name() ) )
            if( j == low)    break;
        if( i >= j )  break;

        temp = s_ptr[i];
        s_ptr[i] = s_ptr[j];
        s_ptr[j] = temp;
    }

    s_ptr[low] = s_ptr[j];
    s_ptr[j]   = v;

    return j;
}

int DB_Sort::partition_by_hours_complete(Student **s_ptr, int low, int high)
{
    //partitions into s_ptr[low ... i-1], s_ptr[j], and a[i+1 ... high]

    int i = low;                                //left scan index
    int j = high + 1;                           //right scan index
    Student *v = s_ptr[low];                    //partitioning item
    Student *temp;

    while (true)
    {
        while( s_ptr[++i]->get_hours_complete() > v->get_hours_complete() )
            if ( i == high )  break;
        while( v->get_hours_complete() > s_ptr[--j]->get_hours_complete() )
            if ( j == low )   break;
        if ( i >= j ) break;

        temp = s_ptr[i];
        s_ptr[i] = s_ptr[j];
        s_ptr[j] = temp;
    }

    s_ptr[low] = s_ptr[j];
    s_ptr[j] = v;

    return j;
}

int DB_Sort::partition_by_hours_required(Student **s_ptr, int low, int high)
{
    //partitions into s_ptr[low ... i-1], s_ptr[j], and a[i+1 ... high]


    int i = low;                                //left scan index
    int j = high + 1;                           //right scan index
    Student *v = s_ptr[low];                    //partitioning item
    Student *temp;

    while (true)
    {
        while(s_ptr[++i]->get_hours_required() < v->get_hours_required())
            if (i == high)  break;
        while(v->get_hours_required() < s_ptr[--j]->get_hours_required())
            if ( j == low )   break;
        if ( i >= j ) break;

        temp = s_ptr[i];
        s_ptr[i] = s_ptr[j];
        s_ptr[j] = temp;
    }

    s_ptr[low] = s_ptr[j];
    s_ptr[j] = v;

    return j;
}

int DB_Sort::partition_by_hours_incomplete(Student **s_ptr, int low, int high)
{
    //partitions into s_ptr[low ... i-1], s_ptr[j], and a[i+1 ... high]


    int i = low;                                //left scan index
    int j = high + 1;                           //right scan index
    Student *v = s_ptr[low];                    //partitioning item
    Student *temp;

    while (true)
    {
        while(s_ptr[++i]->get_hours_incomplete() > v->get_hours_incomplete())
            if (i == high)  break;
        while(v->get_hours_incomplete() > s_ptr[--j]->get_hours_incomplete())
            if ( j == low )   break;
        if ( i >= j ) break;

        temp = s_ptr[i];
        s_ptr[i] = s_ptr[j];
        s_ptr[j] = temp;
    }

    s_ptr[low] = s_ptr[j];
    s_ptr[j] = v;

    return j;
}

int DB_Sort::partition_by_status(Student **s_ptr, int low, int high)
{
    //partitions into s_ptr[low ... i-1], s_ptr[j], and a[i+1 ... high]

    int i = low;                                //left scan index
    int j = high + 1;                           //right scan index
    Student *v = s_ptr[low];                    //partitioning item
    Student *temp;

    while (true)
    {
        while(s_ptr[++i]->get_status() > v->get_status())
            if (i == high)  break;
        while(v->get_status() > s_ptr[--j]->get_status())
            if ( j == low )   break;
        if ( i >= j ) break;

        temp = s_ptr[i];
        s_ptr[i] = s_ptr[j];
        s_ptr[j] = temp;
    }

    s_ptr[low] = s_ptr[j];
    s_ptr[j] = v;

    return j;
}

bool DB_Sort::compare_names(const char *s1, const char *s2)
{

    int i;

    for( i = 0; i < get_shortest_length(s1, s2); i++)
    {
        if( s1[i] > s2[i] )
            return false;
        else if( s1[i] < s2[i] )
            return true;
    }

    return true;
}

int DB_Sort::get_shortest_length(const char *str1, const char *str2)
{
    int i = 0;
    int j = 0;

    while(str1[i] != '\0')
        ++i;

    while(str2[j] != '\0')
        ++j;

    if(i > j)
        return i;
    else
        return j;
}
