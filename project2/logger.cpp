/**
 * logger.cpp
 * 
 * @brief 
 *      This class defines the project logger. It is a singleton that can be
 *      accessed via the LOG macro to intelligently write to files and screen
 *      according to importance level.
 *
 * @author
 *      Shawn Hanna
 *      Tom Nason
 *      Joel Griffith
 *
 **/

#include "logger.h"
#include <cstdio>
#include <cstdlib>
#include <sys/stat.h>
#include <errno.h>

/**************************************
 * Definition: Creates a new logger with a default importance of low
 **************************************/
Logger::Logger()
: _files(), _importanceLevel(LOG_LOW) {}

Logger::~Logger() {
    // loop through the map and close all files
    std::map<std::string, FILE*>::iterator iter;
    for (iter = _files.begin(); iter != _files.end(); iter++) {
        fclose(iter->second);
    }
}
/**************************************
 * Definition: Sets the importance level of the logger to some new value.
 *
 * Parameters: an integer specifying importance level
 *           
 *             The logger will only write messages to file and screen
 *             that are of the same or higher importance as its threshold
 **************************************/
void Logger::setImportanceLevel(int importanceLevel) {
    _importanceLevel = importanceLevel;
}

/**************************************
 * Definition: Writes output to file and screen, adds a new line, and flushes
 *
 * Parameters: an int importance level, string containing a filename,
 *             a format string, and the format string's parameters
 **************************************/
void Logger::write(int level, std::string filename, const char *formatString, ...) {
    // get a reference to the variable amount of args passed in
    va_list listPointer;
    va_start(listPointer, formatString);

    // send it to our private vprintf method which knows how to handle
    // our list pointer
    _vprintf(level, filename, formatString, listPointer);
    // flush to console and file since it's probably what we want
    flushFile(filename);
    flushScreen(filename);
    // add a newline
    printf(level, filename, "\n");

    va_end(listPointer);
}

/**************************************
 * Definition: Writes output to file and screen and flushes
 *
 * Parameters: an int importance level, string containing a filename,
 *             a format string, and the format string's parameters
 **************************************/
void Logger::printf(int level, std::string filename, const char *formatString, ...) { 
    // get a reference to the variable amount of args passed in
    va_list listPointer;
    va_start(listPointer, formatString);

    // use our private method which does the actual printing
    _vprintf(level, filename, formatString, listPointer);
    // flush to console and file, since it's probably what we want
    flushFile(filename);
    flushScreen(filename);

    va_end(listPointer);
}

/**************************************
 * Definition: Writes output to file, but does not flush for efficiency
 *
 * Parameters: an int importance level, string containing a filename,
 *             a format string, and the format string's parameters
 **************************************/
void Logger::printfFile(int level, std::string filename, const char *formatString, ...) { 
    // get a reference to the variable amount of args passed in
    va_list listPointer;
    va_start(listPointer, formatString);

    // use our private method to do the printing to file
    _vprintfFile(level, filename, formatString, listPointer);

    va_end(listPointer);
}

/**************************************
 * Definition: Writes output to screen and flushes
 *
 * Parameters: an int importance level, string containing a filename,
 *             a format string, and the format string's parameters
 **************************************/
void Logger::printfScreen(int level, std::string filename, const char *formatString, ...) { 
    // get a reference to the variable amount of args passed in
    va_list listPointer;
    va_start(listPointer, formatString);

    // use our private method to do the printing to screen
    _vprintfScreen(level, filename, formatString, listPointer);
    flushScreen(filename);

    va_end(listPointer);
}

/**************************************
 * Definition: Flushes to the specified file, if it exists
 *
 * Parameters: a string containing a filename,
 **************************************/
void Logger::flushFile(std::string filename) {
    FILE *file;
    std::map<std::string, FILE*>::iterator iter = _files.find(filename);
    if (iter != _files.end()) {
        file = _files[filename];
        fflush(file);
    }
}

/**************************************
 * Definition: Flushes to the screen for the given "filename" (read: key)
 *
 * Parameters: a string containing a filename,
 **************************************/
void Logger::flushScreen(std::string filename) {
    fflush(stdout);
}


void Logger::_vprintf(int level, std::string filename, const char *formatString, va_list listPointer) {
    // we need a second copy of the pointer so we can both print to file
    // and print to screen
    va_list listPointerF;
    va_copy(listPointerF, listPointer);

    _vprintfScreen(level, filename, formatString, listPointer);
    _vprintfFile(level, filename, formatString, listPointerF);

    // we can get rid of the latter copy now
    va_end(listPointerF);
}

void Logger::_vprintfScreen(int level, std::string filename, const char *formatString, va_list listPointer) {
    if (level >= _importanceLevel) {
        // the importance level of this message is high enough to
        // warrant logging it

        // print the message (and its args) to screen
        vfprintf(stdout, formatString, listPointer);
    }
}

void Logger::_vprintfFile(int level, std::string filename, const char *formatString, va_list listPointer) {
    if (level >= _importanceLevel) {
        // the importance level of this message is high enough to
        // warrant logging it

        // is the file already open?
        FILE *file;
        std::map<std::string, FILE*>::iterator iter = _files.find(filename);
        if (iter == _files.end()) {
            // it's not open yet, so we must create it
            std::string filePath;
  
            std::string workingDir(getcwd(NULL, 0));
            // store it in the logs directory
            std::string outputStr("/logs/");
            filePath = workingDir + outputStr;
            if (mkdir(filePath.c_str(), S_IREAD | S_IWRITE | S_IEXEC) != 0 &&
                errno != EEXIST) {
                fprintf(stderr, "Unable to create directory!\n");
                return;
            }

            // place it in a folder for the date
            time_t rawTime;
            time(&rawTime);

            char dateDir[80];
            strftime(dateDir, 80, "%Y-%m-%d/", localtime(&rawTime));
            std::string dateStr(dateDir);
            filePath += dateStr;
            if (mkdir(filePath.c_str(), S_IREAD | S_IWRITE | S_IEXEC) != 0 &&
                errno != EEXIST) {
                fprintf(stderr, "Unable to create directory!\n");
                return;
            }

            // give the file an extension
            filePath += filename + ".txt";
            // check if it exists already
            if (fopen(filePath.c_str(), "r") != NULL) {
                // if it does, give it an additional numbered extension
                std::string newFilePath;
                char i = 1;
                char numberExt[10];
                sprintf(numberExt, ".%d", i);
                std::string numberExtStr(numberExt);
                newFilePath = filePath + numberExtStr;
                while (fopen(newFilePath.c_str(), "r") != NULL) {
                    i++;
                    sprintf(numberExt, ".%d", i);
                    std::string numberExtStr(numberExt);
                    newFilePath = filePath + numberExtStr;
                }
                filePath = newFilePath;
            }

            // open it for writing
            file = fopen(filePath.c_str(), "w");
            if (file == NULL) {
                fprintf(stderr, "Unable to write to output file!\n");
                return;
            }

            // stick a reference to the file in the map indexed by name
            _files.insert(std::pair<std::string, FILE*>(filename, file));
        }
        else {
            // the file already exists, so let's grab it
            file = _files[filename];
        }

        // print the message out to file and flush it right away
        vfprintf(file, formatString, listPointer);
    }
}    