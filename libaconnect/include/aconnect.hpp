#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdarg.h>
#include <sys/ioctl.h>
#include <alsa/asoundlib.h>
#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;

struct alsa_port
{
  int num;
  string name;
  int type;
};

struct alsa_client
{
  int num;
  string name;
  int type;
  vector<alsa_port> ports;
};

struct alsa_connection_list
{
  vector<alsa_client> clients;
};

alsa_connection_list* aconnect_get_input_connection_list();
alsa_connection_list* aconnect_get_output_connection_list();
bool aconnect_enumerate_all_clients();
