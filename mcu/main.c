#include <stdlib.h>
#include <fitkitlib.h>
#include "servo.h"

char last_ch; //naposledy precteny znak

// stezen
servo_t s_mast = {
  .addr = 0x00A0,
  .period = SERVO_HS422_PERIOD,
  .left = SERVO_HS422_LEFT,
  .right = SERVO_HS422_RIGHT,
  .center = SERVO_HS422_CENTER,
};

servo_t s_wheels = {
  .addr = 0x00B0,
  .period = SERVO_HS422_PERIOD,
  .left = SERVO_HS422_LEFT,
  .right = SERVO_HS422_RIGHT,
  .center = SERVO_HS422_CENTER,
};

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
    uint16_t usec = strtoul(cmd + 2, NULL, 10);
    if (usec) {
      servo_set_width(&s_mast, usec);

    } else {
      term_send_str("Zadejte platnou sirku stridy.");
    }

  } else if (strcmp3(cmd_ucase, "AP ")) {
    uint16_t usec = strtoul(cmd + 3, NULL, 10);
    if (usec) {
      servo_set_period(&s_mast, usec);

    } else {
      term_send_str("Zadejte platnou periodu.");
    }

  } else if (strcmp2(cmd_ucase, "AP")) {
      uint16_t max = servo_get_period(&s_mast);
      term_send_str("Perioda serva A je ");
      term_send_num(max);
      term_send_str(" us.");
      term_send_crlf();

  } else if (strcmp3(cmd_ucase, "A++")) {
      uint16_t usec = servo_get_width(&s_mast);
      servo_set_width(&s_mast, usec + 500);

    } else if (strcmp3(cmd_ucase, "A--")) {
      uint16_t usec = servo_get_width(&s_mast);
      servo_set_width(&s_mast, usec - 500);

  } else if (strcmp2(cmd_ucase, "A+")) {
      uint16_t usec = servo_get_width(&s_mast);
      servo_set_width(&s_mast, usec + 100);

  } else if (strcmp2(cmd_ucase, "A-")) {
      uint16_t usec = servo_get_width(&s_mast);
      servo_set_width(&s_mast, usec - 100);

  } else if (strcmp2(cmd_ucase, "AL")) {
      servo_go_left(&s_mast);

  } else if (strcmp2(cmd_ucase, "AC")) {
      servo_go_center(&s_mast);

  } else if (strcmp2(cmd_ucase, "AR")) {
      servo_go_right(&s_mast);

  } else if (strcmp1(cmd_ucase, "A")) {
    // see below

  } else {
    return (CMD_UNKNOWN);
  }

  uint16_t usec = servo_get_width(&s_mast);
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
  last_ch = 0;

  initialize_hardware();
  servo_init(&s_mast);
  set_led_d6(1);

  term_send_str("Stezen\nAdresa: ");
  term_send_num(s_mast.addr);
  term_send_str("\nPerioda: ");
  term_send_num(s_mast.period);
  term_send_str("\nLevy okraj: ");
  term_send_num(s_mast.left);
  term_send_str("\nPravy okraj: ");
  term_send_num(s_mast.right);
  term_send_str("\nStred: ");
  term_send_num(s_mast.center);
  term_send_crlf();

  uint16_t usec = servo_get_width(&s_mast);
  term_send_str("Strida serva A nastavena na ");
  term_send_num(usec);
  term_send_str(" us.");
  term_send_crlf();

  while (1) {
    delay_ms(10);
    terminal_idle();  // obsluha terminalu
  }
}

