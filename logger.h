#pragma once

#ifndef __LOGGER_H__
#define __LOGGER_H__

/**
@brief initialize logger, cleans up automatically

@brief the path where you want your log document to go to
*/
void loggerInit(const char *logPath);

/**
  @brief logs a message to stdout and to the configured log file
  @param msg a string with tokens
  @param ... variables to be put into the tokens.
*/
#define slog(...) _slog(__FILE__,__LINE__,__VA_ARGS__)
void _slog(char *f, int l, char *msg, ...);

#endif