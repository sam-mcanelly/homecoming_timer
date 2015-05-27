#ifndef DB_CONTROLLER_H
#define DB_CONTROLLER_H

#define HOCO_DB "/hoco_db"

#include "student.h"
#include <fstream>
#include <iostream>
#include <QDir>

/*----------------------------------------------
 *
 *      ===========db_controller.h==========
 *
 *
 * contains the functions needed for loading,
 * storing, and reporting information within
 * the databases.
 *
 * Author: Sam McAnelly
 * Last Revised: 5/21/2015
 *
 *
 * ---------------------------------------------*/

class DB_Controller
{
public:
    enum database
    {
        SETTINGS,
        NAME,
        CWID,
        TIME_COMPLETE,
        TIME_REQUIRED
    };

    enum db_type
    {
        STRING_DB,
        FLOAT_DB
    };

    enum db_gender
    {
        GUYS,
        GIRLS
    };

    enum report_type
    {
        DAILY,
        WEEKLY
    };

    Student **ptr_db_students;

    DB_Controller(db_gender gen, Student **student_array);
    DB_Controller();
    ~DB_Controller();

    void load_data();

    void set_gender(db_gender gen);
    void set_student_array(Student **student_array);

    int get_student_count();
    void set_student_count(int count);

    void generate_report(report_type type);

    int search_name(std::string name);
    int search_card_number(std::string number);

private:
    int student_count;
    int current_week;
    int current_day;
    float fine_per_hour;

    db_gender gender;
    db_type type;

    std::string *daily_report;
    std::string *weekly_report;
    std::string *settings;

    std::string *student_names;
    std::string *student_card_numbers;
    float       *student_req_hours;
    float       *student_cmplt_hours;

    void initialize_db_controller(db_gender gen, Student **students);
    void save_student_data();

    void fetch_settings();

    const char *compute_db(db_gender gender, database db);
    void compute_report_output(db_gender gender, report_type type);

    void fill_string_array(database db);
    void fill_float_array(database db);

    void populate_student_information();

    void populate_student_names();
    void populate_student_card_numbers();
    void populate_student_required_hours();
    void populate_student_completed_hours();

    void generate_daily_report();
    void generate_weekly_report();

    void write_daily_report(const char *abs_file_path);
    void write_weekly_report(const char *abs_file_path);

};



#endif // DB_CONTROLLER_H

