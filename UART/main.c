#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "board.h"
#include "periph/uart.h"
#include "lptimer.h"

#define RCV_QUEUE_SIZE (16)

static char receiver_thread_stack[THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF];
static kernel_pid_t receiver_thread_pid;
static msg_t rcv_queue[RCV_QUEUE_SIZE];

static void *receiver_thread(void *arg);
static void uart_rx_callback(void *arg, uint8_t data);

int main(void)
{
    //example to read uart
  int res = uart_init(UART_STDIO_DEV, 115200, uart_rx_callback, NULL);
  if (res != UART_OK)
  {
    printf("uart unavailable\n");
    return -1;
  }

  receiver_thread_pid = thread_create(receiver_thread_stack,
                                      sizeof(receiver_thread_stack),
                                      (THREAD_PRIORITY_MAIN - 1),
                                      THREAD_CREATE_STACKTEST,
                                      receiver_thread,
                                      NULL,
                                      "receiver thread");

  return 0;
}

static void *receiver_thread(void *arg)
{
  (void)arg;

  printf("reciver started\n");
  msg_init_queue(rcv_queue, RCV_QUEUE_SIZE);

  msg_t msg;

  while (1)
  {
    msg_receive(&msg);
    printf("%02X\n\r", (uint8_t)msg.content.value);
  }

  return NULL;
}

static void uart_rx_callback(void *arg, uint8_t data)
{
  (void)arg;
  msg_t msg;
  
  msg.content.value = data;
  msg_send_int(&msg, receiver_thread_pid);
}
