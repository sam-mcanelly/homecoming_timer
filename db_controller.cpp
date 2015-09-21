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

#define FORMATTING_LINES            20
#define REPORT_HEADER_END_LENGTH    42
#define REPORT_STUDENT_NAME_LENGTH  22
#define REPORT_STUDENT_HOURS_LENGTH 18
#define REPORT_STUDENT_FINE_LENGTH  19
#define MAX_FLOAT_VAL               100

const std::string DB_Controller::weekly_report_header[] ={ "+==================================================================+",
                                                           "||                 Homecoming Time Weekly Report                  ||",
                                                           "||      =====================================================     ||",
                                                           "||                                                                ||",
                                                           "||                House:",
                                                           "||                 Date:",
                                                           "||        Fine Per Hour:",
                                                           "||                                                                ||",
                                                           "||       =====================================================    ||",
                                                           "||                                                                ||",
                                                           "||              Copyright © 2015 Sam McAnelly                     ||",
                                                           "||                                                                ||",
                                                           "+==================================================================+",
                                                           "                                                                    ",
                                                           "+==================================================================+",
                                                           "+                      Student Time Sheet                          +",
                                                           "+    =======================================================       +",
                                                           "+ Name                  || Hours             || Fine Amount        +",
                                                           "+##################################################################+",
                                                           "+                       ||                   ||                    +",
                                                           "+==================================================================+" };

/*------------------------------------------------
 *          PUBLIC FUNCTION DEFINITIONS
 * -----------------------------------------------*/

DB_Controller::DB_Controller()
{
    is_active = false;
}

DB_Controller::~DB_Controller()
{

}

void DB_Controller::begin()
{
    qDebug("> Booting up controller...");
    initialize_db_controller();
    is_active = true;
}

void DB_Controller::end()
{
    qDebug("> Shutting down controller...");
    clock_out_all();
    save_student_data();
    qDebug("> Controller shutdown");
}

void DB_Controller::set_gender(db_gender gen)
{
    switch(gen)
    {
    case GUYS:
        active_db      = ptr_db_students_male;
        active_count   = &male_student_count;
        active_idx     = &male_idx;
        break;
    case GIRLS:
        active_db      = ptr_db_students_female;
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

void DB_Controller::add_student(std::string name, QString card_num, float hours_req)
{
    if( *active_idx == 499 )
    {
        return;
    }
    qDebug("> FUCKING ACTIVE_IDX BITCH: %d", *active_idx);
    active_db[ (*active_idx) ].set_name(name);
    active_db[ (*active_idx) ].set_card_number(card_num);
    active_db[ (*active_idx) ].set_deductions(0.0);
    active_db[ (*active_idx) ].set_hours_complete(0.0);
    active_db[ (*active_idx) ].set_status(false);
    active_db[ (*active_idx)++ ].set_hours_required(hours_req);
    qDebug("> New student created at: %d", ( *active_idx - 1 ) );

    sort(DB_Sort::L_NAME);
}

void DB_Controller::delete_student(int index)
{
    int idx;

    for( idx = index; idx < ( *active_idx - 1 ); idx++ )
    {
        active_db[ idx ] = active_db[ idx + 1 ];
    }

    (*active_idx)--;
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
        name_from_array = active_db[i].get_name();
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
        num_from_array = num_from_array = active_db[i].get_card_number();
        if(number.compare(num_from_array) == 0)
            return i;
    }

    return -1;
}

std::string DB_Controller::get_name_from_index(int i)
{
    if( ( i > -1 ) && ( i < *active_idx ) )
    {
        return active_db[i].get_name();
    }
    else
    {
        return "";
    }
}

std::string DB_Controller::get_card_num_from_index(int i)
{
    if( i > -1 && i < *active_idx)
    {
        return active_db[i].get_card_number();
    }
    else
    {
        return "";
    }
}

float DB_Controller::get_hours_req_from_index(int i)
{
    if( i > -1 && i < *active_idx)
    {
        return active_db[i].get_hours_required();
    }
    else
    {
        return -1.0f;
    }
}

float DB_Controller::get_hours_comp_from_index(int i)
{
    if( i > -1 && i < *active_idx)
    {
        return active_db[i].get_hours_complete();
    }
    else
    {
        return -1.0f;
    }
}

float DB_Controller::get_hours_deducted_from_index(int i)
{
    if( i > -1 && i < *active_idx)
    {
        return active_db[i].get_hours_complete();
    }
    else
    {
        return -1.0f;
    }
}

void DB_Controller::set_hours_deducted_from_index(int i, float deductions)
{
    if( i > -1 && i < *active_idx)
    {
        active_db[i].set_deductions(deductions);
    }
}

bool DB_Controller::get_status_from_index(int i)
{
    if( i > -1 && i < *active_idx)
    {
        return active_db[i].get_status();
    }
    else
    {
        return false;
    }
}

void DB_Controller::toggle_status_from_index(int i)
{
    if( i > -1 && i < *active_idx)
    {
        active_db[ i ].toggle_status();
    }
}

void DB_Controller::sort(DB_Sort::sort_by sortby)
{
    DB_Sort::sort(active_db, *active_idx , sortby);
}

void DB_Controller::generate_daily_report(db_gender gen)
{
    daily_report = new std::string[ *active_idx + FORMATTING_LINES ];
}

void DB_Controller::generate_weekly_report(db_gender gen)
{
    std::ofstream     writer;
    std::string       output_file;
    std::string       *weekly_report;
    std::string       date;
    std::string       curr_name;
    std::string       time_string;
    std::string       house_name;
    QString           curr_time_complete;
    QString           curr_time_required;
    QString           fine_string;
    QString           fine;
    QString           begin_date;
    QString           end_date;
    QDate             end;
    int               idx;
    int               stud_idx;
    int               report_size;
    float             fine_amnt;

    end                = QDate::currentDate();
    end_date           = QDate::currentDate().toString();
    begin_date         = end.addDays( (qint64)(-7) ).toString();
    output_file        = report_path + ( ( gen == GUYS ) ? frat_name : sor_name ) + "\\" + ( ( gen == GUYS ) ? frat_name : sor_name ) + "_week_" + "_" + begin_date.toStdString() + "_to_" + end_date.toStdString();
    date               = begin_date.toStdString() + " - " + end_date.toStdString();
    fine               = ( gen == GUYS ) ? QString::number(guy_fine) : QString::number(girl_fine);
    stud_idx           = 0;
    curr_name          = "";
    house_name         = ( gen == GUYS ) ? frat_name : sor_name;
    writer.open( output_file.c_str() );

    qDebug("> Output file: %s", output_file.c_str());
    report_size = ( ( gen == GUYS ) ? male_idx : female_idx ) + FORMATTING_LINES;
    weekly_report = new std::string[ report_size ];

    weekly_report[0] = weekly_report_header[0];
    weekly_report[1] = weekly_report_header[1];
    weekly_report[2] = weekly_report_header[2];
    weekly_report[3] = weekly_report_header[3];

    weekly_report[4] = weekly_report_header[4] + house_name;
    for( idx = 0; idx < REPORT_HEADER_END_LENGTH - house_name.length(); idx++ )
    {
        weekly_report[4] = weekly_report[4] + " ";
    }
    weekly_report[4] = weekly_report[4] + "||";

    weekly_report[5] = weekly_report_header[5] + date;
    for( idx = 0; idx < REPORT_HEADER_END_LENGTH - date.length(); idx++ )
    {
        weekly_report[5] = weekly_report[5] + " ";
    }
    weekly_report[5] = weekly_report[5] + "||";

    weekly_report[6] = weekly_report_header[6] + fine.toStdString();
    for( idx = 0; idx < REPORT_HEADER_END_LENGTH - fine.length(); idx++ )
    {
        weekly_report[6] = weekly_report[6] + " ";
    }
    weekly_report[6] = weekly_report[6] + "||";

    for( idx = 7; idx < 19; idx++ )
    {
        weekly_report[idx] = weekly_report_header[idx];
    }

    //Fill in the plus sign at the beginning of the line
    for( idx = 19; idx < ( report_size - 1 ); idx++ )
    {
        weekly_report[idx] = "+ ";
    }

    //Fill in the names of the students
    for( idx = 19; idx < ( report_size - 1 ); idx++ )
    {
        curr_name = ( gen == GUYS ) ? ptr_db_students_male[ stud_idx++ ].get_name() : ptr_db_students_female[ stud_idx++ ].get_name();
        weekly_report[idx] += curr_name;

        //Add the space formatting
        for( int i = 0; i < (REPORT_STUDENT_NAME_LENGTH - curr_name.length()); i++)
        {
            weekly_report[idx] += " ";
        }

        weekly_report[idx] += "|| ";
    }

    stud_idx = 0;

    //Fill in the hours of the students
    for( idx = 19; idx < ( report_size - 1 ); idx++ )
    {
        if( gen == GUYS )
        {
            curr_time_complete.sprintf("%5.2f", ptr_db_students_male[ stud_idx ].get_hours_complete());
            curr_time_required.sprintf("%5.2f", ptr_db_students_male[ stud_idx ].get_hours_required());
        }
        else
        {
            curr_time_complete.sprintf("%5.2f", ptr_db_students_female[ stud_idx ].get_hours_complete());
            curr_time_required.sprintf("%5.2f", ptr_db_students_female[ stud_idx ].get_hours_required());
        }

        stud_idx++;
        time_string = curr_time_complete.toStdString() + "/" + curr_time_required.toStdString();
        weekly_report[ idx ] += time_string;

        //Add the space formatting
        for( int i = 0; i < (REPORT_STUDENT_HOURS_LENGTH - time_string.length()); i++)
        {
            weekly_report[idx] += " ";
        }

        weekly_report[idx] += "|| ";
    }

    stud_idx = 0;

    //Fill in the fines of the students
    for( idx = 19; idx < ( report_size - 1 ); idx++ )
    {
        if( gen == GUYS )
        {
            fine_amnt = ptr_db_students_male[ stud_idx++ ].get_hours_incomplete() * guy_fine;
        }
        else
        {
            fine_amnt = ptr_db_students_female[ stud_idx++ ].get_hours_incomplete() * girl_fine;
        }

        if( fine_amnt <= 0 )
        {
            fine_string.sprintf("$0.00");
        }
        else
        {
            fine_string.sprintf("$%5.2f", fine_amnt);
        }

        weekly_report[ idx ] += fine_string.toStdString();

        //Add the space formatting
        for( int i = 0; i < (REPORT_STUDENT_FINE_LENGTH - fine_string.length()); i++)
        {
            weekly_report[idx] += " ";
        }

        weekly_report[idx] += "+";
    }

    weekly_report[ report_size - 1 ] = weekly_report_header[ 20 ];

    for( idx = 0; idx < ( report_size ); idx++)
    {
        qDebug("%s", weekly_report[idx].c_str());
    }

    write_report(output_file.c_str(), weekly_report, report_size);

    clear_deductions();

    for( idx = 0; idx < female_idx; idx++)
    {
        ptr_db_students_female[idx].set_hours_complete( 0.0 );
    }

    for( idx = 0; idx < male_idx; idx++ )
    {
        ptr_db_students_male[idx].set_hours_complete( 0.0 );
    }
}

void DB_Controller::save()
{
    save_student_data();
}

/*------------------------------------------------
 *          PRIVATE FUNCTION DEFINITIONS
 * -----------------------------------------------*/

void DB_Controller::initialize_db_controller()
{
    bool successfully_loaded = false;
    qDebug("> Fetching settings");

    fetch_settings();

    qDebug("> Settings opened");
    qDebug("> Populating student information...");

    while( !successfully_loaded )
    {
        try
        {
            populate_student_information();
            successfully_loaded = true;
        }
        catch( int e )
        {
            qDebug("> failed loading students... trying again");
        }
    }

    qDebug("> Student information loaded");
    qDebug("> Controller booted up");
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
            //qDebug("> guy database: %s", guy_db.c_str());
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
        case 5:
            frat_name = curr_line;
            break;
        case 6:
            sor_name = curr_line;
        }
        ++i;
    }

}

std::string DB_Controller::compute_db(db_gender gen, database db)
{
    std::string file_name;
    std::string gen_folder;
    std::string sub_folder = "";

    if (GUYS == gen)
    {
        gen_folder = guy_db;
    }
    else
    {
        gen_folder = girl_db;
    }

    switch(db)
    {
        case (NAME):
            file_name = "db_name";
            break;

        case (CWID):
            file_name = "db_cwid";
            break;

        case (TIME_COMPLETE):
            file_name = "db_time_complete";
            break;

        case (TIME_REQUIRED):
            file_name = "db_time_required";
            break;

        case (TIME_DEDUCTED):
            file_name = "db_time_deducted";
            break;

        default:
            return NULL;
            break;

    }

    //qDebug("> Database path: %s", (gen_folder + sub_folder + file_name).c_str());
    return (gen_folder + sub_folder + file_name).c_str();
}

void DB_Controller::compute_report_output(db_gender gen, report_type type)
{

}

void DB_Controller::fill_string_array(db_gender gen, database db)
{
    std::ifstream reader;

    std::string fname = compute_db(gen, db);
    qDebug("> database location: %s", fname.c_str());
    reader.open(fname.c_str());

    std::string curr_line;
    int i = 0;

    while(std::getline(reader, curr_line))
    {
        if( i > 499 )
            break;
        switch(db)
        {
        case NAME:
            student_names[i] = curr_line;
            break;
        case CWID:
            student_card_numbers[i] = QString::fromStdString(curr_line);
            break;
        }
        ++i;
    }
    reader.close();

    if (db == NAME)
    {
        switch(gen)
        {
        case GUYS:
            male_student_count   = i;
            male_idx             = i;
            break;
        case GIRLS:
            female_student_count = i;
            female_idx           = i;
            break;
        }
    }
}

void DB_Controller::fill_float_array(db_gender gen, database db)
{
    std::ifstream reader;
    std::string fname = compute_db(gen, db);
    reader.open(fname.c_str());


    std::string curr_line;
    int i = 0;
    float hrs;

    while(std::getline(reader, curr_line))
    {
        if( i > 499 )
            break;

        hrs = ::atof(curr_line.c_str());

        if( ( hrs < 0 ) || ( hrs > MAX_FLOAT_VAL ) )
        {
            qDebug("> Error reading float value!");
            hrs = 0;
        }

        switch(db)
        {
        case TIME_COMPLETE:
            student_cmplt_hours[i] = hrs;
            break;
        case TIME_REQUIRED:
            student_req_hours[i] = hrs;
            break;
        case TIME_DEDUCTED:
            student_deductions[i] = hrs;
            break;
        }

        ++i;
    }
    reader.close();
}

void DB_Controller::write_to_file(db_gender gen, database db)
{
    if( !is_active )
        return;

    std::ofstream    writer;
    int              idx;
    Student          *db_write;
    int              count;


    std::string fname = compute_db(gen, db);

    switch(gen)
    {
    case GUYS:
        db_write = ptr_db_students_male;
        count = male_idx;
        if( count < 1 )
        {
            return;
        }
        writer.open(fname.c_str());
        break;
    case GIRLS:
        db_write = ptr_db_students_female;
        count = female_idx;
        if( count < 1 )
        {
            return;
        }
        writer.open(fname.c_str());
        break;
    }
    //qDebug("> set correct database and count for write");

    for(idx = 0; idx < count; idx++)
    {
        switch( db )
        {
        case NAME:
        {
            //qDebug("> writing name from index %d", idx);
            writer << db_write[ idx ].get_name().c_str() << "\n";
            break;
        }
        case CWID:
        {
            //qDebug("> writing cwid from index %d", idx);
            writer << db_write[ idx ].get_card_number() << "\n";
            break;
        }
        case TIME_COMPLETE:
        {
            //qDebug("> writing time complete from index %d", idx);
            writer << db_write[ idx ].get_hours_complete() << "\n";
            break;
        }
        case TIME_REQUIRED:
        {
            //qDebug("> writing time required from index %d", idx);
            writer << db_write[ idx ].get_base_hours() << "\n";
            break;
        }
        case TIME_DEDUCTED:
        {
            //qDebug("> writing time deducted from index %d", idx);
            writer << db_write[ idx ].get_deductions() << "\n";
            break;
        }
        case SETTINGS:
        {
            //qDebug("> settings write fail!");
            break;
        }
        default:
        {
        }
        }
    }

    //qDebug("> closing writer");
    writer.close();
    //qDebug("> writer closed");
}


void DB_Controller::populate_student_information()
{
    qDebug("> filling guys name array...");
    fill_string_array(GUYS, NAME);

    qDebug("> Guy count from file: %d", male_student_count);

    qDebug("> filling guys card number array...");
    fill_string_array(GUYS, CWID);

    qDebug("> filling guys required hours");
    fill_float_array(GUYS, TIME_REQUIRED);

    qDebug("> filling guys deductions");
    fill_float_array(GUYS, TIME_DEDUCTED);

    qDebug("> filling guys complete hours...");
    fill_float_array(GUYS, TIME_COMPLETE);

    qDebug("> creating male student array... size: %d", male_student_count);
    active_db = ptr_db_students_male;

    qDebug("> populating male student array...");
    populate_student_names(GUYS);
    qDebug("> male names added");
    populate_student_card_numbers(GUYS);
    qDebug("> male card numbers added");
    populate_student_required_hours(GUYS);
    qDebug("> male hours required added");
    populate_student_completed_hours(GUYS);
    qDebug("> male hours complete added");
    populate_student_deductions(GUYS);
    qDebug("> male deductions added");

    qDebug("> Male information populated");

    fill_string_array(GIRLS, NAME);
    fill_string_array(GIRLS, CWID);
    fill_float_array(GIRLS, TIME_REQUIRED);
    fill_float_array(GIRLS, TIME_DEDUCTED);
    fill_float_array(GIRLS, TIME_COMPLETE);

    active_db = ptr_db_students_female;

    populate_student_names(GIRLS);
    populate_student_card_numbers(GIRLS);
    populate_student_required_hours(GIRLS);
    populate_student_completed_hours(GIRLS);
    populate_student_deductions(GIRLS);

    qDebug("> Female information populated");
}


void DB_Controller::populate_student_names(db_gender gen)
{
    int count;
    Student *populate_db;
    switch(gen)
    {
    case GUYS:
        qDebug("> male count: %d", male_student_count);
        count = male_student_count;
        populate_db = ptr_db_students_male;
        break;
    case GIRLS:
        count = female_student_count;
        populate_db = ptr_db_students_female;
        break;
    }

    for(int i = 0; i < count; i++)
    {
        qDebug("> index: %d", i);
        active_db[i].set_name(student_names[i]);
    }
}

void DB_Controller::populate_student_card_numbers(db_gender gen)
{
    int count;
    Student *populate_db;
    switch(gen)
    {
    case GUYS:
        qDebug("> male count: %d", male_student_count);
        count = male_student_count;
        populate_db = ptr_db_students_male;
        break;
    case GIRLS:
        count = female_student_count;
        populate_db = ptr_db_students_female;
        break;
    }
    for(int i = 0; i < count; i++)
    {
        qDebug("> index: %d", i);
        active_db[i].set_card_number(student_card_numbers[i]);
    }
}

void DB_Controller::populate_student_required_hours(db_gender gen)
{
    int count;
    switch(gen)
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
        active_db[i].set_hours_day_total(0.0);
        active_db[i].set_status(false);
        active_db[i].set_hours_required(student_req_hours[i]);
    }
}

void DB_Controller::populate_student_completed_hours(db_gender gen)
{
    int count;
    switch(gen)
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
        active_db[i].set_hours_complete(student_cmplt_hours[i]);
    }
}

void DB_Controller::populate_student_deductions(db_gender gen)
{
    int count;
    switch(gen)
    {
    case GUYS:
        count = male_student_count;
        break;
    case GIRLS:
        count = female_student_count;
        break;
    }

    for( int i = 0; i < count; i++ )
    {
        active_db[i].set_deductions(student_deductions[i]);
    }
}

void DB_Controller::sort_db(DB_Sort::sort_by type, db_gender gen)
{
    Student**      db_to_sort;
    switch(gen)
    {
    case GUYS:
        DB_Sort::sort(ptr_db_students_male, male_idx, type);
        break;
    case GIRLS:
        DB_Sort::sort(ptr_db_students_female, female_idx, type);
        break;
    }
}

void DB_Controller::clock_out_all()
{
    if( !is_active )
        return;

    int idx;

    for( idx = 0; idx < male_idx ; idx++ )
    {
        ptr_db_students_male[ idx ].clock_out();
    }
    qDebug("> Male students clocked out");

    for( idx = 0; idx < female_idx ; idx++ )
    {
        ptr_db_students_female[ idx ].clock_in();
    }
    qDebug("> Female students clocked out");
}

void DB_Controller::save_student_data()
{
    sort_db(DB_Sort::L_NAME, DB_Controller::GUYS);
    sort_db(DB_Sort::L_NAME, DB_Controller::GIRLS);

    int   db;
    std::string info;

    for( db = NAME; db < DB_COUNT; db++ )
    {
        switch ( db )
        {
        case NAME:
            info = "name";
            break;
        case CWID:
            info = "card info";
            break;
        case TIME_COMPLETE:
            info = "hours complete";
            break;
        case TIME_REQUIRED:
            info = "hours required";
            break;
        case TIME_DEDUCTED:
            info = "hours deducted";
            break;
        default:
            info = "unknown data";
            break;
        }


        if( male_idx > 0 )
        {
            //qDebug("> Saving male %s data...", info.c_str());
            write_to_file( GUYS, (DB_Controller::database)db );
        }

        if( female_idx > 0 )
        {
            //qDebug("> Saving female %s data", info.c_str());
            write_to_file( GIRLS, (DB_Controller::database)db );
        }
    }
}



void DB_Controller::clear_deductions()
{
    int idx;

    for( idx = 0; idx < male_idx; idx++)
    {
        ptr_db_students_male[idx].set_deductions( 0 );
    }

    qDebug("> Male deductions cleared");

    for( idx = 0; idx < female_idx; idx++ )
    {
        ptr_db_students_female[idx].set_deductions( 0 );
    }

    qDebug("> Female deductions cleared");
}

void DB_Controller::write_report(const char *abs_file_path, std::string *report, int size)
{
    qDebug("> Writing report...");
    int            idx;
    std::ofstream  writer;

    writer.open(abs_file_path);

    for( idx = 0; idx < size; idx++ )
    {
        writer << report[idx] << "\n";
    }

    writer.close();
    qDebug("> Report written");
}







