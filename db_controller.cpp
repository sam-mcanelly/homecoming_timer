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

/*------------------------------------------------
 *          PUBLIC FUNCTION DEFINITIONS
 * -----------------------------------------------*/

DB_Controller::DB_Controller(db_gender gen, Student **student_array)
{
    initialize_db_controller(gen, student_array);
}

DB_Controller::DB_Controller()
{

}

DB_Controller::~DB_Controller()
{

    /* IS THIS NEEDED???? */
    /*
    if(daily_report != NULL)
        delete daily_report;
    if(weekly_report != NULL)
        delete weekly_report;
    if(settings != NULL)
        delete settings;

    if(student_names != NULL)
        delete student_names;
    if(student_card_numbers != NULL)
        delete student_card_numbers;
    if(student_req_hours != NULL)
        delete student_req_hours;
    if(student_cmplt_hours != NULL)
        delete student_cmplt_hours;
     */

    save_student_data();

    delete ptr_db_students;
}

void DB_Controller::load_data()
{

    initialize_db_controller(gender, ptr_db_students);
}

void DB_Controller::set_gender(db_gender gen)
{
    gender = gen;
}

void DB_Controller::set_student_array(Student **student_array)
{
    ptr_db_students = student_array;
}


int DB_Controller::get_student_count()
{
    return student_count;
}
void DB_Controller::set_student_count(int count)
{
    student_count = count;
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

    for(int i = 0; i < student_count; i++)
    {
        name_from_array = ptr_db_students[i]->get_name();
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
    for (int i = 0; i < student_count; i++)
    {
        num_from_array = num_from_array = ptr_db_students[i]->get_card_number();
        if(number.compare(num_from_array) == 0)
            return i;
    }

    return -1;
}

void DB_Controller::sort(DB_Sort::sort_by sortby)
{
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
        DB_Sort::sort(ptr_db_students, student_count, DB_Sort::TIME_REQUIRED);
        break;
    default:
        return;
    }
}

/*------------------------------------------------
 *          PRIVATE FUNCTION DEFINITIONS
 * -----------------------------------------------*/

void DB_Controller::initialize_db_controller(db_gender gen, Student **students)
{
    gender = gen;
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

void DB_Controller::fill_string_array(database db)
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
    student_count = i;
}
void DB_Controller::fill_float_array(database db)
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
    student_count = i;
}


void DB_Controller::populate_student_information()
{
    student_names = new std::string[256000];
    fill_string_array(NAME);

    student_card_numbers = new std::string[student_count];
    fill_string_array(CWID);

    student_req_hours = new float[student_count];
    fill_float_array(TIME_REQUIRED);

    student_cmplt_hours = new float[student_count];
    fill_float_array(TIME_COMPLETE);

    DB_Controller::populate_student_names();
    DB_Controller::populate_student_card_numbers();
    DB_Controller::populate_student_required_hours();
    DB_Controller::populate_student_completed_hours();
}


void DB_Controller::populate_student_names()
{
    ptr_db_students = new Student*[student_count];
    for(int i = 0; i < student_count; i++)
    {
        ptr_db_students[i] = new Student(student_names[i]);
    }
}

void DB_Controller::populate_student_card_numbers()
{
    for(int i = 0; i < student_count; i++)
    {
        ptr_db_students[i]->set_card_number(student_card_numbers[i]);
    }
}

void DB_Controller::populate_student_required_hours()
{
    for(int i = 0; i < student_count; i++)
    {
        ptr_db_students[i]->set_hours_required(student_req_hours[i]);
    }
}

void DB_Controller::populate_student_completed_hours()
{
    for(int i = 0; i < student_count; i++)
    {
        ptr_db_students[i]->set_hours_complete(student_cmplt_hours[i]);
    }
}


void DB_Controller::generate_daily_report()
{
    //The extra 35 holds the report formatting
    daily_report = new std::string[student_count + 35];
}

void DB_Controller::generate_weekly_report()
{

}


void DB_Controller::write_daily_report(const char *abs_file_path)
{

}






