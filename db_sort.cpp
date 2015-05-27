#include "db_sort.h"

void DB_Sort::sort(Student **s_ptr, int student_count, sort_by attribute)
{
    sort(s_ptr, attribute, 0, student_count);
}

void DB_Sort::sort(Student **s_ptr, sort_by attribute, int low, int high)
{
    if (high <= low)
        return;                                 //if done, exit

    int j;

    switch (attribute)                          //get index of dividing item
    {
    case (F_NAME):
        j = partition_by_f_name(s_ptr, low, high);
        break;
    case (L_NAME):
        j = partition_by_l_name(s_ptr, low, high);
        break;
    case (TIME_COMPLETE):
        j = partition_by_hours_complete(s_ptr, low, high);
        break;
    case (TIME_REQUIRED):
        j = partition_by_hours_required(s_ptr, low, high);
        break;
    case (STATUS):
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


    int i = low;                                //left scan index
    int j = high + 1;                           //right scan index
    Student v = *s_ptr[low];                        //partitioning item

    while (true)
    {
        //Scan right, scan left, check for scan complete, and exchange

        //while((*s_ptr[++i]->get_name().c_str())[0])
    }
}

int DB_Sort::partition_by_l_name(Student **s_ptr, int low, int high)
{
    //partitions into s_ptr[low ... i-1], s_ptr[j], and a[i+1 ... high]


    int i = low;                                //left scan index
    int j = high + 1;                           //right scan index
    Student v = *s_ptr[low];                        //partitioning item

    while (true)
    {
        //Scan right, scan left, check for scan complete, and exchange

        //while((*s_ptr[++i]->get_name().c_str())[0])
    }
}

int DB_Sort::partition_by_status(Student **s_ptr, int low, int high)
{
    //partitions into s_ptr[low ... i-1], s_ptr[j], and a[i+1 ... high]


    int i = low;                                //left scan index
    int j = high + 1;                           //right scan index
    Student v = *s_ptr[low];                        //partitioning item

    while (true)
    {
        //Scan right, scan left, check for scan complete, and exchange

        //while((*s_ptr[++i]->get_name().c_str())[0])
    }
}

int DB_Sort::partition_by_hours_complete(Student **s_ptr, int low, int high)
{
    //partitions into s_ptr[low ... i-1], s_ptr[j], and a[i+1 ... high]


    int i = low;                                //left scan index
    int j = high + 1;                           //right scan index
    Student v = *s_ptr[low];                        //partitioning item

    while (true)
    {
        //Scan right, scan left, check for scan complete, and exchange

        //while((*s_ptr[++i]->get_name().c_str())[0])
    }
}

int DB_Sort::partition_by_hours_required(Student **s_ptr, int low, int high)
{
    //partitions into s_ptr[low ... i-1], s_ptr[j], and a[i+1 ... high]


    int i = low;                                //left scan index
    int j = high + 1;                           //right scan index
    Student v = *s_ptr[low];                        //partitioning item

    while (true)
    {
        //Scan right, scan left, check for scan complete, and exchange

        //while((*s_ptr[++i]->get_name().c_str())[0])
    }
}
