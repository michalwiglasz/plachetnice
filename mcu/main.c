#include <stdlib.h>
#include <fitkitlib.h>
#include "servo.h"

/*******************************************************************************
 * Vypis uzivatelske napovedy (funkce se vola pri vykonavani prikazu "help")
*******************************************************************************/
void print_user_help(void)
{
  term_send_str("Plachetnice:"); //Vypis retezce na terminal
  term_send_crlf();        //Prechod na novy radek
}

/*******************************************************************************
 * Dekodovani uzivatelskych prikazu a jejich vykonavani
*******************************************************************************/
unsigned char decode_user_cmd(char *cmd_ucase, char *cmd)
{
  if (strcmp2(cmd_ucase, "A ")) {
    unsigned long usec = strtoul(cmd + 2, NULL, 10);
    if (usec) {
      servo_a_set_width_usec(usec);

    } else {
      term_send_str("Zadejte platnou sirku stridy.");
    }

  } else if (strcmp3(cmd_ucase, "AP ")) {
    unsigned long usec = strtoul(cmd + 5, NULL, 10);
    if (usec) {
      servo_a_set_max_usec(usec);

    } else {
      term_send_str("Zadejte platnou periodu.");
    }

  } else if (strcmp2(cmd_ucase, "AP")) {
      unsigned long max = servo_a_get_max_usec();
      term_send_str("Perioda serva A je ");
      term_send_num(max);
      term_send_str(" us.");
      term_send_crlf();

  } else if (strcmp3(cmd_ucase, "A++")) {
      unsigned long usec = servo_a_get_width_usec();
      servo_a_set_width_usec(usec + 500);

    } else if (strcmp3(cmd_ucase, "A--")) {
      unsigned long usec = servo_a_get_width_usec();
      servo_a_set_width_usec(usec - 500);

  } else if (strcmp2(cmd_ucase, "A+")) {
      unsigned long usec = servo_a_get_width_usec();
      servo_a_set_width_usec(usec + 100);

  } else if (strcmp2(cmd_ucase, "A-")) {
      unsigned long usec = servo_a_get_width_usec();
      servo_a_set_width_usec(usec - 100);

  } else if (strcmp2(cmd_ucase, "AL")) {
      servo_a_set_width_usec(SERVO_LEFT);

  } else if (strcmp2(cmd_ucase, "AC")) {
      servo_a_set_width_usec(SERVO_CENTER);

  } else if (strcmp2(cmd_ucase, "AR")) {
      servo_a_set_width_usec(SERVO_RIGHT);

  } else if (strcmp1(cmd_ucase, "A")) {
    // see below

  } else {
    return (CMD_UNKNOWN);
  }

  unsigned long usec = servo_a_get_width_usec();
  term_send_str("Strida serva A je ");
  term_send_num(usec);
  term_send_str(" us.");
  term_send_crlf();
  return USER_COMMAND;
}

/*******************************************************************************
 * Inicializace periferii/komponent po naprogramovani FPGA
*******************************************************************************/
void fpga_initialized()
{
}

/*******************************************************************************
 * Hlavni funkce
*******************************************************************************/
int main(void)
{
  initialize_hardware();

  servo_a_init();
  set_led_d6(1);

  unsigned long usec = servo_a_get_width_usec();
  term_send_str("Strida serva A nastavena na ");
  term_send_num(usec);
  term_send_str(" us.");
  term_send_crlf();

  while (1) {
    delay_ms(10);
    terminal_idle();  // obsluha terminalu
  }
}

