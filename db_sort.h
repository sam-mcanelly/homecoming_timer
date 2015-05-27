#ifndef DB_SORT
#define DB_SORT

#include "student.h"

class DB_Sort
{
public:
    enum sort_by
    {
        F_NAME,
        L_NAME,
        TIME_COMPLETE,
        TIME_REQUIRED,
        STATUS
    };

    static void sort(Student **s_ptr, int student_count, sort_by attribute);

private:

    static void sort(Student **s_ptr, sort_by attribute, int low, int high);
    static int partition_by_f_name(Student **s_ptr, int low, int high);
    static int partition_by_l_name(Student **s_ptr, int low, int high);
    static int partition_by_status(Student **s_ptr, int low, int high);
    static int partition_by_hours_complete(Student **s_ptr, int low, int high);
    static int partition_by_hours_required(Student **s_ptr, int low, int high);

};

#endif // DB_SORT

