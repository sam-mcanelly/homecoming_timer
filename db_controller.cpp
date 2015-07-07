#include "db_controller.h"

/*----------------------------------------------
 *
 *   ===========db_controller.cpp==========
 *
 *
 * Function definitions for the db_controller
 * class.
 *
 * Author: Sam McAnelly
 * Last Revised: 6/22/2015
 *
 *
 * ---------------------------------------------*/

/*----------------------------------------------
 *          LITERAL CONSTANTS
 * -------------------------------------------*/

#define FORMATTING_LINES        35

/*------------------------------------------------
 *          PUBLIC FUNCTION DEFINITIONS
 * -----------------------------------------------*/

DB_Controller::DB_Controller()
{

}

DB_Controller::~DB_Controller()
{
    save_student_data();

    delete[] ptr_db_students_male;
    delete[] ptr_db_students_female;
}

void DB_Controller::begin()
{
    initialize_db_controller();
}

void DB_Controller::set_gender(db_gender gender)
{
    switch(gender)
    {
    case GUYS:
        active_db      = ptr_db_students_male;
        active_count   = &male_student_count;
        active_idx     = &male_idx;
        break;
    case GIRLS:
        active_db = ptr_db_students_female;
        active_count   = &female_student_count;
        active_idx     = &female_idx;
        break;
    }
}

int DB_Controller::get_student_count()
{
    return *active_idx;
}

void DB_Controller::generate_report(report_type type)
{

}

void DB_Controller::add_student(std::string name, std::string card_num, float hours_req)
{
    if( *active_idx == *active_count )
    {
        resize_db();
        qDebug("> Database resized: [ new size ] - %d", *active_count);
    }
    active_db[ (*active_idx)++ ] = new Student(name, card_num, hours_req);
    qDebug("> New student created at: %d", ( *active_idx - 1 ) );


}

int DB_Controller::search_name(std::string name)
{
    //converting to lowercase
    for (std::string::iterator i = name.begin(); i != name.end(); i++)
    {
        *i = tolower(*i);
    }

    std::string name_from_array;
    for(int i = 0; i < *active_idx; i++)
    {
        name_from_array = active_db[i]->get_name();
        for (std::string::iterator i = name_from_array.begin(); i != name_from_array.end(); i++)
        {
            *i = tolower(*i);
        }

        if (name.compare(name_from_array) == 0)
            return i;
    }

    return -1;
}

int DB_Controller::search_card_number(std::string number)
{
    std::string num_from_array;
    for (int i = 0; i < *active_idx; i++)
    {
        num_from_array = num_from_array = active_db[i]->get_card_number();
        if(number.compare(num_from_array) == 0)
            return i;
    }

    return -1;
}

std::string DB_Controller::get_name_from_index(int i)
{
    return active_db[i]->get_name();
}

std::string DB_Controller::get_card_num_from_index(int i)
{
    return active_db[i]->get_card_number();
}

float DB_Controller::get_hours_req_from_index(int i)
{
    return active_db[i]->get_hours_required();
}

float DB_Controller::get_hours_comp_from_index(int i)
{
    return active_db[i]->get_hours_complete();
}

bool DB_Controller::get_status_from_index(int i)
{
    return active_db[i]->get_status();
}

void DB_Controller::toggle_status_from_index(int i)
{
    active_db[i]->toggle_status();
}

void DB_Controller::sort(DB_Sort::sort_by sortby)
{
    DB_Sort::sort(active_db, *active_idx , sortby);
}

/*------------------------------------------------
 *          PRIVATE FUNCTION DEFINITIONS
 * -----------------------------------------------*/

void DB_Controller::initialize_db_controller()
{
    fetch_settings();
    populate_student_information();
}

void DB_Controller::save_student_data()
{

}

void DB_Controller::fetch_settings()
{
    std::ifstream reader;
    reader.open("settings.hmt");

    std::string curr_line;
    int i = 0;

    while(std::getline(reader, curr_line))
    {
        switch(i)
        {
        case 0:
            guy_db = curr_line;
            break;
        case 1:
            girl_db = curr_line;
            break;
        case 2:
            guy_fine = ::atof(curr_line.c_str());
            break;
        case 3:
            girl_fine = ::atof(curr_line.c_str());
            break;
        case 4:
            report_path = curr_line;
            break;
        }
        ++i;
    }

}

const char * DB_Controller::compute_db(db_gender gender, database db)
{   std::string file_name;
    std::string gen_folder;
    std::string sub_folder = "";

    if (gender == GUYS)
        gen_folder = guy_db;
    else
        gen_folder = girl_db;
    switch(db)
    {
        case (NAME):
        {
            file_name = "db_name";
            break;
        }

        case (CWID):
        {
            file_name = "db_cwid";
            break;
        }

        case (TIME_COMPLETE):
        {
            sub_folder = "current/";
            file_name = "db_time_complete";
            break;
        }

        case (TIME_REQUIRED):
        {
            file_name = "db_time_required";
            if (gender == GIRLS)
                sub_folder = "current/";
            break;
        }

        default:
        {
            return NULL;
            break;
        }

    }

    qDebug("%s", (gen_folder + sub_folder + file_name).c_str());
    return (gen_folder + sub_folder + file_name).c_str();
}

void DB_Controller::compute_report_output(db_gender gender, report_type type)
{

}

void DB_Controller::fill_string_array(db_gender gender, database db)
{
    std::ifstream reader;

    const char* fname = compute_db(gender, db);
    reader.open(fname);

    std::string curr_line;
    int i = 0;

    while(std::getline(reader, curr_line))
    {
        switch(db)
        {
        case NAME:
            student_names[i] = curr_line;
            break;
        case CWID:
            student_card_numbers[i] = curr_line;
            break;
        }
        ++i;
    }
    reader.close();
    if (db == NAME)
    {
        switch(gender)
        {
        case GUYS:
            male_student_count = i;
            male_idx           = i;
            break;
        case GIRLS:
            female_student_count = i;
            female_idx           = i;
            break;
        }
    }
}

void DB_Controller::fill_float_array(db_gender gender, database db)
{
    std::ifstream reader;
    const char* fname = compute_db(gender, db);
    reader.open(fname);


    std::string curr_line;
    int i = 0;
    float hrs;

    while(std::getline(reader, curr_line))
    {
        hrs = ::atof(curr_line.c_str());
        switch(db)
        {
        case TIME_COMPLETE:
            student_cmplt_hours[i] = hrs;
            break;
        case TIME_REQUIRED:
            student_req_hours[i] = hrs;
            break;
        }

        ++i;
    }
    reader.close();
}


void DB_Controller::populate_student_information()
{
    student_names = new std::string[256000];
    fill_string_array(GUYS, NAME);

    student_card_numbers = new std::string[male_student_count];
    fill_string_array(GUYS, CWID);

    student_req_hours = new float[male_student_count];
    fill_float_array(GUYS, TIME_REQUIRED);

    student_cmplt_hours = new float[male_student_count];
    fill_float_array(GUYS, TIME_COMPLETE);

    ptr_db_students_male = new Student*[male_student_count];
    active_db = ptr_db_students_male;

    DB_Controller::populate_student_names(GUYS);
    DB_Controller::populate_student_card_numbers(GUYS);
    DB_Controller::populate_student_required_hours(GUYS);
    DB_Controller::populate_student_completed_hours(GUYS);

    delete[] student_names;
    delete[] student_card_numbers;
    delete[] student_req_hours;
    delete[] student_cmplt_hours;

    student_names = new std::string[256000];
    fill_string_array(GIRLS, NAME);

    student_card_numbers = new std::string[female_student_count];
    fill_string_array(GIRLS, CWID);

    student_req_hours = new float[female_student_count];
    fill_float_array(GIRLS, TIME_REQUIRED);

    student_cmplt_hours = new float[female_student_count];
    fill_float_array(GIRLS, TIME_COMPLETE);

    ptr_db_students_female = new Student*[female_student_count];
    active_db = ptr_db_students_female;

    DB_Controller::populate_student_names(GIRLS);
    DB_Controller::populate_student_card_numbers(GIRLS);
    DB_Controller::populate_student_required_hours(GIRLS);
    DB_Controller::populate_student_completed_hours(GIRLS);

    delete[] student_names;
    delete[] student_card_numbers;
    delete[] student_req_hours;
    delete[] student_cmplt_hours;
}


void DB_Controller::populate_student_names(db_gender gender)
{
    int count;
    switch(gender)
    {
    case GUYS:
        count = male_student_count;
        break;
    case GIRLS:
        count = female_student_count;
        break;
    }

    for(int i = 0; i < count; i++)
    {
        active_db[i] = new Student(student_names[i]);
    }
}

void DB_Controller::populate_student_card_numbers(db_gender gender)
{
    int count;
    switch(gender)
    {
    case GUYS:
        count = male_student_count;
        break;
    case GIRLS:
        count = female_student_count;
        break;
    }
    for(int i = 0; i < count; i++)
    {
        active_db[i]->set_card_number(student_card_numbers[i]);
    }
}

void DB_Controller::populate_student_required_hours(db_gender gender)
{
    int count;
    switch(gender)
    {
    case GUYS:
        count = male_student_count;
        break;
    case GIRLS:
        count = female_student_count;
        break;
    }
    for(int i = 0; i < count; i++)
    {
        active_db[i]->set_hours_required(student_req_hours[i]);
    }
}

void DB_Controller::populate_student_completed_hours(db_gender gender)
{
    int count;
    switch(gender)
    {
    case GUYS:
        count = male_student_count;
        break;
    case GIRLS:
        count = female_student_count;
        break;
    }

    for(int i = 0; i < count; i++)
    {
        active_db[i]->set_hours_complete(student_cmplt_hours[i]);
    }
}

void DB_Controller::resize_db()
{
    int idx;

    Student **ptr_temp = new Student*[*active_idx + 50];

    for( idx = 0; idx < *active_idx; idx++ )
    {
        ptr_temp[ idx ] = active_db[ idx ];
    }

    if( active_db == ptr_db_students_male)
    {
        //delete ptr_db_students_male;
        ptr_db_students_male = ptr_temp;
        active_db = ptr_db_students_male;
    }
    else if( active_db == ptr_db_students_female)
    {
        //delete ptr_db_students_female;
        ptr_db_students_female = ptr_temp;
        active_db = ptr_db_students_female;
    }
    *active_count += 50;
    qDebug("Active Index: %d", *active_idx);
    //*active_idx++;
}

void DB_Controller::generate_daily_report()
{
    daily_report = new std::string[*active_idx + FORMATTING_LINES];
}

void DB_Controller::generate_weekly_report()
{

}


void DB_Controller::write_daily_report(const char *abs_file_path)
{

}






