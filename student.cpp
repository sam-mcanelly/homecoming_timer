#include "student.h"

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

}

Student::Student(std::string _name)
{
    name = _name;

    //storing the first name and last name for sorting purposes
    compute_names();

    //set to "clocked out" by default
    status = false;
}

Student::Student(std::string _name, std::string _card_num, float _hours_req)
{
    name             = _name;
    card_number      = _card_num;
    hours_required   = _hours_req;
    hours_complete   =  0;

    compute_names();

    status = false;

}

Student& Student::operator =(const Student &old_stud)
{
    this->name = old_stud.get_name();
    this->card_number = old_stud.get_card_number();
    this->hours_complete = old_stud.get_hours_complete();
    this->hours_required = old_stud.get_hours_required();
    this->status = old_stud.get_status();

    //ADD CODE TO GET TIME CLOCKED IN

    //Copy over first name
    int i = 0;
    const char *name = old_stud.get_first_name();
    int j = std::strlen(name);
    f_name = new char[j];

    while (i < j)
    {
        f_name[i] = name[i];
        ++i;
    }

    //Copy over last name
    name = old_stud.get_last_name();
    j = std::strlen(name);
    l_name = new char[j];

    while (i < j)
    {
        l_name[i] = name[i];
        ++i;
    }

    return *this;
}

void Student::compute_names()
{
    const char * temp = name.c_str();
    int count = 0;
    int start_of_l_name;
    while (*temp != ' ')
    {
        count++;
        temp++;
    }

    f_name = new char[++count];
    std::strncpy(f_name, name.c_str(), count);
    f_name[count] = '\0';

    temp++;
    start_of_l_name = count;
    count = 0;

    while (count < name.length())
    {
        count++;
        temp++;
    }

    l_name = new char[++count];
    memcpy(l_name, &name[start_of_l_name], count);
    l_name[count] = '\0';
 }

float Student::get_hours_required() const
{
    return hours_required;
}

void Student::set_hours_required(float hrs)
{
    hours_required = hrs;
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
    status = !status;
    if(!status)
        clock_out();
    else
        clock_in();
}

float Student::get_hours_complete() const
{
    return hours_complete;
}

void Student::set_hours_complete(float new_hours_complete)
{
    hours_complete = new_hours_complete;
}

void Student::increment_hours_complete(float addition)
{
    hours_complete += addition;
}

std::string Student::get_name() const
{
    return name;
}

const char * Student::get_first_name() const
{
    const char *temp = f_name;
    return temp;
}

const char * Student::get_last_name() const
{
    const char *temp = l_name;
    return temp;
}

void Student::set_name(std::string new_name)
{
    name = new_name;
    compute_names();
}

std::string Student::get_card_number() const
{
    return card_number;
}

void Student::set_card_number(std::string new_card_number)
{
    card_number = new_card_number;
}

void Student::clock_in()
{
    status = true;
    timer.start();
}

void Student::clock_out()
{
    if(!status)
        return;

    float time_done = ((timer.elapsed() / 1000.0) / 60.0) / 60.0;

    //Code for testing
    //std::ostringstream ss;
    //ss << time_done;
    //std::string s(ss.str());
    //qDebug(s.c_str());

    increment_hours_complete(time_done);

    status = false;
}

void Student::output_debug_info()
{
    qDebug("+=========================+");
    qDebug("+      Student Info       +");
    qDebug("+=========================+");
    qDebug("student name: %s", name.c_str());
    qDebug("student card number: %s", card_number.c_str());
    qDebug("student status: %s", status ? "true" : "false");
    qDebug("student required hours: %f", hours_required);
    qDebug("student complete hours: %f", hours_complete);
    qDebug("+=========================+");
    qDebug("");


}

