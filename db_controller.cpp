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
 * Last Revised: 5/21/2015
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
        active_db = ptr_db_students_male;
        break;
    case GIRLS:
        active_db = ptr_db_students_female;
        break;
    default:
        qDebug("Invalid database gender selection...");
    }
}

int DB_Controller::get_student_count()
{
    if (active_db == ptr_db_students_male)
        return male_student_count;
    else
        return female_student_count;
}
void DB_Controller::set_student_count(int count)
{
    if (active_db == ptr_db_students_male)
        male_student_count = count;
    else
        female_student_count = count;
}

void DB_Controller::generate_report(report_type type)
{

}

int DB_Controller::search_name(std::string name)
{
    //converting to lowercase
    for (std::string::iterator i = name.begin(); i != name.end(); i++)
    {
        *i = tolower(*i);
    }

    std::string name_from_array;

    int count;
    if (active_db == ptr_db_students_male)
        count = male_student_count;
    else
        count = female_student_count;

    for(int i = 0; i < count; i++)
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
    int count;
    if (active_db == ptr_db_students_male)
        count = male_student_count;
    else
        count = female_student_count;
    std::string num_from_array;
    for (int i = 0; i < count; i++)
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
    int count;
    switch (sortby)
    {
    case DB_Sort::F_NAME:
        break;
    case DB_Sort::L_NAME:
        break;
    case DB_Sort::STATUS:
        break;
    case DB_Sort::TIME_COMPLETE:
        break;
    case DB_Sort::TIME_REQUIRED:
        if(active_db == ptr_db_students_male)
            count = male_student_count;
        else
            count = female_student_count;
        DB_Sort::sort(active_db, count, DB_Sort::TIME_REQUIRED);
        break;
    default:
        return;
    }
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

}

const char * DB_Controller::compute_db(db_gender gender, database db)
{
    //computes the current path
    //the database should always be kept in the same working directory as the application
    QDir dir = QDir::currentPath();
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    QString path = dir.absolutePath();
    std::string utf8_path = path.toUtf8().constData();
    std::string file_name;
    std::string gen_folder;
    std::string sub_folder = "";
    if (gender == GUYS)
        gen_folder = "/ato/";
    else
        gen_folder = "/adpi/";
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
                sub_folder = "/current/";
            break;
        }

        default:
        {
            return NULL;
            break;
        }

    }

    return (utf8_path + HOCO_DB + gen_folder + sub_folder + file_name).c_str();
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
            break;
        case GIRLS:
            female_student_count = i;
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
    int *count;
    if( active_db == ptr_db_students_male)
        count = &male_student_count;
    else
        count = &female_student_count;

    Student **ptr_temp = new Student*[*count + 50];

    for(int i = 0; i < *count; i++)
    {
        ptr_temp[i] = active_db[i];
    }

    active_db = ptr_temp;
    *count += 50;
}

void DB_Controller::generate_daily_report()
{
    int count;
    if (active_db == ptr_db_students_male)
        count = male_student_count;
    else
        count = female_student_count;
    daily_report = new std::string[count + FORMATTING_LINES];
}

void DB_Controller::generate_weekly_report()
{

}


void DB_Controller::write_daily_report(const char *abs_file_path)
{

}






