#include <stdlib.h>
#include <fitkitlib.h>
#include "servo.h"

// stezen
servo_t s_mast = {
  .addr = 0x00A0,
  .period = SERVO_HS422_PERIOD,
  .left = 500,
  .right = 2300,
  .center = 1400,
  .left_angle = -90,
  .right_angle = 90,
  .center_angle = 0,
};

servo_t s_wheels = {
  .addr = 0x00B0,
  .period = SERVO_HS422_PERIOD,
  .left = 1040,
  .right = 1740,
  .center = 1390,
  .left_angle = -45,
  .right_angle = 45,
  .center_angle = 0,
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
  if (strcmp2(cmd_ucase, "M ")) {
    long angle = strtol(cmd + 2, NULL, 10);
    servo_go_angle(&s_mast, angle);

  } else if (strcmp3(cmd_ucase, "MW ")) {
    uint16_t usec = strtoul(cmd + 3, NULL, 10);
    if (usec) {
      servo_set_width(&s_mast, usec);

    } else {
      term_send_str("Zadejte platnou sirku stridy.\n");
    }

  } else if (strcmp3(cmd_ucase, "M++")) {
      uint16_t usec = servo_get_width(&s_mast);
      servo_set_width(&s_mast, usec + 500);

    } else if (strcmp3(cmd_ucase, "M--")) {
      uint16_t usec = servo_get_width(&s_mast);
      servo_set_width(&s_mast, usec - 500);

  } else if (strcmp2(cmd_ucase, "M+")) {
      uint16_t usec = servo_get_width(&s_mast);
      servo_set_width(&s_mast, usec + 100);

  } else if (strcmp2(cmd_ucase, "M-")) {
      uint16_t usec = servo_get_width(&s_mast);
      servo_set_width(&s_mast, usec - 100);

  } else if (strcmp2(cmd_ucase, "ML")) {
      servo_go_left(&s_mast);

  } else if (strcmp2(cmd_ucase, "MC")) {
      servo_go_center(&s_mast);

  } else if (strcmp2(cmd_ucase, "MR")) {
      servo_go_right(&s_mast);


  } else if (strcmp2(cmd_ucase, "W ")) {
    long angle = strtol(cmd + 2, NULL, 10);
    servo_go_angle(&s_wheels, angle);

  } else if (strcmp3(cmd_ucase, "WW ")) {
    uint16_t usec = strtoul(cmd + 3, NULL, 10);
    if (usec) {
      servo_set_width(&s_wheels, usec);

    } else {
      term_send_str("Zadejte platnou sirku stridy.\n");
    }

  } else if (strcmp3(cmd_ucase, "W++")) {
      uint16_t usec = servo_get_width(&s_wheels);
      servo_set_width(&s_wheels, usec + 500);

    } else if (strcmp3(cmd_ucase, "W--")) {
      uint16_t usec = servo_get_width(&s_wheels);
      servo_set_width(&s_wheels, usec - 500);

  } else if (strcmp2(cmd_ucase, "W+")) {
      uint16_t usec = servo_get_width(&s_wheels);
      servo_set_width(&s_wheels, usec + 100);

  } else if (strcmp2(cmd_ucase, "W-")) {
      uint16_t usec = servo_get_width(&s_wheels);
      servo_set_width(&s_wheels, usec - 100);

  } else if (strcmp2(cmd_ucase, "WL")) {
      servo_go_left(&s_wheels);

  } else if (strcmp2(cmd_ucase, "WC")) {
      servo_go_center(&s_wheels);

  } else if (strcmp2(cmd_ucase, "WR")) {
      servo_go_right(&s_wheels);



  } else if (strcmp1(cmd_ucase, "I")) {
    // see below

  } else {
    return (CMD_UNKNOWN);
  }

  uint16_t usec = servo_get_width(&s_mast);
  term_send_str("Stezen:");
  term_send_num(usec);
  term_send_str(" us.");
  term_send_crlf();

  usec = servo_get_width(&s_wheels);
  term_send_str("Kola:");
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
  servo_init(&s_mast);
  servo_init(&s_wheels);
  set_led_d6(1);
  set_led_d5(1);

  term_send_crlf();
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

  term_send_crlf();
  term_send_str("Kola\nAdresa: ");
  term_send_num(s_wheels.addr);
  term_send_str("\nPerioda: ");
  term_send_num(s_wheels.period);
  term_send_str("\nLevy okraj: ");
  term_send_num(s_wheels.left);
  term_send_str("\nPravy okraj: ");
  term_send_num(s_wheels.right);
  term_send_str("\nStred: ");
  term_send_num(s_wheels.center);
  term_send_crlf();

  while (1) {
    delay_ms(10);
    terminal_idle();  // obsluha terminalu
  }
}

