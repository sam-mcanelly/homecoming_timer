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
        TIME_REQUIRED,
        TIME_DEDUCTED,
        DB_COUNT
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
    void end();

    void add_student(std::string name, std::string card_num, float hours_req);
    void delete_student(int index);

    void set_gender(db_gender gender);

    int get_student_count();

    void generate_report(report_type type);

    int search_name(std::string name);
    int search_card_number(std::string number);

    std::string get_name_from_index(int i);
    std::string get_card_num_from_index(int i);

    float get_hours_req_from_index(int i);
    float get_hours_comp_from_index(int i);

    float get_hours_deducted_from_index(int i);
    void  set_hours_deducted_from_index(int i, float deductions);

    bool  get_status_from_index(int i);
    void  toggle_status_from_index(int i);

    void generate_daily_report(db_gender gen);
    void generate_weekly_report(db_gender gen);

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
    std::string frat_name;
    std::string sor_name;
    std::string report_path;

    float girl_fine;
    float guy_fine;

    std::string *daily_report;
    std::string *weekly_report;
    std::string *settings;

    std::string *student_names;
    std::string *student_card_numbers;
    float       *student_req_hours;
    float       *student_deductions;
    float       *student_cmplt_hours;

    bool        is_active;

    static const std::string weekly_report_header[21];

    void initialize_db_controller();

    void fetch_settings();

    void load_base_time(db_gender gen);
    void save_base_time(db_gender gen);

    const char *compute_db(db_gender gen, database db);
    void compute_report_output(db_gender gen, report_type type);

    void fill_string_array(db_gender gen, database db);
    void fill_float_array(db_gender gen, database db);

    void write_to_file(db_gender gen, database db);

    void populate_student_information();

    void populate_student_names(db_gender gen);
    void populate_student_card_numbers(db_gender gen);
    void populate_student_required_hours(db_gender gen);
    void populate_student_completed_hours(db_gender gen);
    void populate_student_deductions(db_gender gen);

    void sort_db(DB_Sort::sort_by type, db_gender gen);

    void resize_db();

    void clock_out_all();
    void clock_out_from_idx(int i);
    void save_student_data();

    void clear_deductions();

    void write_report(const char *abs_file_path, std::string *report, int size);

};



#endif // DB_CONTROLLER_H

