#include "student.h"
#include <string>

using namespace std;

/*----------------------------------------------
 *
 *      ===========student.cpp==========
 *
 *
 * Function definitions for the student class
 *
 * Author: Sam McAnelly
 * Last Revised: 5/21/2015
 *
 *
 * ---------------------------------------------*/

Student::Student()
{
    card_number      = "";
    hours_required   =  0;
    hours_complete   =  0;
    hours_deducted   =  0;
    hours_day_total  =  0;
    names_computed = false;
}

Student::Student(std::string _name)
{
    name             = _name;
    names_computed   = false;
    card_number      = "";
    hours_required   =  0;
    hours_complete   =  0;
    hours_deducted   =  0;
    hours_day_total  =  0;

    //storing the first name and last name for sorting purposes
    compute_names();

    //set to "clocked out" by default
    status = false;
}

Student::Student(std::string _name, QString _card_num, float _hours_req)
{
    name             = _name;
    names_computed   = false;
    card_number      = _card_num;
    hours_required   = _hours_req;
    hours_complete   =  0;
    hours_deducted   =  0;
    hours_day_total  =  0;

    compute_names();

    status = false;
}

Student::~Student()
{

}

Student& Student::operator =(const Student &old_stud)
{
    this->name             = old_stud.get_name();
    this->card_number      = QString::fromStdString(old_stud.get_card_number());
    this->hours_complete   = old_stud.get_base_hours_complete();
    this->hours_required   = old_stud.get_hours_required();
    this->hours_deducted   = old_stud.get_hours_deducted();
    this->hours_day_total  = old_stud.get_hours_day_total();
    this->timer            = old_stud.get_timer();
    this->status           = old_stud.get_status();
    this->names_computed   = old_stud.names_computed;
    this->first_name       = old_stud.get_first_name();
    this->last_name        = old_stud.get_last_name();

    return *this;
}

void Student::compute_names()
{
    char            f_name[50];
    char            l_name[50];
    const char * temp = name.c_str();
    int count = 0;
    int start_of_l_name;

    while (*temp != ' ')
    {
        count++;
        temp++;
    }

    strncpy(f_name, name.c_str(), count);
    f_name[count] = '\0';

    temp++;
    start_of_l_name = count;
    count = 0;

    while (count < name.length())
    {
        count++;
        temp++;
    }

    memcpy(l_name, &name[start_of_l_name], count);
    l_name[count] = '\0';

    std::string wow(f_name);
    std::string doge(l_name);
    first_name = wow;
    last_name  = doge;

 }

float Student::get_hours_required() const
{
    return hours_required - hours_deducted;
}

void Student::set_hours_required(float hrs)
{
    hours_required = hrs;
}

float Student::get_base_hours() const
{
    return hours_required;
}

float Student::get_deductions() const
{
    return hours_deducted;
}

void Student::set_deductions(float deductions)
{
    hours_deducted = deductions;
}

void Student::set_hours_day_total(float total)
{
    hours_day_total = total;
}

bool Student::get_status() const
{
    return status;
}

void Student::set_status(bool new_status)
{
    status = new_status;
}

void Student::toggle_status()
{
    if(status)
    {
        clock_out();
    }
    else
    {
        clock_in();
    }

    status = !status;
}

float Student::get_hours_complete()
{
    if( !status )
    {
        return hours_complete + hours_day_total;
    }
    else
    {
        //float time_done = ((timer.elapsed() / 1000.0) / 60.0) / 60.0;
        //qDebug("> Incrementing hours by %f", time_done);
        //increment_hours_complete(time_done);
        //timer.restart();

        return hours_complete + hours_day_total;
    }
}

float Student::get_base_hours_complete() const
{
    return hours_complete;
}

float Student::get_hours_deducted() const
{
    return hours_deducted;
}

float Student::get_hours_day_total() const
{
    return hours_day_total;
}

void Student::set_hours_complete(float new_hours_complete)
{
    hours_complete = new_hours_complete;
    hours_day_total  =  0;
}

void Student::increment_hours_complete(float addition)
{
    hours_day_total += addition;
}

float Student::get_hours_incomplete() const
{
    return get_hours_required() - (hours_complete + hours_day_total);
}

std::string Student::get_name() const
{
    return name;
}

const std::string Student::get_first_name() const
{
    const std::string temp = first_name;
    return temp;
}

const std::string Student::get_last_name() const
{
    const std::string temp = last_name;
    return temp;
}

void Student::set_name(std::string new_name)
{
    name = new_name;
    compute_names();
}

std::string Student::get_card_number() const
{
    return card_number.toStdString();
}

void Student::set_card_number(QString new_card_number)
{
    qDebug("> setting card number: %s", new_card_number.toStdString().c_str());
    card_number = new_card_number;
    qDebug("> card number set");
}

QTime Student::get_timer() const
{
    return timer;
}

void Student::clock_in()
{
    qDebug("> %s clocked in", name.c_str());
    timer.restart();
}

void Student::clock_out()
{
    if(!status)
        return;

    qDebug("> %s clocked out", name.c_str());

    float time_done = ((timer.elapsed() / 1000.0) / 60.0) / 60.0;

    //Code for testing
    std::ostringstream ss;
    ss << time_done;
    std::string s(ss.str());
    qDebug(s.c_str());

    increment_hours_complete(time_done);
}

void Student::output_debug_info()
{
    qDebug("+=========================+");
    qDebug("+      Student Info       +");
    qDebug("+=========================+");
    qDebug("student name: %s", name.c_str());
    qDebug("student card number: %s", card_number.toStdString().c_str());
    qDebug("student status: %s", status ? "true" : "false");
    qDebug("student required hours: %f", hours_required);
    qDebug("student complete hours: %f", hours_complete);
    qDebug("+=========================+");
    qDebug("");


}

