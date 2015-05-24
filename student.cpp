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

Student::Student(std::string _name)
{
    name = _name;

    //set to "clocked out" by default
    status = false;
}

float Student::get_hours_required()
{
    return hours_required;
}

void Student::set_hours_required(float hrs)
{
    hours_required = hrs;
}

bool Student::get_status()
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

float Student::get_hours_complete()
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

std::string Student::get_name()
{
    return name;
}

void Student::set_name(std::string new_name)
{
    name = new_name;
}

std::string Student::get_card_number()
{
    return card_number;
}

void Student::set_card_number(std::string new_card_number)
{
    card_number = new_card_number;
}

void Student::clock_in()
{
    timer.start();
}

void Student::clock_out()
{

    float time_done = ((timer.elapsed() / 1000.0) / 60.0) / 60.0;

    //Code for testing
    //std::ostringstream ss;
    //ss << time_done;
    //std::string s(ss.str());
    //qDebug(s.c_str());

    increment_hours_complete(time_done);
}


