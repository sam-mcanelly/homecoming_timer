#ifndef DB_CONTROLLER_H
#define DB_CONTROLLER_H

#include "student.h"
#include "db_sort.h"
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

    DB_Controller();
    ~DB_Controller();

    void begin();

    void load_data();

    void add_student(std::string name, std::string card_num, float hours_req);

    void set_gender(db_gender gender);

    int get_student_count();

    void generate_report(report_type type);

    int search_name(std::string name);
    int search_card_number(std::string number);

    std::string get_name_from_index(int i);
    std::string get_card_num_from_index(int i);
    float get_hours_req_from_index(int i);
    float get_hours_comp_from_index(int i);
    bool  get_status_from_index(int i);
    void  toggle_status_from_index(int i);

    void sort(DB_Sort::sort_by sortby);

private:
    Student **ptr_db_students_male;
    Student **ptr_db_students_female;
    Student **active_db;

    int *active_count;
    int *active_idx;

    int male_student_count;
    int female_student_count;

    int male_idx;
    int female_idx;

    int current_week;
    int current_day;

    std::string guy_db;
    std::string girl_db;
    std::string report_path;

    float girl_fine;
    float guy_fine;

    db_gender gender;
    db_type type;

    std::string *daily_report;
    std::string *weekly_report;
    std::string *settings;

    std::string *student_names;
    std::string *student_card_numbers;
    float       *student_req_hours;
    float       *student_cmplt_hours;

    void initialize_db_controller();
    void save_student_data();

    void fetch_settings();

    const char *compute_db(db_gender gender, database db);
    void compute_report_output(db_gender gender, report_type type);

    void fill_string_array(db_gender gender, database db);
    void fill_float_array(db_gender gender, database db);

    void populate_student_information();

    void populate_student_names(db_gender gender);
    void populate_student_card_numbers(db_gender gender);
    void populate_student_required_hours(db_gender gender);
    void populate_student_completed_hours(db_gender gender);

    void resize_db();

    void generate_daily_report();
    void generate_weekly_report();

    void write_daily_report(const char *abs_file_path);
    void write_weekly_report(const char *abs_file_path);

};



#endif // DB_CONTROLLER_H

