/*
 Copyright (c) 2016 Anton Valentinov Kirilov

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
 OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef EVENT_LOOP_H_

#define EVENT_LOOP_H_

#include <h2o.h>
#include <stdbool.h>
#include <stddef.h>

#include "global_data.h"

#define CONN_NUM_SAMPLES 512

typedef enum {
	SHUTDOWN,
	TASK
} message_type_t;

struct thread_context_t;

typedef struct {
	h2o_socket_t *h2o_https_socket;
	h2o_socket_t *h2o_socket;
	size_t conn_num;
	h2o_accept_ctx_t h2o_accept_ctx;
	h2o_context_t h2o_ctx;
	h2o_linklist_t local_messages;
	size_t accepted_conn_num;
	size_t conn_num_sample[CONN_NUM_SAMPLES];
	size_t conn_num_sample_idx;
} event_loop_t;

typedef struct {
	message_type_t type;
	h2o_multithread_message_t super;
} message_t;

typedef struct {
	message_t super;
	void *arg;
	void (*task)(void *);
} task_message_t;

void event_loop(struct thread_context_t *ctx);
void free_event_loop(event_loop_t *event_loop, h2o_multithread_receiver_t *h2o_receiver);
void initialize_event_loop(bool is_main_thread,
                           global_data_t *global_data,
                           h2o_multithread_receiver_t *h2o_receiver,
                           event_loop_t *loop);
void send_local_message(message_t *msg, h2o_linklist_t *local_messages);
void send_message(message_t *msg, h2o_multithread_receiver_t *h2o_receiver);

#endif // EVENT_LOOP_H_
