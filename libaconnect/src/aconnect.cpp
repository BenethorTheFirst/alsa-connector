//Adapted from source code by Takashi Iwai (aconnect.cxx) containing the following:
/*
 * connect / disconnect two subscriber ports
 *   ver.0.1.3
 *
 * Copyright (C) 1999 Takashi Iwai
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 */

#include "aconnect.hpp"
#include "asoundlib.h"

#define LIST_INPUT	1
#define LIST_OUTPUT	2

typedef void (*action_func_t)(snd_seq_t *seq, snd_seq_client_info_t *cinfo, snd_seq_port_info_t *pinfo, int count, bool input);

#define perm_ok(pinfo,bits) ((snd_seq_port_info_get_capability(pinfo) & (bits)) == (bits))

static alsa_connection_list g_input_connection_list;
static alsa_connection_list g_output_connection_list;

static void print_port_and_subs(snd_seq_t *seq, snd_seq_client_info_t *cinfo,
				snd_seq_port_info_t *pinfo, int count);
static void do_search_port(snd_seq_t *seq, int perm, action_func_t do_action);
static void append_port(snd_seq_t *seq, snd_seq_client_info_t *cinfo,
		       snd_seq_port_info_t *pinfo, int count, bool input);
static void print_port(snd_seq_t *seq, snd_seq_client_info_t *cinfo,
		       snd_seq_port_info_t *pinfo, int count);
static void list_subscribers(snd_seq_t *seq, const snd_seq_addr_t *addr);

static void print_port_and_subs(snd_seq_t *seq, snd_seq_client_info_t *cinfo,
				snd_seq_port_info_t *pinfo, int count)
{
	print_port(seq, cinfo, pinfo, count);
	list_subscribers(seq, snd_seq_port_info_get_addr(pinfo));
}

static void append_port_and_subs(snd_seq_t *seq, snd_seq_client_info_t *cinfo,
				snd_seq_port_info_t *pinfo, int count, bool input)
{
	append_port(seq, cinfo, pinfo, count, input);
	//list_subscribers(seq, snd_seq_port_info_get_addr(pinfo));
}

alsa_connection_list* aconnect_get_input_connection_list()
{
  return &g_input_connection_list;
}

alsa_connection_list* aconnect_get_output_connection_list()
{
  return &g_output_connection_list;
}

bool aconnect_enumerate_all_clients()
{
  snd_seq_t *seq = 0;
  if (snd_seq_open(&seq, "default", SND_SEQ_OPEN_DUPLEX, 0) < 0) {
		fprintf(stderr, "can't open sequencer\n");
		return false;
	}
  do_search_port(seq, LIST_INPUT, append_port_and_subs);
  do_search_port(seq, LIST_OUTPUT, append_port_and_subs);
  return true;
}
static int check_permission(snd_seq_port_info_t *pinfo, int perm)
{
	if (perm) {
		if (perm & LIST_INPUT) {
			if (perm_ok(pinfo, SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ))
				goto __ok;
		}
		if (perm & LIST_OUTPUT) {
			if (perm_ok(pinfo, SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE))
				goto __ok;
		}
		return 0;
	}
 __ok:
	if (snd_seq_port_info_get_capability(pinfo) & SND_SEQ_PORT_CAP_NO_EXPORT)
		return 0;
	return 1;
}

static void do_search_port(snd_seq_t *seq, int perm, action_func_t do_action)
{
	snd_seq_client_info_t *cinfo;
	snd_seq_port_info_t *pinfo;
	int count;

	snd_seq_client_info_alloca(&cinfo);
	snd_seq_port_info_alloca(&pinfo);
	snd_seq_client_info_set_client(cinfo, -1);
	while (snd_seq_query_next_client(seq, cinfo) >= 0) {
		/* reset query info */
		snd_seq_port_info_set_client(pinfo, snd_seq_client_info_get_client(cinfo));
		snd_seq_port_info_set_port(pinfo, -1);
		count = 0;
		while (snd_seq_query_next_port(seq, pinfo) >= 0) {
			if (check_permission(pinfo, perm)) {
        if (perm == LIST_INPUT)
          do_action(seq, cinfo, pinfo, count, true);
        else
          do_action(seq, cinfo, pinfo, count, false);
				count++;
			}
		}
	}
}

static void list_each_subs(snd_seq_t *seq, snd_seq_query_subscribe_t *subs, int type, const char *msg)
{
	int count = 0;
	snd_seq_query_subscribe_set_type(subs, (snd_seq_query_subs_type_t)type);
	snd_seq_query_subscribe_set_index(subs, 0);
	while (snd_seq_query_port_subscribers(seq, subs) >= 0) {
		const snd_seq_addr_t *addr;
		addr = snd_seq_query_subscribe_get_addr(subs);
		printf("%d:%d", addr->client, addr->port);
		if (snd_seq_query_subscribe_get_exclusive(subs))
			printf("[ex]");
		if (snd_seq_query_subscribe_get_time_update(subs))
			printf("[%s:%d]",
			       (snd_seq_query_subscribe_get_time_real(subs) ? "real" : "tick"),
			       snd_seq_query_subscribe_get_queue(subs));
		snd_seq_query_subscribe_set_index(subs, snd_seq_query_subscribe_get_index(subs) + 1);
	}
	if (count > 0)
		printf("\n");
}

static void list_subscribers(snd_seq_t *seq, const snd_seq_addr_t *addr)
{
	snd_seq_query_subscribe_t *subs;
	snd_seq_query_subscribe_alloca(&subs);
	snd_seq_query_subscribe_set_root(subs, addr);
	list_each_subs(seq, subs, SND_SEQ_QUERY_SUBS_READ, "Connecting To");
	list_each_subs(seq, subs, SND_SEQ_QUERY_SUBS_WRITE, "Connected From");
}

static void append_port(snd_seq_t *seq, snd_seq_client_info_t *cinfo,
		       snd_seq_port_info_t *pinfo, int count, bool input)
{
	if (! count) {
    alsa_client client;
    client.num = snd_seq_client_info_get_client(cinfo);
    client.name = snd_seq_client_info_get_name(cinfo);
    client.type = snd_seq_client_info_get_type(cinfo);
    if (input)
      g_input_connection_list.clients.push_back(client);
    else
      g_output_connection_list.clients.push_back(client);
	}
	alsa_port port;
  port.num = snd_seq_port_info_get_port(pinfo);
  port.name = snd_seq_port_info_get_name(pinfo);
  port.type = snd_seq_port_info_get_type(pinfo);
  if (input)
    g_input_connection_list.clients.back().ports.push_back(port);
  else
    g_output_connection_list.clients.back().ports.push_back(port);
}

static void print_port(snd_seq_t *seq, snd_seq_client_info_t *cinfo,
		       snd_seq_port_info_t *pinfo, int count)
{
	if (! count) {
		printf("client %d: '%s' [type=%s]\n",
		       snd_seq_client_info_get_client(cinfo),
		       snd_seq_client_info_get_name(cinfo),
		       (snd_seq_client_info_get_type(cinfo) == SND_SEQ_USER_CLIENT ? "user" : "kernel"));
	}
	printf("  %3d '%-16s' %d\n",
	       snd_seq_port_info_get_port(pinfo),
	       snd_seq_port_info_get_name(pinfo),
	       snd_seq_port_info_get_type(pinfo)
				 );

	printf("  %3d '%-16s'\n",
	       snd_seq_port_info_get_port(pinfo),
	       snd_seq_port_info_get_name(pinfo));
}

