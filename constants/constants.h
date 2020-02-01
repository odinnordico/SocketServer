#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <limits.h>

const int JSON_IDENTATION = 2;
const int DESTINATION_BROADCAST = INT_MAX;
const int MAX_CLIENTS_ALLOWED = 100;

static const char *const CHAT_ACTION_ACK = "ACK";
static const char *const CHAT_ACTION_START = "START";
static const char *const CHAT_ACTION_END = "END";
static const char *const CHAT_ACTION_MSG = "MSG";

static const char *const LABEL_DESTINATION = "destination";
static const char *const LABEL_SOURCE = "source";
static const char *const LABEL_ACTION = "action";
static const char *const LABEL_MSG = "msg";
static const char *const LABEL_SERVER = "server";
static const char *const LABEL_CLIENT = "client";
static const char *const LABEL_IP = "ip";
static const char *const LABEL_PORT = "port";
static const char *const LABEL_SOCKET = "socket";

#endif /* CONSTANTS_H_ */
