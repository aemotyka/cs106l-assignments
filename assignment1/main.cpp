/*
 * CS106L Assignment 1: EnrollmentNavigator
 * Created by Fabio Ibanez with modifications by Jacob Roberts-Baca.
 *
 * Welcome to Assignment 1 of CS106L! Please complete each STUDENT TODO
 * in this file. You do not need to modify any other files.
 *
 * Students must implement: parse_csv, write_courses_offered,
 * write_courses_not_offered
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::string COURSES_OFFERED_PATH = "student_output/courses_offered.csv";
const std::string COURSES_NOT_OFFERED_PATH = "student_output/courses_not_offered.csv";

/**
 * Represents a course a student can take in ExploreCourses.
 * You must fill in the types of the fields in this struct.
 * Hint: Remember what types C++ streams work with?!
 */
struct Course {
  std::string title;
  int number_of_units;
  std::string quarter;
};

/**
 * Note:
 * We need to #include utils.cpp _after_ we declare the Course struct above
 * so that the code inside utils.cpp knows what a Course is.
 * Recall that #include literally copies and pastes file contents.
 */
#include "utils.cpp"

/**
 * This function should populate the `courses` vector with structs of type
 * `Course`. We want to create these structs with the records in the courses.csv
 * file, where each line is a record!
 *
 * Hints:
 * 1) Take a look at the split function we provide in utils.cpp
 * 2) Each LINE is a record! *this is important, so we're saying it again :>)*
 * 3) The first line in the CSV defines the column names, so you can ignore it!
 *
 * @param filename The name of the file to parse.
 * @param courses  A vector of courses to populate.
 */
void parse_csv(std::string filename, std::vector<Course>& courses) {
  std::ifstream file(filename); //need an ifstream because we are reading in input from a file
  std::string out;
  getline(file, out); // Skip the first line (column headers). 
  while(getline(file, out)) { // out now contains one line of the file and returns file.fail(), do this until we hit the end of the file. We fail anyway when we try to getline at the eof.
    std::vector<std::string> Out(split(out, ',')); // Create a vector of strings containing the 3 elements of out split by comma delimiter
    std::istringstream numUnits(Out[1]); //need istringstream to read in from the string that needs to change to an int
    int units; //need something to hold int for when we convert it
    numUnits >> units; // read the string into an int
    Course course{Out[0], units, Out[2]}; //finally construct the struct using first and third elements that are strings, and the second which is an int
    courses.push_back(course); //write this struct to the end of courses
  }
  return;
}

/**
 * This function has TWO requirements.
 *
 * 1) Write the courses that are offered to the file
 * "student_output/courses_offered.csv"
 *
 * 2) Delete the courses that are offered from the `all_courses` vector.
 * IMPORTANT: do this after you write out to the file!
 *
 * HINTS:
 * 1) Keep track of the classes that you need to delete!
 * 2) Use the delete_elem_from_vector function we give you!
 * 3) Remember to write the CSV column headers at the beginning of the output!
 *    See courses.csv for reference.
 *
 * @param all_courses A vector of all courses gotten by calling `parse_csv`.
 *                    This vector will be modified by removing all offered courses.
 */
void write_courses_offered(std::vector<Course>& all_courses) {
  std::ofstream Offered(COURSES_OFFERED_PATH);
  std::vector<Course> deleteCourses; //vector to track actual courses to eventually remove when we are done iterating
  Offered << "Title,Number of Units,Quarter" << "\n";
  for (int i = 0; i < all_courses.size(); i++) {
    if (all_courses[i].quarter != "null") { //easier than C
      // it's offerred, so we add to the file and add to the array to be deleted
      Offered << all_courses[i].title << "," << all_courses[i].number_of_units << "," << all_courses[i].quarter << "\n"; //this syntax sucks lmao.
      deleteCourses.push_back(all_courses[i]); //add this index to delete array
    } //else not offerred, skip. No else statement at all i believe?
  }
  // now delete the courses:
  for (int i = 0; i < deleteCourses.size(); i++){
    delete_elem_from_vector(all_courses, deleteCourses[i]);
  }
  return;
}

/**
 * This function writes the courses NOT offered to the file
 * "student_output/courses_not_offered.csv".
 *
 * This function is ALWAYS called after the `write_courses_offered` function.
 * `unlisted_courses` will trivially contain classes that are not offered
 * since you delete offered classes from `all_courses` in the
 * `write_courses_offered` function.
 *
 * HINT: This should be VERY similar to `write_courses_offered`
 *
 * @param unlisted_courses A vector of courses that are not offered.
 */
void write_courses_not_offered(const std::vector<Course>& unlisted_courses) {
  std::ofstream NotOfferred(COURSES_NOT_OFFERED_PATH);
  NotOfferred << "Title,Number of Units,Quarter" << "\n";
  for (int i = 0; i < unlisted_courses.size(); i++) { // just iterate through remaining stuff and copy em over
    NotOfferred << unlisted_courses[i].title << "," << unlisted_courses[i].number_of_units << "," << unlisted_courses[i].quarter << "\n";
  }
  return;
}

int main() {
  /* Makes sure you defined your Course struct correctly! */
  static_assert(is_valid_course<Course>, "Course struct is not correctly defined!");

  std::vector<Course> courses;
  parse_csv("courses.csv", courses);

  /* Uncomment for debugging... */
  // print_courses(courses);

  write_courses_offered(courses);
  write_courses_not_offered(courses);

  return run_autograder();
}