#ifndef DB_SORT
#define DB_SORT

#include <stdlib.h>
#include "student.h"

class DB_Sort
{
public:
    enum sort_by
    {
        L_NAME,
        F_NAME,
        TIME_COMPLETE,
        TIME_REQUIRED,
        TIME_INCOMPLETE,
        STATUS
    };

    static void sort(Student *s_ptr, int student_count, sort_by attribute);

private:

    static void sort(Student *s_ptr, sort_by attribute, int low, int high);

    static int partition_by_f_name(Student *s_ptr, int low, int high);
    static int partition_by_l_name(Student *s_ptr, int low, int high);

    static bool compare_names(const std::string s1, const std::string s2);

    static int partition_by_hours_complete(Student *s_ptr, int low, int high);
    static int partition_by_hours_required(Student *s_ptr, int low, int high);
    static int partition_by_hours_incomplete(Student *s_ptr, int low, int high);

    static int partition_by_status(Student *s_ptr, int low, int high);

    static int get_shortest_length(const char *str1, const char *str2);

};

#endif // DB_SORT

